#ifndef USERENTITY_H
#define USERENTITY_H
#include <QString>

struct UserEntity
{
    UserEntity(){}
    UserEntity(QString name,int id = 0)
        :name(name),id(id)
    {}
    ~UserEntity();

    int id;//对应人脸识别时候的label User表中的主键
    QString name;
};

#endif // USERENTITY_H
