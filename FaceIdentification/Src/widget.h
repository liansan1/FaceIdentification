#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTimerEvent>//定时器事件
#include <QTimer>
#include <QFileDialog>  //文件对话框
#include <vector>
#include <QMessageBox>
#include "daofactory.h"
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <QDateTime>
using namespace cv::face;
using namespace cv;
using namespace std;
#include "historyform.h"



#define FACEMODEL "E:/opencv_20075/opencv3.4-install/install/etc/haarcascades/haarcascade_frontalface_alt2.xml"
#define FACETOOL "faces.xml"
//我们需要将人脸和标签都学习到对应的faces.xml
#define FACECOUNT 10   //学习人脸的数量

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    //1、初识摄像头相关
    void InitVideo();
    //2、初始化人脸相关(级联分类器和人脸识别器)
    void InitFaceTool();
    //3、重写定时器函数
    void timerEvent(QTimerEvent *e);
    //4、将视频帧图像放置到Label上
    void drawPic(Mat frame);
    //5、黑化处理 有效识别区域亮度不变 周围区域变暗
    void BlackFrame(Mat &frame);
    //6、查找人脸
    void FindFace();
    //7、绘制红色的矩形框
    void drawFace();
    //8、识别人脸
    int checkFace();
    //9、显示XXX已打卡
    void showName(int label);
    ~Widget();

public slots:
    void studyTimerEvent();//处理学习人脸时的槽函数
    void showNameTimerEvent();//处理识别人脸时显示人名用的定时器
    void dealbut();

private slots:
    void on_pushButton_study_clicked();

    void on_pushButton_history_clicked();

private:
    Ui::Widget *ui;
    //操作摄像头或者视频的类对象
    VideoCapture vc;
    Mat frame;//读取的视频帧对象
    Rect enableRect;//有效识别区域
    int timerId;//关联定时器
    DaoFactory *daofactory;


    QTimer studyFacesTimer;//定义学习人脸用的定时器
    QTimer showNameTimer;  //定义识别人脸时显示人名用的定时器
    CascadeClassifier classifier;//级联分类器对象 识别是否是一个人
    Ptr<FaceRecognizer> recognizer;//创建人脸识别器对象 确认是谁的脸
    vector<Rect>FindFaces;   //查找到的人脸的矩形集合
    vector<Mat>studyFaces;   //学习用的人脸


    bool isStudy;   //false 代表没有学习  true正在学习人脸
    //目的 防止学习人脸的时候进行 人脸的识别
    HistoryForm f;

};

#endif // WIDGET_H
