#include "historyform.h"
#include "ui_historyform.h"

HistoryForm::HistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryForm)
{
    ui->setupUi(this);
    ui->dateEdit_start->setDate(QDate::currentDate());
    ui->dateEdit_end->setDate(QDate::currentDate());
}

HistoryForm::~HistoryForm()
{
    delete ui;
}

void HistoryForm::on_dateEdit_start_dateChanged(const QDate &date)
{
    //显示历史记录
    //清空ListWidget
    ui->listWidget->clear();
    vector<ClockEntity> v;
    DaoFactory::getInstance()->getClockDao()->selectClock(
                ui->dateEdit_start->dateTime(),
                ui->dateEdit_end->dateTime(),
                v);
    //v[0] --> 代表的是一个ClockEntity (id name time)
    for(int i=0;i<v.size();i++){
        //格式化字符串
        QString text = QString("姓名 %1 打卡时间 %2")
                .arg((v[i]).name)
                .arg((v[i]).time.toString());

        ui->listWidget->addItem(text);
    }

}


void HistoryForm::on_dateEdit_end_dateChanged(const QDate &date)
{
    //显示历史记录
    //清空ListWidget
    ui->listWidget->clear();
    vector<ClockEntity> v;
    DaoFactory::getInstance()->getClockDao()->selectClock(
                ui->dateEdit_start->dateTime(),
                ui->dateEdit_end->dateTime(),
                v);
    //v[0] --> 代表的是一个ClockEntity (id name time)
    for(int i=0;i<v.size();i++){
        //格式化字符串
        QString text = QString("姓名 %1 打卡时间 %2")
                .arg((v[i]).name)
                .arg((v[i]).time.toString());

        ui->listWidget->addItem(text);
    }
}
