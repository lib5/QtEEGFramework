#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtConcurrent>

// 定义一个结构体来存储每个信号的处理结果


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    init();//初始化绑定信号与槽
    setFocusPolicy(Qt::StrongFocus);

    //拦截器
    ui->graph->installEventFilter(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initNameList()
{
    QVector<QPair<int,QString> > namelist;
//    viewConfig config;
    for(int i=0;i<hdr->edfsignals;++i){
        namelist.append(QPair<int,QString>(i,hdr->signalparam[i].label));
        qDebug()<<i<<"   "<<hdr->signalparam[i].label;
//        config.add(i,hdr->signalparam[i].label,true);

    }
    nameList=namelist;
//    viewconfig=config;

}
//添加最近文件
void MainWindow::addRecentFile(const QString &filePath)
{
    QSettings settings("YourCompany", "YourApp"); // QSettings 存储最近文件
    recentFiles = settings.value("recentFiles").toStringList();

    recentFiles.removeAll(filePath); // 避免重复
    recentFiles.prepend(filePath);   // 插入到最前面

    while (recentFiles.size() > maxRecentFiles) {
        recentFiles.removeLast();
    }

    settings.setValue("recentFiles", recentFiles); // 保存到 QSettings

    updateRecentFilesMenu(); // 更新菜单
}


void MainWindow::updateRecentFilesMenu()
{
    recentMenu->clear(); // 清空菜单

    QSettings settings("YourCompany", "YourApp"); // 使用 QSettings 存储
    recentFiles = settings.value("recentFiles").toStringList(); // 读取存储的文件列表

    // 确保最多只存储 maxRecentFiles 个文件
    while (recentFiles.size() > maxRecentFiles) {
        recentFiles.removeLast();
    }

    for (const QString &filePath : recentFiles) {
        QAction *action = new QAction(filePath, this);
        connect(action, &QAction::triggered, this, [=]() {
            on_actionClosefile_triggered();
            plotLine(filePath); // 调用你的打开文件函数
        });
        recentMenu->addAction(action);
    }
}

void MainWindow::addMenu()
{

    recentMenu = ui->menuFile->addMenu(tr("最近打开文件")); // 添加到“文件”菜单
    updateRecentFilesMenu(); // 更新菜单


}

QString MainWindow::openEdf()
{
    QString curPath=QDir::currentPath();
    QString dlgTitle="选择一个文件";
    QString filter="脑电波文件(*.edf *.bdf)";
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    return aFileName;
}


// 定义加载剩余数据的函数
void loadRemainingData(edf_hdr_struct* hdr, int initialRecords, std::vector<std::vector<int>>& allData) {
    for (int i = 0; i < hdr->edfsignals; i++) {
        edf_param_struct signalParam = hdr->signalparam[i];
        size_t samples_per_record = signalParam.smp_in_datarecord;

        // 加载剩余记录
        for (int recordIdx = initialRecords; recordIdx < hdr->datarecords_in_file; recordIdx++) {
            size_t offset = recordIdx * samples_per_record;
            edfseek(hdr->handle, i, offset, EDFSEEK_SET);
            edfread_digital_samples(hdr->handle, i, samples_per_record, allData[i].data() + offset);
        }
    }
}


//void MainWindow::plotLine(QString edfPath) loadRemainingData
//{
//    nowFile=edfPath;
//    edfclose_file(hdr->handle);
//    //1.校验edfPath是否为edf路径
//    char path[1024]={0};
//    memcpy(path,edfPath.toUtf8().data(), strlen(edfPath.toUtf8().data()));
//    //2.读取文件数据
//    int n=edfopen_file_readonly(path, hdr, EDFLIB_READ_ALL_ANNOTATIONS);
//    //3.读取判断
//    if(n!=0){
//        QMessageBox::information(this,"错误",edfPath+"读取失败！无法绘制波形图",QMessageBox::Ok,QMessageBox::NoButton);
//        return ;
//    }
//    //初始化相关edf数据
//    int signalCount = hdr->edfsignals;
//    initNameList();
//    //edf开始时间
//    *startTime=QDateTime(QDate(hdr->startdate_year,hdr->startdate_month,hdr->startdate_day),
//                         QTime(hdr->starttime_hour,hdr->starttime_minute,hdr->starttime_second,hdr->starttime_subsecond));

//    //nowTime=startTime;
//    nowTime=new QDateTime(startTime->addMSecs(0));
//    duration=hdr->datarecord_duration*1e-7;
//    //采样率
//    SamplingRate=hdr->signalparam[0].smp_in_datarecord / duration;
//    //划分界面并加入图像
//    avg=0;
//    for (int i = 0; i < signalCount; i++) {
//        edf_param_struct signalParam = hdr->signalparam[i];
//        if(i==0)
//        total_samples = signalParam.smp_in_datarecord * hdr->datarecords_in_file;
//        int maxn=signalParam.dig_max;
//        int minn=signalParam.dig_min;
//        avg=avg+maxn-minn;
//    }
//    ui->graph->clearGraphs();
//    avg=avg/(6*signalCount);

//    //ui->text->append(QString::number(hdr->file_duration));
//    for(int i=0;i<hdr->edfsignals;i++){
//        ui->graph->addGraph();
//        ui->graph->graph(i)->setName(hdr->signalparam[i].label);
//    }
//    ui->graph->addGraph();
//    ui->graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//    ui->graph->axisRect()->setRangeDrag(Qt::Horizontal);
//    ui->graph->axisRect()->setRangeZoom(Qt::Horizontal);



//    //初始化
//    ui->graph->xAxis->setRange(0,hdr->signalparam[0].smp_in_datarecord);
//    topAxis->setRange(0,hdr->signalparam[0].smp_in_datarecord);
//    qDebug()<<hdr->datarecords_in_file*hdr->signalparam[0].smp_in_datarecord;
//    //拖动条范围

//    ui->hScrollBar->setRange(0,total_samples);

//    //初始化x,y 大小为一秒的采样数字；刻度，标签大小为信号数量
//    QVector<double> x(total_samples),y(total_samples);
//    ticks.resize(hdr->edfsignals);

//    QVector<QString> labels(hdr->edfsignals);
//    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);

//    duration=hdr->datarecords_in_file*(hdr->datarecord_duration/10000000);
//    //ui->text->append(QString::number(hdr->signalparam[0].smp_in_file));
//    qDebug()<<"记录的样点数"<<hdr->signalparam[0].smp_in_file;
//    //ui->text->append(QString::number(hdr->datarecords_in_file));

//    //设置曲线颜色
//    QVector<QColor> colors = { QColor(255, 0, 0),      // 亮红
//                              QColor(0, 150, 255),    // 亮蓝
//                              QColor(0, 200, 0),      // 亮绿
//                              QColor(255, 0, 255),    // 亮紫
//                              QColor(0, 255, 255),    // 亮青
//                              QColor(255, 165, 0),    // 亮橙
//                              QColor(255, 255, 0),    // 亮黄
//                              QColor(160, 32, 240),   // 亮紫（不同）
//                              QColor(255, 105, 180),  // 亮粉色
//                              QColor(30, 144, 255) }; // 亮深蓝

//    qDebug()<<"总记录数"<<total_samples;
//    ui->graph->yAxis->setVisible(true);

//    maxTimeGap =(qint64)(total_samples/SamplingRate*1000);
//    //edf结束时间
//    endTime=new QDateTime(startTime->addMSecs(maxTimeGap));




//    // 假设有 23 个极点
//    const int numPoles = hdr->edfsignals;

//    // 为每个极点分配一个 allData
//    std::vector<std::vector<int>> allData(numPoles);

//    // 定义每次加载的记录数

//    const int totalRecords = hdr->datarecords_in_file;

//    // 预分配内存
//    for (int i = 0; i < hdr->edfsignals; i++) {
//        size_t total_samples = hdr->signalparam[i].smp_in_datarecord * totalRecords;
//        allData[i].resize(total_samples);
//        x.resize(total_samples);
//        y.resize(total_samples);
//    }

//    // 初始加载部分记录
//    for (int i = 0; i < hdr->edfsignals; i++) {
//        edf_param_struct signalParam = hdr->signalparam[i];
//        size_t samples_per_record = signalParam.smp_in_datarecord;

//        // 加载初始记录
//        for (int recordIdx = 0; recordIdx < initialRecords; recordIdx++) {
//        size_t offset = recordIdx * samples_per_record;
//        edfseek(hdr->handle, i, offset, EDFSEEK_SET);
//        edfread_digital_samples(hdr->handle, i, samples_per_record, allData[i].data() + offset);
//        }

//        int temp = i * avg / 4;

//        // 准备绘图数据
//        for (int j = 0; j < initialRecords * samples_per_record; j++) {
//        x[j] = j;
//        y[j] = allData[i][j] - temp;
//        }

//        // 设置曲线颜色
//        QPen pen;
//        pen.setWidth(1);
//        pen.setColor(colors[i % colors.size()]);
//        ui->graph->graph(i)->setPen(pen);

//        // 设置曲线数据
//        ui->graph->graph(i)->setData(x, y, true);

//        // 设置刻度标签
//        ticks[i] = -temp;
//        labels[i] = signalParam.label;
//        textTicker->addTick(ticks[i], labels[i]);

//        // 添加文本标签
//        QCPItemText *labelItem = new QCPItemText(ui->graph);
//        labelItem->setFont(QFont("Arial", 8));
//        labelItem->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter);
//        labelItem->position->setCoords(topAxis->range().lower, ticks[i] + 25);
//        labelItem->setText(labels[i]);
//        labelItem->setColor(colors[i % colors.size()]);
//        labelItems.append(labelItem);
//    }

//    // 初始设置 x 轴范围
//    ui->graph->xAxis->setRange(0, hdr->signalparam[0].smp_in_datarecord*recordcnt);
//    topAxis->setRange(0, hdr->signalparam[0].smp_in_datarecord*recordcnt  / SamplingRate);

//    // 设置 y 轴范围
//    double minTick = *std::min_element(ticks.begin(), ticks.end());
//    double maxTick = *std::max_element(ticks.begin(), ticks.end());
//    ui->graph->yAxis->setRange(maxTick + 50, minTick - 50); // 反转 y 轴方向
//    timeLabel->setText(nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));
//    // 重绘图
//    ui->graph->yAxis->grid()->setPen(QPen(QColor(150, 150, 150), 1.2, Qt::SolidLine));
//    ui->graph->yAxis->setTicker(textTicker);
//    ui->graph->yAxis->setTicks(true);

//    ui->graph->replot();

//    // 加载剩余数据
//    for (int i = 0; i < hdr->edfsignals; i++) {
//        edf_param_struct signalParam = hdr->signalparam[i];
//        size_t samples_per_record = signalParam.smp_in_datarecord;

//        // 加载剩余记录
//        for (int recordIdx = initialRecords; recordIdx < totalRecords; recordIdx++) {
//        size_t offset = recordIdx * samples_per_record;
//        edfseek(hdr->handle, i, offset, EDFSEEK_SET);
//        edfread_digital_samples(hdr->handle, i, samples_per_record, allData[i].data() + offset);
//        }

//        int temp = i * avg / 4;

//        // 准备绘图数据
//        for (int j = 0; j < totalRecords * samples_per_record; j++) {
//        x[j] = j;
//        y[j] = allData[i][j] - temp;
//        }

//        // 更新曲线数据
//        ui->graph->graph(i)->setData(x, y, true);
//    }

//    timeLabel->setText( nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));
//    // 启用水平和垂直拖拽
//    ui->graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

//    // 允许垂直方向的拖拽
//    ui->graph->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);

//    // 重绘图
//    ui->graph->replot();


//}



void MainWindow::plotLine(QString edfPath)
{   //初始化标签
    ui->graph->clearItems();
    timeLabel = new QCPItemText(ui->graph); // 重新创建 timeLabel
    timeLabel->setPositionAlignment(Qt::AlignBottom | Qt::AlignRight); // 设置对齐方式为右下角
    timeLabel->position->setType(QCPItemPosition::ptAxisRectRatio);    // 使用轴矩形比例
    timeLabel->position->setCoords(1, 1);                              // 设置位置为右下角
    timeLabel->setText("Initial Text");                                // 设置文本内容
    timeLabel->setFont(QFont(font().family(), 9));                    // 设置字体大小为 12
    timeLabel->setColor(Qt::white);                                    // 设置字体颜色为白色
    timeLabel->setPadding(QMargins(4, 4, 4, 4));                       // 设置内边距
    timeLabel->setBrush(QBrush(QColor(50, 50, 50))); // 设置背景颜色为白色
    nowFile = edfPath;
    edfclose_file(hdr->handle);

    char path[1024] = {0};
    memcpy(path, edfPath.toUtf8().data(), strlen(edfPath.toUtf8().data()));

    int n = edfopen_file_readonly(path, hdr, EDFLIB_READ_ALL_ANNOTATIONS);
    if (n != 0) {
        QMessageBox::information(this, "错误", edfPath + "读取失败！无法绘制波形图", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    ui->hScrollBar->setValue(0);
    int signalCount = hdr->edfsignals;
    initNameList();

    *startTime = QDateTime(QDate(hdr->startdate_year, hdr->startdate_month, hdr->startdate_day),
                           QTime(hdr->starttime_hour, hdr->starttime_minute, hdr->starttime_second, hdr->starttime_subsecond));
    nowTime = new QDateTime(startTime->addMSecs(0));
    duration = hdr->datarecord_duration * 1e-7;
    SamplingRate = hdr->signalparam[0].smp_in_datarecord / duration;

    avg = 0;
    for (int i = 0; i < signalCount; i++) {
        edf_param_struct signalParam = hdr->signalparam[i];
        if (i == 0)
            total_samples = signalParam.smp_in_datarecord * hdr->datarecords_in_file;
        int maxn = signalParam.dig_max;
        int minn = signalParam.dig_min;
        avg += maxn - minn;
    }
    avg = avg / (6 * signalCount);

    ui->graph->clearGraphs();
    for (int i = 0; i < hdr->edfsignals; i++) {
        ui->graph->addGraph();
        ui->graph->graph(i)->setName(hdr->signalparam[i].label);
    }

    ui->graph->addGraph();
    ui->graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->graph->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->graph->axisRect()->setRangeZoom(Qt::Horizontal);

    ui->graph->xAxis->setRange(0, hdr->signalparam[0].smp_in_datarecord);
    topAxis->setRange(0, hdr->signalparam[0].smp_in_datarecord);
    ui->hScrollBar->setRange(0, total_samples);

    QVector<double> x(total_samples), y(total_samples);
    ticks.resize(hdr->edfsignals);
    QVector<QString> labels(hdr->edfsignals);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);

    duration = hdr->datarecords_in_file * (hdr->datarecord_duration / 10000000);

    QVector<QColor> colors = {
        QColor(255, 0, 0), QColor(0, 150, 255), QColor(0, 200, 0),
        QColor(255, 0, 255), QColor(0, 255, 255), QColor(255, 165, 0),
        QColor(255, 255, 0), QColor(160, 32, 240), QColor(255, 105, 180),
        QColor(30, 144, 255)};

    maxTimeGap = (qint64)(total_samples / SamplingRate * 1000);
    endTime = new QDateTime(startTime->addMSecs(maxTimeGap));

    const int numPoles = hdr->edfsignals;
    std::vector<std::vector<int>> allData(numPoles);
    const int totalRecords = hdr->datarecords_in_file;

    for (int i = 0; i < hdr->edfsignals; i++) {
        size_t total_samples = hdr->signalparam[i].smp_in_datarecord * totalRecords;
        allData[i].resize(total_samples);
        x.resize(total_samples);
        y.resize(total_samples);
    }

    // **1. 仅加载部分数据用于初次绘制**
    int initialRecords = std::min(5, totalRecords);  // 只加载前5个记录
    for (int i = 0; i < hdr->edfsignals; i++) {
        edf_param_struct signalParam = hdr->signalparam[i];
        size_t samples_per_record = signalParam.smp_in_datarecord;

        for (int recordIdx = 0; recordIdx < initialRecords; recordIdx++) {
            size_t offset = recordIdx * samples_per_record;
            edfseek(hdr->handle, i, offset, EDFSEEK_SET);
            edfread_digital_samples(hdr->handle, i, samples_per_record, allData[i].data() + offset);
        }

        int temp = i * avg / 4;
        for (int j = 0; j < initialRecords * samples_per_record; j++) {
            x[j] = j;
            y[j] = allData[i][j] - temp;
        }

        QPen pen;
        pen.setWidth(1);
        pen.setColor(colors[i % colors.size()]);
        ui->graph->graph(i)->setPen(pen);
        ui->graph->graph(i)->setData(x, y, true);

        ticks[i] = -temp;
        labels[i] = signalParam.label;
        textTicker->addTick(ticks[i], labels[i]);

        QCPItemText *labelItem = new QCPItemText(ui->graph);
        labelItem->setFont(QFont("Arial", 8));
        labelItem->setPositionAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        labelItem->position->setCoords(topAxis->range().lower, ticks[i] + 25);
        labelItem->setText(labels[i]);
        labelItem->setColor(colors[i % colors.size()]);
        labelItems.append(labelItem);
    }

    ui->graph->xAxis->setRange(0, hdr->signalparam[0].smp_in_datarecord * initialRecords);
    topAxis->setRange(0, hdr->signalparam[0].smp_in_datarecord * initialRecords / SamplingRate);
    // 设置 y 轴范围
    double minTick = *std::min_element(ticks.begin(), ticks.end());
    double maxTick = *std::max_element(ticks.begin(), ticks.end());
    ui->graph->yAxis->setRange(maxTick + 50, minTick - 50); // 反转 y 轴方向
    timeLabel->setText(nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));
    // 重绘图
    ui->graph->yAxis->grid()->setPen(QPen(QColor(150, 150, 150), 1.2, Qt::SolidLine));
    ui->graph->yAxis->setTicker(textTicker);
    ui->graph->yAxis->setTicks(true);
     ui->graph->yAxis->setVisible(true);
    ui->graph->setInteractions(QCP::iRangeDrag); // 仅允许拖动
    ui->graph->replot();

    // **2. 在后台线程中加载剩余数据**
    QtConcurrent::run([=]() {
        for (int i = 0; i < hdr->edfsignals; i++) {
            edf_param_struct signalParam = hdr->signalparam[i];
            size_t samples_per_record = signalParam.smp_in_datarecord;

            for (int recordIdx = initialRecords; recordIdx < totalRecords; recordIdx++) {
                size_t offset = recordIdx * samples_per_record;
                edfseek(hdr->handle, i, offset, EDFSEEK_SET);
                edfread_digital_samples(hdr->handle, i, samples_per_record,
                                        const_cast<int*>(allData[i].data() + offset));


            }

            int temp = i * avg / 4;
            QVector<double> x_local(totalRecords * samples_per_record), y_local(totalRecords * samples_per_record);
            for (int j = 0; j < totalRecords * samples_per_record; j++) {
                x_local[j] = j;
                y_local[j] = allData[i][j] - temp;
            }

            QMetaObject::invokeMethod(this, [=]() {
                ui->graph->graph(i)->setData(x_local, y_local, true);
                ui->graph->replot();
            }, Qt::QueuedConnection);
        }
    });
}


void MainWindow::getAvg()
{
}

void MainWindow::timeLabelChange(double xMin)
{
    // 计算滚动偏移量对应的时间
    double timePerPoint = 1.0 / SamplingRate; // 每个点对应的时间（秒）
    double timeOffset = (xMin - ui->graph->xAxis->range().lower) * timePerPoint; // 时间偏移量（秒）

    // 更新 startTime
    *startTime = startTime->addMSecs(static_cast<int>(timeOffset * 1000)); // 将秒转换为毫秒

    // 更新 timeLabel
    timeLabel->setText(startTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));

}

void MainWindow::init()
{
    //初始化时间标签与背景
    initTimeLabelAndBack();

    //添加状态栏已经菜单
    addStatus();

    addMenu();

    //edf存储的结构体
    hdr=new edf_hdr_struct;
    //计时器
    readTimer=new QTimer();
    dataTimer=new QTimer();
    //读取edf数组大小
    buffSize=1024*128;
    readBuff=(int*)malloc(buffSize*sizeof(int));


    //readBuff=(double*)malloc(buffSize*sizeof(double));

    //connect(ui->test,SIGNAL(clicked(bool)),this,SLOT(createEdfTest()));

    //滚动
     ui->hScrollBar->setPageStep(100); // 设置滚动条的步长
//     ui->graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // 启用拖拽和缩放
     ui->graph->setInteractions(QCP::iRangeDrag); // 只启用拖拽，禁用缩放
     connect(ui->hScrollBar, &QScrollBar::valueChanged, this, &MainWindow::horzScrollBarChanged);

     //播放计时器事件
     connect(dataTimer, &QTimer::timeout, this, &MainWindow::timeToPlot);


/*
    //

    //读取32位数据事件
    connect(readTimer, SIGNAL(timeout()), this, SLOT(readData32()));
    //文件栏点击事件
    connect(ui->fileTree,SIGNAL(itemPressed(QTreeWidgetItem*,int))
            ,this,SLOT(itemPressedSlot(QTreeWidgetItem*,int)));
    //文件点击事件
    connect(folderMenu,SIGNAL(triggered(QAction*)),this,
            SLOT(fileClickAction(QAction*)));
    //文件点击事件
    connect(fileMenu,SIGNAL(triggered(QAction*)),this,
            SLOT(fileClickAction(QAction*)));
    //文件栏双击事件
    connect(ui->fileTree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(DoubleClickpath(QTreeWidgetItem*,int)));
    //滚动条移动事件，重绘曲线图
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    //曲线图x轴范围改变事件
    connect(ui->graph->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    //y轴放缩倍率改变事件，重新绘制曲线图
    connect(fixSlider,SIGNAL(valueChanged(int)),this,SLOT(replotLine2(int)));
    //点击test进行openbci测试
    connect(ui->test,SIGNAL(clicked(bool)),this,SLOT(openBCIWifiTest()));
    connect(ui->graph,SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)),this,SLOT(annotation(QCPAbstractPlottable*,int,QMouseEvent*)));
    connect(ui->actConnect, &QAction::triggered, this, [this](){
        eegServer* mysocket = new eegServer();
        mysocket->show();
        eegPlot* plot = new eegPlot();
        plot->show();
        connect(mysocket, &eegServer::plotData, plot, &eegPlot::plot);
    });

*/

//     //曲线图x轴范围改变事件
//     connect(ui->graph->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
//     //y轴放缩倍率改变事件，重新绘制曲线图
//     connect(fixSlider,SIGNAL(valueChanged(int)),this,SLOT(replotLine2(int)));
}

void MainWindow::initTimeLabelAndBack()
{
    ui->hScrollBar->setValue(0);
    //设置时间
    startTime = new QDateTime();
     nowTime = new QDateTime(QDateTime::currentDateTime());

    // 初始化 graph 下标日期
    timeLabel = new QCPItemText(ui->graph);

    // 设置文本对齐方式（水平和垂直都居中）
    timeLabel->setPositionAlignment(Qt::AlignCenter);

    // 设置文本位置（基于轴矩形比例）
    timeLabel->position->setType(QCPItemPosition::ptAxisRectRatio);

    // 设置文本坐标（x: 0.5 表示水平居中，y: 0.5 表示垂直居中）
    timeLabel->position->setCoords(0.5, 0.5);

    // 设置文本内容
    timeLabel->setText("NO FILE OPEN");

    // 设置字体大小
    timeLabel->setFont(QFont(font().family(), 7)); // 字体大小为 7

    // 设置字体颜色为白色
    timeLabel->setColor(Qt::white);

    // 设置文本边距
    timeLabel->setPadding(QMargins(4, 4, 4, 4));


    // 设置背景颜色为更浅的灰色
    ui->graph->setBackground(QBrush(QColor(50, 50, 50))); // 浅灰色

    // 隐藏默认的 x 轴和 y 轴
    ui->graph->xAxis->setVisible(false);
    ui->graph->yAxis->setVisible(false);

    // 创建一个新的 x 轴（顶部 x 轴）
    topAxis = ui->graph->axisRect()->addAxis(QCPAxis::atTop);
    topAxis->setVisible(true);

    // 设置顶部 x 轴的颜色为亮白色
    topAxis->setBasePen(QPen(Qt::white));

    // 设置顶部 x 轴文本颜色为浅灰色
    topAxis->setTickLabelColor(QColor(200, 200, 200)); // 浅灰色

    // 设置顶部 x 轴刻度线颜色为亮白色
    topAxis->setTickPen(QPen(Qt::white));

    // 设置顶部 x 轴副刻度线颜色为亮白色
    topAxis->setSubTickPen(QPen(Qt::white));

    // 设置顶部 x 轴网格线（垂直网格线）
    topAxis->grid()->setVisible(true); // 启用 x 轴网格线
    topAxis->grid()->setPen(QPen(QColor(150, 150, 150), 1, Qt::SolidLine)); // 浅灰色实线

    // 设置顶部 x 轴刻度线和副刻度线
    topAxis->setTickLength(10); // 主刻度线长度
    topAxis->setSubTickLength(5); // 副刻度线长度
    topAxis->ticker()->setTickCount(11); // 设置主刻度线数量（0 到 10 共 11 个刻度）
    topAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability); // 设置刻度策略



    // 设置顶部 x 轴刻度数值显示在下部
    topAxis->setTickLabelSide(QCPAxis:: lsInside);

    // 设置坐标轴范围
    topAxis->setRange(0, 10);
    ui->graph->yAxis->setRange(0, 10);

    // 调整边距，使 x 轴与窗口上边缘重合，y 轴与窗口左边缘重合
    ui->graph->axisRect()->setAutoMargins(QCP::msNone); // 禁用自动边距

    ui->graph->axisRect()->setMargins(QMargins(0, 0, 0, 0)); // 设置边距为 0
    ui->graph->setInteractions(QCP::iRangeDrag); // 仅允许拖动

    ui->graph->replot();

    // 确保第一个垂直网格线与窗口左边缘重合

}

void MainWindow::addStatus()
{
//    ui->statusbar->addWidget(ui->actTimeJump);
    setStatusBar(nullptr);

    // 创建一个工具栏
    QToolBar *toolBar = new QToolBar("Bottom ToolBar", this);
    addToolBar(Qt::BottomToolBarArea, toolBar);
    // 设置样式表，取消上边框
    // 设置样式表，将边框颜色设为浅黑色
    toolBar->setStyleSheet("QToolBar { border: 1px solid #C0C0C0; }");

    // 添加左侧占位部件
    QWidget *leftSpacer = new QWidget();
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(leftSpacer);

    toolBar->addAction(ui->actTimeJump);
    toolBar->addAction(ui->actiontest);
    toolBar->addAction(ui->action);
    toolBar->addAction(ui->actStepback);
    toolBar->addAction(ui->actStart);
    toolBar->addAction(ui->actStop);
    toolBar->addAction(ui->actStepforward);
    toolBar->addAction(ui->prtsc);


    // 添加右侧占位部件
    QWidget *rightSpacer = new QWidget();
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(rightSpacer);

}


//重新绘制，与y轴缩放适配
void MainWindow::replotLine(){
    //ui->horizontalScrollBar->setPageStep(ui->graph->xAxis->range().size()/4);

    ui->hScrollBar->setSingleStep(ui->graph->xAxis->range().size()/16);
    int leftPos=ui->graph->xAxis->range().center()-ui->graph->xAxis->range().size()/2;
    if(leftPos<0)leftPos=0;
    int rightPos=ui->graph->xAxis->range().center()+ui->graph->xAxis->range().size()/2;
    if(rightPos<=0){
        qDebug()<<rightPos;
        return;
    }
    int lenth=rightPos-leftPos+1;
    if(lenth>buffSize){
        //TODO 提示太密集
        return;
    }
    for(int i=0;i<hdr->edfsignals;i++){
        edfseek(hdr->handle,i,leftPos,EDFSEEK_SET);
        QVector<double> x(lenth),y(lenth);
        int flag=edfread_digital_samples(hdr->handle, i, lenth,readBuff);
        //int flag=edfread_physical_samples(hdr->handle, i, lenth,readBuff);
        double temp=i*avg/4;
        for (int j = 0;j < flag;j++) {
        x[j]=j+leftPos;
        y[j]=*(readBuff + j)*fix-temp;
        }
        //ui->graph->graph(i)->data().data()->clear();
        ui->graph->graph(i)->setData(x,y,true);
    }

    //更新时间Label
    double difftime=ui->graph->xAxis->range().center()*duration
                      /(hdr->datarecords_in_file*hdr->signalparam[0].smp_in_datarecord);
    *nowTime=startTime->addMSecs(int(difftime*1000));
    if (!timeLabel) {
        qDebug() << "timeLabel is null!";
        return;
    }
    timeLabel->setText(nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));
//    ui->text->append(nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));
//    ui->text->append(QString::number(ui->graph->xAxis->range().center()));
//    ui->text->append(QString::number(hdr->datarecords_in_file*hdr->signalparam[0].smp_in_datarecord));
//    ui->text->append(QString::number(duration));
    //QTextEdit
    // 启用水平（x 轴）和垂直（y 轴）拖拽
    ui->graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->graph->replot();

}







//打开并绘制图像
void MainWindow::on_actOpenFile_triggered()
{
    //1.获取edf文件路径
    QString edfPath=openEdf();

//    prependToRecentFiles(edfPath);
//    ui->text->append(edfPath);
    if(edfPath.isEmpty()){
        return;
    }
    addRecentFile(edfPath); // 添加到最近文件列表

    //2.绘制图像
    plotLine(edfPath);
}

//截图槽函数
void MainWindow::on_prtsc_triggered()
{
    //获取截图
    QPixmap screenshot;
    //QPixmap screenshot = QPixmap::grabWindow(QApplication::);
    WId winId=ui->graph->winId();
    QScreen *screen = QGuiApplication::primaryScreen();
    if(screen){
        screenshot =screen->grabWindow(winId,0,0,-1,-1);
    }
    //    QClipboard *clipboard=QGuiApplication::clipboard();
    //    clipboard->setPixmap(screenshot);

    QString strDir = QCoreApplication::applicationDirPath() + "\\ScreenShot";
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkdir(strDir);
    }

    //生成文件
    QString strFile = strDir + QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss") + ".png";

    QString fileName = QFileDialog::getSaveFileName(this,"保存图片",strFile,"PNG (*.png);;BMP (*.bmp);;JPEG (*.jpg *.jpeg)");
    if (!fileName.isNull())
    {
        screenshot.save(fileName);
    }
}

void MainWindow::horzScrollBarChanged(int value)
{
    // 获取当前 x 轴的范围
    double xRange = ui->graph->xAxis->range().size(); // x 轴的总范围

    // 计算新的 x 轴范围
    double xMin = value; // 根据滚动条的值设置 x 轴的最小值
    double xMax = xMin + xRange; // 保持 x 轴的范围不变

    // 更新 x 轴范围
    ui->graph->xAxis->setRange(xMin, xMax);
    topAxis->setRange(xMin / SamplingRate, xMax / SamplingRate);

    // 更新标签位置
    setLabelItemsPositions();

    // 计算时间偏移量
    double timePerPoint = 1000.0 / SamplingRate; // 每个采样点对应的时间（毫秒）
    qint64 timeOffset = static_cast<qint64>(xMin * timePerPoint); // 偏移量（毫秒）

    // 更新 nowTime（相对 startTime 的偏移）
    *nowTime = startTime->addMSecs(timeOffset);
    if(!hdr->handle)
    // 更新 timeLabel
    timeLabel->setText(nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));

    // 刷新视图
    ui->graph->replot();


}
//更新标签
void MainWindow::setLabelItemsPositions()
{
    if(!hdr->handle)
    timeLabel->setText(nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));
    if (labelItems.size() != ticks.size()) {
        qWarning() << "setLabelItemsPositions: labelItems and ticks sizes do not match!";
        return;
    }

    // 遍历每个标签并设置位置
    for (int i = 0; i < labelItems.size(); ++i) {
        QCPItemText* labelItem = labelItems[i];
        if (labelItem) {
            // 设置标签的位置
            labelItem->position->setCoords(ui->graph->xAxis->range().lower, ticks[i] + 25); // 设置坐标

        }
    }
}

void  MainWindow::wheelEvent(QWheelEvent *event) {
    // 获取滚轮滚动的角度
    QPoint numDegrees = event->angleDelta() / 8;

    // 计算滚动的步长
    double step = SamplingRate/2; // 每次滚动移动 10 个单位（可以根据需要调整）

    // 获取当前 x 轴范围
    double xMin = ui->graph->xAxis->range().lower;
    double xMax = ui->graph->xAxis->range().upper;

    // 根据滚轮方向调整范围
    if (numDegrees.y() <0) {
        // 向上滚动，x 轴向左移动（显示更早的数据）
        xMin -= step;
        xMax -= step;
    } else if (numDegrees.y() > 0) {
        // 向下滚动，x 轴向右移动（显示更新的数据）
        xMin += step;
        xMax += step;
    }

    // 确保 x 轴范围不超出数据边界
    xMin = std::max(xMin, 0.0);
    xMax = std::min(xMax, static_cast<double>(total_samples));

    // 更新 x 轴范围
    ui->graph->xAxis->setRange(xMin, xMax);
    topAxis->setRange(xMin / SamplingRate, xMax / SamplingRate);

    // 更新 labels 位置
    setLabelItemsPositions();

    // 计算滚动偏移量对应的时间
    double timePerPoint = 1.0 / SamplingRate; // 每个点对应的时间（秒）
    double timeOffset = step * timePerPoint; // 滚动偏移量对应的时间（秒）

    // 根据滚轮方向更新 startTime
    if (numDegrees.y() > 0) {
        // 向上滚动，时间向过去移动
        *nowTime = nowTime->addMSecs(static_cast<int>(timeOffset * 1000)); // 减去时间偏移量
    } else if (numDegrees.y() < 0) {
        // 向下滚动，时间向未来移动
        * nowTime  = nowTime ->addMSecs(static_cast<int>(-timeOffset * 1000)); // 加上时间偏移量
    }

    if (nowTime->toMSecsSinceEpoch()  < startTime->toMSecsSinceEpoch() ) {
        *nowTime = *startTime; // 如果超出范围，则设为 startTime
    }
    if(!hdr->handle)
    timeLabel->setText( nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));

    // 刷新视图
    ui->graph->replot();


    // 接受事件
    event->accept();
}

//获取x轴中点时间
void MainWindow::getNowTime(){
    double difftime=ui->graph->xAxis->range().center()*duration
                      /(hdr->datarecords_in_file*hdr->signalparam[0].smp_in_datarecord);
    *nowTime=startTime->addMSecs(int(difftime*1000));
}
//打开时间跳转对话框，并且初始化时间跳转对话框
void MainWindow::on_actTimeJump_triggered()
{
    if (hdr->handle) { // 这里修正了 if 条件，应该是如果文件未打开才弹出警告
        QMessageBox::warning(this, "警告", "请先打开文件！");
        return;
    }
    timeJump *timeJumpWidget=new timeJump();
    connect(this,SIGNAL(initTimeJump(QDateTime,QDateTime)),timeJumpWidget,SLOT(initTime(QDateTime,QDateTime)));
    connect(timeJumpWidget,SIGNAL(confirmTime(QDateTime)),this,SLOT(changeTime(QDateTime)));
    timeJumpWidget->show();
    if (hdr->handle) { // 这里修正了 if 条件，应该是如果文件未打开才弹出警告
        QMessageBox::warning(this, "警告", "请先打开文件！");
        return;
    }
    getNowTime();
    emit initTimeJump(*nowTime,*endTime);
}

//时间跳转的槽函数，根据时间跳转对话框设置进行时间跳转
void MainWindow::changeTime(QDateTime jumpTime){


    int offset = jumpTime.toMSecsSinceEpoch() - startTime->toMSecsSinceEpoch();
    qDebug() << "偏移" << offset<< " "<< maxTimeGap;
        if (offset >=maxTimeGap&&offset>0) {
        QMessageBox::warning(this, "警告", "时间范围太大，最大时间为: " + endTime->toString("yyyy-MM-dd hh:mm:ss"));
        return;
    }

    int center = offset * SamplingRate / 1000; // 计算中心位置，以采样点为单位
    int halfRange = ui->graph->xAxis->range().size() / 2;
    int xMin = center - halfRange;
    int xMax = center + halfRange;

    // 更新 x 轴范围（以采样点为单位）
    ui->graph->xAxis->setRange(xMin, xMax);

    // 更新 topAxis 显示秒数（以秒为单位）
    double tMin = static_cast<double>(xMin) / SamplingRate;
    double tMax = static_cast<double>(xMax) / SamplingRate;
    topAxis->setRange(tMin, tMax);

    *nowTime = jumpTime.addMSecs(-offset);


    // 更新标签位置
    setLabelItemsPositions();

    // 刷新视图
    ui->graph->replot();
}

void MainWindow::on_actStart_triggered() {
    if (hdr->handle) { // 这里修正了 if 条件，应该是如果文件未打开才弹出警告
        QMessageBox::warning(this, "警告", "请先打开文件！");
        return;
    }
    static bool flag = true;
    if (flag) {
        anotherTime = ui->graph->xAxis->range().lower* 1.0 / hdr->signalparam[0].smp_in_datarecord;
        double freq = 60; // 播放频率为 60 Hz
        timeSpeed = 1.0 / freq; // 每次定时器触发时的时间增量
        flag = false;
    }
    qDebug()<<"播放时间"<<anotherTime;

//    if (dataTimer->isActive()) {
//        dataTimer->stop();
//    }
//    dataTimer->start(1000 / 60); // 启动定时器

    if (!dataTimer->isActive()) {
        dataTimer->start(1000 / 60); // 定时器间隔设置为 16.67 毫秒
    }
}

void MainWindow::on_actionClosefile_triggered()
{
    if (hdr && hdr->handle >= 0) {
        edfclose_file(hdr->handle);  // 关闭 EDF 文件
        hdr->handle = -1;  // 避免重复关闭
    }

    nowFile.clear();  // 清空当前文件路径
    total_samples = 0;
    SamplingRate = 0;

    // 清空图表数据
    ui->graph->clearGraphs();


    // 5. 删除时间标签 timeLabel 并重置指针
    if (timeLabel) {
        ui->graph->removeItem(timeLabel);

        timeLabel = nullptr;
    }

    ui->graph->axisRect()->removeAxis(topAxis);
    topAxis = nullptr; // 避免野指针

    // 初始化 UI
    initTimeLabelAndBack();


    // 释放所有 label 资源
    for (QCPItemText* labelItem : labelItems) {
        ui->graph->removeItem(labelItem);
    }
    labelItems.clear();



}

//播放的槽函数
void MainWindow::timeToPlot(){
    anotherTime += timeSpeed; // 更新播放时间
    qDebug()<<"播放时间"<<anotherTime<<"播放速度"<<timeSpeed;

    // 计算新的 x 轴范围
    double xMin = anotherTime * hdr->signalparam[0].smp_in_datarecord;
    double xMax = xMin + ui->graph->xAxis->range().size(); // 保持范围大小不变

    // 更新 x 轴范围
    ui->graph->xAxis->setRange(xMin, xMax);
    topAxis->setRange(xMin / SamplingRate, xMax / SamplingRate);

    qint64 timeOffset = static_cast<qint64>(anotherTime * 1000);

    // 更新 nowTime，确保不会超出范围
    *nowTime = startTime->addMSecs(timeOffset);

    if (*nowTime > *endTime) {
        *nowTime = *endTime; // 防止超出结束时间
        dataTimer->stop(); // 停止播放
    }
    // 更新标签位置
    setLabelItemsPositions();

    // 仅更新坐标轴，不进行完整重绘
    ui->graph->replot();
}


//暂停
void MainWindow::on_actStop_triggered()
{
    dataTimer->stop();
}

//加速
void MainWindow::on_actiontest_triggered()
{
    timeSpeed=timeSpeed*2;
}
//减速
void MainWindow::on_action_triggered()
{
    timeSpeed=timeSpeed/2;
}
//快进
void MainWindow::on_actStepforward_triggered()
{
    anotherTime=anotherTime+timeSpeed*10;
    //ui->text->append("stepforward nowtime:"+QString::number(anotherTime,'f'));
}
//退回
void MainWindow::on_actStepback_triggered()
{
    anotherTime=anotherTime-timeSpeed*10;
    //ui->text->append("stepback nowtime:"+QString::number(anotherTime,'f'));
}





bool MainWindow::MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    {
        if (obj == ui->graph && event->type() == QEvent::Wheel) {
            wheelEvent(static_cast<QWheelEvent*>(event));
            return true;  // 事件已处理，不再传递给 QCustomPlot
        }
        return QMainWindow::eventFilter(obj, event);
    }
}





/*






//重绘函数，与时间跳转相适配
void MainWindow::plotLine3(){
    ui->horizontalScrollBar->setSingleStep(ui->graph->xAxis->range().size()/16);
    int leftPos=ui->graph->xAxis->range().center()-ui->graph->xAxis->range().size()/2;
    if(leftPos<0)leftPos=0;
    int rightPos=ui->graph->xAxis->range().center()+ui->graph->xAxis->range().size()/2;
    if(rightPos<=0){
        qDebug()<<rightPos;
        return;
    }
    int lenth=rightPos-leftPos+1;
    if(lenth>buffSize){
        //TODO 提示太密集
        return;
    }
    for(int i=0;i<hdr->edfsignals;i++){
        edfseek(hdr->handle,i,leftPos,EDFSEEK_SET);
        QVector<double> x(lenth),y(lenth);
        int flag=edfread_digital_samples(hdr->handle, i, lenth,readBuff);
        //int flag=edfread_physical_samples(hdr->handle, i, lenth,readBuff);
        double temp=i*avg/4;
        for (int j = 0;j < flag;j++) {
            x[j]=j+leftPos;
            y[j]=*(readBuff + j)*fix-temp;
        }
        //ui->graph->graph(i)->data().data()->clear();
        ui->graph->graph(i)->setData(x,y,true);
    }

    //更新时间Label
    getNowTime();
    if (!timeLabel) {
        qDebug() << "timeLabel is null!";
        return;
    }
    timeLabel->setText(nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));
    ui->text->append(nowTime->toString("yyyy-MM-dd hh:mm:ss:zzz"));
    ui->text->append(QString::number(ui->graph->xAxis->range().center()));
    ui->text->append(QString::number(hdr->datarecords_in_file*hdr->signalparam[0].smp_in_datarecord));
    ui->text->append(QString::number(duration));
    ui->graph->replot();
}

void MainWindow::on_graphViewSet_triggered()
{
    //graphViewSettings *gvs=new graphViewSettings(nameList);
    graphViewSettings *gvs=new graphViewSettings(viewconfig);
    gvs->show();
    connect(gvs,SIGNAL(viewConfigSave(viewConfig)),this,SLOT(changeConfig(viewConfig)));
}

*/
