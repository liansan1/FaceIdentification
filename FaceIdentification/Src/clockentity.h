#ifndef CLOCKENTITY_H
#define CLOCKENTITY_H
#include <QString>
#include <QDateTime>

struct ClockEntity
{

    ClockEntity();
    ~ClockEntity();

    ClockEntity(QString name, QDateTime time = QDateTime(),
                int id = 0):name(name),time(time),id(id){}

    int id;
    QString name;
    QDateTime time;
};

#endif // CLOCKENTITY_H
