#include "infowidget.h"
#include "ui_infowidget.h"
#include "general.h"
#include "signals.h"
#include <QDebug>
infowidget::infowidget(QWidget *parent)
    : QWidget{parent},
    ui(new Ui::infowidget)
{
    ui->setupUi(this);


    // 创建 General 界面
    generalWidget = new General();
    ui->stackedWidget->addWidget(generalWidget);

    // 创建 Signals 界面
   signalsWidget = new Signals();
    ui->stackedWidget->addWidget(signalsWidget);
    qDebug() << "StackedWidget count: " << ui->stackedWidget->count();

    ui->stackedWidget->setCurrentIndex(2);

    // 连接按钮点击事件
    connect(ui->generalButton, &QPushButton::clicked, this, &infowidget::showGeneral);
    connect(ui->singalButton, &QPushButton::clicked, this, &infowidget::showSignals);
}
infowidget::~infowidget()
{
    delete ui;
}

void infowidget::initHdr(const edf_hdr_struct *hdr, const QString nowFile)
{
    ui->textEdit->setText(nowFile);
    generalWidget->initHdr(hdr);

}



void infowidget::showGeneral()
{
    // 切换到 General 界面
    ui->stackedWidget->setCurrentIndex(2);

}

void infowidget::showSignals()
{
    // 切换到 Signals 界面
    ui->stackedWidget->setCurrentIndex(3);
}
