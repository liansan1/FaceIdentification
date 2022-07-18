#ifndef USERDAO_H
#define USERDAO_H
#include "userentity.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
//负责管理操作struct UserEntity

class UserDao
{
public:
    UserDao();
    ~UserDao();

    bool insertUser(UserEntity e);
    bool selectUser(int id, UserEntity& e);
    //根据ID值 查询Person信息  UserEntity& e是传出参数
    int getId();
    //获得插入的最后一条数据的id，
    //作为这个人的label 放入学习模型中
};

#endif // USERDAO_H
