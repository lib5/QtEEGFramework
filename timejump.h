#ifndef TIMEJUMP_H
#define TIMEJUMP_H

#include <QWidget>
#include <QDateTime>
#include <QDate>
#include <QTime>
namespace Ui {
class timeJump;
}

class timeJump : public QWidget
{
    Q_OBJECT
public:
    explicit timeJump(QWidget *parent = nullptr);
     ~timeJump();

signals:
    void confirmTime(QDateTime dateTime);

private slots:
    //槽函数，初始化默认时间为edf曲线图中心点时间。
    void initTime(QDateTime dateTime, QDateTime endTime);
    //点击确定键后，将选择的时间返回主窗口
    void on_pushButton_clicked();

private:
    Ui::timeJump *ui;
};

#endif // TIMEJUMP_H
