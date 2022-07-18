#include "userdao.h"

UserDao::UserDao()
{
    QSqlQuery query;
    QString sql = QString("create table IF NOT EXISTS User("
                          "id integer primary key autoincrement,"
                          "name varchar(20));");
    bool ok = query.exec(sql);
    if(ok == false){
        qDebug() << "User创建失败";
    }
}

UserDao::~UserDao()
{

}

bool UserDao::insertUser(UserEntity e)
{
    QSqlQuery query;
    query.prepare("insert into User(name) values(?)");
    query.bindValue(0,e.name);

    return query.exec();
}

bool UserDao::selectUser(int id, UserEntity &e)
{
    QSqlQuery query;
    query.prepare("select * from User where id = ?");
    query.bindValue(0,id);

    if(query.exec()==false){
        qDebug() << "查询数据失败";
        return false;
    }

    while(query.next()){
        e.id  = id;
        e.name = query.record().value("name").toString();
    }
    return true;
}

int UserDao::getId()
{
    QSqlQuery query;
    query.prepare("SELECT LAST_INSERT_ROWID()");
    bool ok = query.exec();
    if(ok == false){
        qDebug() << "getID error";
    }
    int id;
    while(query.next()){
        id = query.record().value(0).toInt();
    }
    return id;
}



