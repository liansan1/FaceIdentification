#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),isStudy(false),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    daofactory = DaoFactory::getInstance();//获得单例的指针
    InitVideo();
    InitFaceTool();
    //启动定时器 30ms startTimer
    timerId = startTimer(30);

    //最初学习人脸的按钮是不让点 除非编辑完名字
    ui->pushButton_study->setEnabled(false);//灰色的

    //关联信号与槽函数
    connect(ui->lineEdit,&QLineEdit::editingFinished,
            this,&Widget::dealbut);

    connect(&studyFacesTimer,&QTimer::timeout,
            this,&Widget::studyTimerEvent);

    connect(&showNameTimer,&QTimer::timeout,
            this,&Widget::showNameTimerEvent);
}

void Widget::InitVideo()
{
    //1 打开摄像头 open(0)
    if(false == vc.open(0)){
        QMessageBox::warning(this,"open","摄像头打开失败");
    }
    //2 读取一帧图像 目的：为了确定enableRect
    vc >> frame;
    enableRect = Rect(frame.cols/2-150,
                      frame.rows/2-150,
                      300,
                      300);

    //3 重置Labelshow的大小
    ui->labelshow->resize(QSize(frame.cols,frame.rows));

}

void Widget::InitFaceTool()
{
    //初始化级联分类器
    classifier.load(FACEMODEL);
    if(classifier.empty()==true){
        QMessageBox::warning(this,"warn","级联分类器初始化失败");
    }

    //初始化人脸识别
    QFile file(FACETOOL);
    if(file.exists()==true){ //曾经学习过该模型
        recognizer
                = FaceRecognizer::load<LBPHFaceRecognizer>(FACETOOL);
    }else{
        recognizer  = LBPHFaceRecognizer::create();
    }
}

void Widget::timerEvent(QTimerEvent *e)
{
    //读取一帧图像
    vc >> frame;
    //判断是否读取成功
    if(frame.data==NULL){
        qDebug() << "读取视频帧图像失败";
    }
    //以y轴翻转图像
    flip(frame,frame,1);
    //黑化处理 有效识别区域亮度不变 周围区域变暗BlackFrame(frame)
    BlackFrame(frame);
    FindFace();//查找人脸
    drawFace();//为找到的人脸绘制红色的矩形框
    int label = checkFace();//识别人脸
    //在Qt的label上显示图片 drawPic(frame)
    showName(label);
    drawPic(frame);
}

void Widget::drawPic(Mat frame)
{
    //色彩空间转换 BGR-->RGB
    Mat rgb = frame.clone();
    cvtColor(frame,rgb,CV_BGR2RGB);
    //生成QImage
    QImage img(rgb.data,
               rgb.cols,
               rgb.rows,
               rgb.cols*rgb.channels(),
               QImage::Format_RGB888);
    //借助于setPixmap函数将QImage图片放置到Label.
    ui->labelshow->setPixmap(QPixmap::fromImage(img));
}

void Widget::BlackFrame(Mat &frame)
{
    //绘制白色的有效识别区域 Scalar(255,255,255)
    rectangle(frame,enableRect,Scalar(255,255,255));
    //调整像素点 有效识别区域亮度不变
    for(int i=0;i<frame.cols;i++){
        for(int j=0;j<frame.rows;j++){
            //有效识别区域内的亮度保持不变
            if(enableRect.contains(Point(i,j))==true){
                continue;
            }
            //周围区域变暗
            frame.at<Vec3b>(j,i)[0] =
                    saturate_cast<uchar>(frame.at<Vec3b>(j,i)[0]-80);
            frame.at<Vec3b>(j,i)[1] =
                    saturate_cast<uchar>(frame.at<Vec3b>(j,i)[1]-80);
            frame.at<Vec3b>(j,i)[2] =
                    saturate_cast<uchar>(frame.at<Vec3b>(j,i)[2]-80);
        }
    }

}

Widget::~Widget()
{
    delete ui;
    daofactory->ReleaseInstance();
}
void Widget::FindFace(){
    //截图  取得有效范围内的Mat图片
    Mat gray = frame(enableRect);
    cvtColor(gray,gray,CV_BGR2GRAY);
    classifier.detectMultiScale(gray,FindFaces,1.1,10,0);
}

void Widget::drawFace()
{
    if(FindFaces.size() > 0){
        Rect r(FindFaces[0].x+enableRect.x,
                FindFaces[0].y+enableRect.y,
                FindFaces[0].width,
                FindFaces[0].height);
        rectangle(frame,r,Scalar(0,0,255));
    }
}
//********************showNameTimer*************//
int Widget::checkFace()
{
    if(isStudy==true||recognizer->empty()==true ||
            showNameTimer.isActive()==true){
        return -1;
    }
    int label = -1;
    //获取视频中有效效识别区域内的待识别的人脸图片
    if(FindFaces.size()>0){  //注意：判断有没有人脸
        Rect r(FindFaces[0].x+enableRect.x,
                FindFaces[0].y+enableRect.y,
                FindFaces[0].width ,
                FindFaces[0].height);

        Mat face = frame(r);//face就是学习用的图片
        if(face.data==NULL){
            return -1;
        }
        cvtColor(face,face,CV_BGR2GRAY);
        //重置图片的大小 100*100
        cv::resize(face,face,Size(100,100));


        double confidence = 0.0;
        recognizer->predict(face,label,confidence);
    }
    return label;
}

void Widget::showName(int label)
{
    if(label == -1){
        return;//说明没有这人的信息
    }
    UserEntity e;
    daofactory->getUserDao()->selectUser(label,e);

    QString str = QString("%1 已打卡 %2").arg(e.name)
            .arg(QDateTime::currentDateTime().toString());
    ui->label_showName->setText(str);

    //将打卡信息保存到历史记录表History
    daofactory->getClockDao()->insertClock(ClockEntity(e.name));

    //********************showNameTimer*************//
    //只要是显示了打卡信息  那么过1秒钟就清空label_showName
    if(showNameTimer.isActive()==false){ //定时器是否在工作
        showNameTimer.start(1000);
    }
    return;
}
void Widget::studyTimerEvent()
{
    if(FindFaces.size()==0){//说明此时没有待学习的人脸
        return;
    }else{
        //根据已经识别出来的矩形框(左上角)来获得学习用的人脸的图片
        Rect r(FindFaces[0].x+enableRect.x,
                FindFaces[0].y+enableRect.y,
                FindFaces[0].width,
                FindFaces[0].height);

        Mat face = frame(r);//face就是学习用的图片
        cvtColor(face,face,CV_BGR2GRAY);
        //重置图片的大小 100*100
        cv::resize(face,face,Size(100,100));
        if(face.data != NULL){
            studyFaces.push_back(face);
            qDebug() << studyFaces.size();
        }
        //判断是否加载了10张脸
        if(studyFaces.size()==FACECOUNT){
            //关闭定时器 不采集人脸
            studyFacesTimer.stop();
            //获得lineEdit上的名字
            QString name = ui->lineEdit->text();
            //将名字写入数据库
            daofactory->getUserDao()->insertUser(UserEntity(name));
            //获取刚刚插入的数据的id 作为学习用的标签
            int id = daofactory->getUserDao()->getId();
            vector<int>labels;
            labels.insert(labels.begin(),FACECOUNT,id);
            //插入的功能  类似pushback
            //更新模型并保存模型。
            recognizer->update(studyFaces,labels);
            recognizer->save(FACETOOL);
            //清空studyFaces
            studyFaces.clear();
            ui->pushButton_study->setEnabled(true);
            isStudy = false;
            QMessageBox::warning(this,"warn","学习人脸完成");
        }

    }
}

//********************showNameTimer*************//
void Widget::showNameTimerEvent()
{
    ui->label_showName->clear();
    showNameTimer.stop();
}

void Widget::dealbut()
{
    ui->pushButton_study->setEnabled(true);
}

void Widget::on_pushButton_study_clicked()
{
    //每1秒钟学习一次人脸 执行studyTimerEvent()
    studyFacesTimer.start(1000);//
    ui->pushButton_study->setEnabled(false);//不允许重复学习人脸
    isStudy=true;//开始学习人脸

}

void Widget::on_pushButton_history_clicked()
{
    //显示打卡记录的界面
    f.show();
}
