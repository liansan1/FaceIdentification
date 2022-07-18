#include "clockdao.h"

ClockDao::ClockDao()
{
    QSqlQuery query;
    QString sql = QString("create table IF NOT EXISTS History("
                          "id integer primary key autoincrement,"
                          "name varchar(20),"
                          "time datetime);");
    bool ok = query.exec(sql);
    if(ok == false){
        qDebug() << "History创建失败";
    }
}

ClockDao::~ClockDao()
{

}

bool ClockDao::insertClock(ClockEntity e)
{
    QSqlQuery query;
    query.prepare("insert into History(name,time) values(?,datetime('now','localtime'))");
    //datetime('now','localtime') 系统时间
    query.bindValue(0,e.name);

    return query.exec();

}

bool ClockDao::selectClock(QDateTime start, QDateTime end, vector<ClockEntity> &v)
{
    QSqlQuery query;
    query.prepare("select * from History where time between ? and ?");
    query.bindValue(0,QString("%1 00:00:00").arg(start.toString("yyyy-MM-dd")));
    query.bindValue(1,QString("%1 23:59:59").arg(end.toString("yyyy-MM-dd")));


    if(query.exec()==false){
        qDebug() << "查询数据失败";
        return false;
    }

    while(query.next()){
       ClockEntity e(query.value("name").toString(),
                     query.value("time").toDateTime(),
                     query.value("id").toInt());
       v.push_back(e);
    }
    return true;
}

