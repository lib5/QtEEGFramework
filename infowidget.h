#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include "edflib.h"
#include"general.h"
#include "signals.h"

namespace Ui {
class infowidget;
}


class infowidget : public QWidget
{
    Q_OBJECT
public:
    explicit infowidget(QWidget *parent = nullptr);
    ~infowidget();




    void initHdr(const edf_hdr_struct *hdr,const QString nowFile);
    //点击确定键后，将选择的时间返回主窗口

signals:
private:
    General *generalWidget;
    Signals *signalsWidget;
private slots:
    void showGeneral();  // 显示 General 界面
    void showSignals();  // 显示 Signals 界面

private:
    Ui::infowidget *ui;
};

#endif // INFOWIDGET_H
