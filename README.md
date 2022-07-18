# FaceIdentification
人脸识别的c++程序
需求分析
##数据库的部分
单例模式
	class DaoFactory
	{
	private:
		DaoFactory();
		DaoFactory(const DaoFactory& other){}


		static DaoFactory* instance;

		QSqlDatabase db;
		ColckDao* clockDao;
		UserDao* pUser;

	public:
	   static DaoFactory* getInstance();//负责初始化instance
	   static void release();//负责释放instance
	   
	   inline ColckDao* getClockDao(){return clockDao;}
	   inline UserDao* getUserDao(){return pUser;}

		~DaoFactory();
	}
	
ColckEntity
class ClockEntity
{
public:
    ClockEntity();
    ~ClockEntity();
     ClockEntity(QString name, QDateTime time = QDateTime(), int id = 0);

    int id;
    QString name;
    QDateTime time;
};

ColckDao
class ColckDao
{
public:
    ColckDao();
    ~ColckDao();

    bool insertClock(ClockEntity e);
    bool selectClock(QDateTime start, QDateTime end, vector<ClockEntity>& v);
};


UserEntity
	class UserEntity
	{
	public:
		UserEntity();
		UserEntity(QString name,int id = 0);
		~UserEntity();

		int id;
		QString name;
	};
UserDao
	class UserDao
	{
	public:
		UserDao();
		~UserDao();

		bool insertUser(UserEntity e);
		bool selectUser(int id, UserEntity& e);
		//根据ID值 查询User信息  UserEntity& e是传出参数
		int getId();
		//获得插入的最后一条数据的id，
		//作为这个人的label 放入学习模型中
	};
	
	
##机器学习：
	一、label上显示黑化后的视频帧图像

		1、初识摄像头相关
			void InitVideo();
		2、初始化人脸相关(级联分类器和人脸识别器)
			void InitFaceTool();
		3、重写定时器函数
			void timerEvent(QTimerEvent *e);
		4、将视频帧图像放置到Label上
			void drawPic(Mat frame);
		5、黑化处理 有效识别区域亮度不变 周围区域变暗
			void BlackFrame(Mat &frame);
	
		##测试：实现timerEvent()方法	
			void Widget::timerEvent(QTimerEvent *e){
			//读取一帧图像
			vc >> frame;
			if(NULL == frame.data){
				QMessageBox::warning(this,"warn","读取视频帧图像失败");
			}
			flip(frame,frame,1);//以y轴进行翻转

			//黑化处理 有效识别区域亮度不变 周围区域变暗
			BlackFrame(frame);
			//在label上显示
			drawPic(frame);

		}
		
		
		
	二、学习人脸完成之后，将名字写入数据库中	
		1、学习人脸 按钮转到槽函数，开启学习人脸的定时器。
			private slots:
				void on_pushButton_study_clicked();
				void studyTimerEvent();
			
			信号与槽函数的连接
				connect(&studyFacesTimer,SIGNAL(timeout()),
						this,SLOT(studyTimerEvent()));
						
		2、实现studyTimerEvent()函数
			
	
	##测试：
		数据库中存入学习时的人名。
		
##人脸识别
	    int check();//识别图像中的人脸
		void showName(int label);
		
		void showNameTimerEvent(); 
		QTimer showNameTimer;
实例代码：
    connect(&showNameTimer, SIGNAL(timeout()),
            this, SLOT(showNameTimerEvent()));	

	
	##运行效果 --> 识别人脸并显示打卡成功
	
显示历史记录
ui->dateEdit_start->setDate(QDate::currentDate());
    ui->dateEdit_end->setDate(QDate::currentDate());
	
	
void ClockForm::on_dateEdit_begin_dateChanged(const QDate &date)
{
    showHistory();
     //更新ListWidget
	 //查询打卡信息记录表
	 //将所有的信息addItem更新到ListWidget上
}
