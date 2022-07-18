#ifndef CLOCKDAO_H
#define CLOCKDAO_H
#include "clockentity.h"
#include <vector>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>
using namespace std;

class ClockDao
{
public:
    ClockDao();
    ~ClockDao();
    bool insertClock(ClockEntity e);
    bool selectClock(QDateTime start, QDateTime end, vector<ClockEntity>& v);
};

#endif // CLOCKDAO_H
