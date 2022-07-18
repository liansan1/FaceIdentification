#include "daofactory.h"
//静态指针类外初始化
DaoFactory * DaoFactory::Instance = NULL;

DaoFactory::DaoFactory()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("my.db");
    if(database.open()==false){
        qDebug() << "数据库打开失败";
    }
    //创建UserDao
    pUser = new UserDao;
    pClock = new ClockDao;
}

DaoFactory::~DaoFactory()
{
    database.close();
}

DaoFactory *DaoFactory::getInstance()
{
    if(NULL == Instance){
        Instance = new DaoFactory;
    }
    return Instance;
}

void DaoFactory::ReleaseInstance()
{
    if(Instance != NULL){
        delete Instance;
        Instance = NULL;
    }
}
