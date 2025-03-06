#include "timejump.h"
#include "ui_timejump.h"
#include <QDateTime>
#include <QDate>
#include <QTime>


//时间跳转功能窗口
timeJump::timeJump(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::timeJump)
{
    ui->setupUi(this);
}

timeJump::~timeJump()
{
    delete ui;
}

void timeJump::initTime(QDateTime dateTime, QDateTime endTime)
{
    ui->year->setRange(0,9999);
    ui->month->setRange(1,12);
    ui->day->setRange(1,31);
    ui->hour->setRange(1,24);
    ui->minute->setRange(1,60);
    ui->second->setRange(1,60);
    ui->microSecond->setRange(1,1000);
    ui->year->setValue(dateTime.date().year());
    ui->month->setValue(dateTime.date().month());
    ui->day->setValue(dateTime.date().day());
    ui->hour->setValue(dateTime.time().hour());
    ui->minute->setValue(dateTime.time().minute());
    ui->second->setValue(dateTime.time().second());
    ui->microSecond->setValue(dateTime.time().msec());
    ui->endTime->setText(endTime.toString("yyyy-MM-dd hh:mm:ss"));
}
//槽函数，初始化默认时间为edf曲线图中心点时间。


//点击确定键后，将选择的时间返回主窗口
void timeJump::on_pushButton_clicked()
{
    emit confirmTime(QDateTime(QDate(ui->year->value(),ui->month->value(),ui->day->value()),
                               QTime(ui->hour->value(),ui->minute->value(),ui->second->value(),ui->microSecond->value())));
    this->close();
}
