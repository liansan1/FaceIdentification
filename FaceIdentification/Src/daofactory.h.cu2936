#ifndef DAOFACTORY_H
#define DAOFACTORY_H
//单例模式
#include <QSqlDatabase>
#include "userdao.h"
#include "clockdao.h"

class DaoFactory
{
public:
    static DaoFactory* getInstance();
    static void ReleaseInstance();
    UserDao* getUserDao(){
        return pUser;
    }
    ClockDao* getClockDao(){
        return pClock;
    }
    ~DaoFactory();
private:
    DaoFactory();
    DaoFactory(const DaoFactory&){}
    static DaoFactory* Instance;

    UserDao *pUser;
    ClockDao *pClock;
    QSqlDatabase database;
};

#endif // DAOFACTORY_H
