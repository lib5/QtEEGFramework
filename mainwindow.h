#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "edflib.h"
#include "qcustomplot.h"
#include "timejump.h"
#include <QMainWindow>
#include "pluginmanager.h"
#include "infowidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
     void wheelEvent(QWheelEvent *event) override;
    // 在 MainWindow 中重写 eventFilter 函数
    bool eventFilter(QObject *obj, QEvent *event) override;

 signals:
     //时间跳转信号
     void initTimeJump(QDateTime dateTime,QDateTime endTime);
     void psdPlot(QVector<double> data,int frequency);
     void addTextSignal(double x, double y);

private:
    Ui::MainWindow *ui;
    //绘制图像
    QCustomPlot *customPlot;

    std::vector<double> annotationXaxis;
    std::vector<QCPItemText*> annotations;
    //edf点极名称列表
    QVector<QPair<int,QString> > nameList;

    QVector<QColor> colors = {
                              QColor(255, 0, 0), QColor(0, 150, 255), QColor(0, 200, 0),
                              QColor(255, 0, 255), QColor(0, 255, 255), QColor(255, 165, 0),
                              QColor(255, 255, 0), QColor(160, 32, 240), QColor(255, 105, 180),
                              QColor(30, 144, 255)};

    QString nowFile;
    QStringList recentFiles; // 存储最近打开的文件路径
    const int maxRecentFiles = 5; // 限制最多显示的文件数
    QMenu *recentMenu; // 最近打开的文件菜单
    void updateRecentFilesMenu(); // 更新菜单
    void addRecentFile(const QString &filePath);//添加文件名

    int nowcnt=0;
    //设置左下角时间
    void setTime(QDateTime time);

     QVector<QVector<double>> table;
    std::vector<std::vector<int>> allData;
    PluginManager *pluginManager;
    QMap<QString, QAction*> pluginActions;
    QString pluginPath;

    //读取edf文件数据总样点数
    int total_samples ;
    //一条信号记录持续时间s
    int duration;
    //所显示的记录数
    const int  recordcnt=5;
    const int initialRecords = 5; // 初始加载 5 个记录
    //采样率
    int SamplingRate;
    //edf开始时间
    QDateTime *startTime;
    //edf结束时间
    QDateTime *endTime;
    //edf曲线图左边缘时间
    QDateTime *nowTime;

    //edf曲线图中心时间
    QDateTime *nowCentreTime;
    //edf曲线图左下角时间标签
    QCPItemText *timeLabel;
    //读取数据计时器
    QTimer *readTimer;
    //播放计时器
    QTimer *dataTimer;
    //edf读取数据结构
    edf_hdr_struct *hdr;
    //edf信号读取数组

    //新的x坐标轴
    QCPAxis *topAxis;
    int *readBuff;
    //double *readBuff;
    //edf信号读取数组大小
    int buffSize;
    //与y轴放缩有关
    double avg;

    //电极的标签
    QVector<QCPItemText*> labelItems;
    //刻度位置
    QVector<double> ticks;
    //视图设置类
//    viewConfig viewconfig;
    //初始化电极名称
    //播放当前时间
    double anotherTime;
    //播放速度
    double timeSpeed;
    //y轴缩放倍率
    double fix=1.0;
    //y轴缩放滑动条
    QSlider *fixSlider;
    int maxTimeGap;



    //根据时间重新绘制，用于播放功能
    void rePlotTime(double time);
    //获取x轴中点时间
    void getNowTime();
    //重绘函数，与时间跳转相适配
    void plotLine3();
    //根据x坐标获取时间
    QDateTime getTimeByXAxis(int x);
    //设备连接测试
    void horzScrollBarChanged(int value);
    void replotLine();

    //插件管理
    void loadPlugins();
    void unloadPlugins();
    void processTableWithPlugin(IEEGPlugin* plugin);
    void processTableWithVisualizationPlugin(IEEGPlugin* plugin);



    void initNameList();

    void addMenu();


    //打开edf文件，返回edf路径
    QString openEdf();
    //画出edfPath路径edf文件的波形图
    void plotLine(QString edfPath);
    //void getRecentFiles();
    //获取均值
    //没有实现
    void getAvg();
    void timeLabelChange(double xMin);
    void setLabelItemsPositions();

    //信号和槽绑定以及初始化
    void init();
    void initTimeLabelAndBack();
    void addStatus();

    //绘制槽函数，用于播放
    void timeToPlot();

    //int->double
    void convertAllDataToTable();



private slots:
    //打开文件
    void on_actOpenFile_triggered();
    //截图功能
    void on_prtsc_triggered();
    //时间跳转槽函数
    void on_actTimeJump_triggered();
    //更改时间
    void changeTime(QDateTime jumpTime);
    //开始功能
    void on_actStart_triggered();

    void on_actionClosefile_triggered();

    //停止
    void on_actStop_triggered();
    //加速
    void on_actiontest_triggered();
    //减速
    void on_action_triggered();
    //快进
    void on_actStepforward_triggered();
    //后退
    void on_actStepback_triggered();

    // 插件相关
    void onPluginActionTriggered();
    void onTableProcessed(const QVector<QVector<double>>& result);

    //显示文件的基本信息
    void on_actionInfo_triggered();




};
#endif // MAINWINDOW_H
