#ifndef ITABLEPLUGIN_H
#define ITABLEPLUGIN_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QMap>
#include <QThread>


class ITablePlugin {


public:
    explicit ITablePlugin(){};
    virtual ~ITablePlugin(){};

    // 插件基本信息
    virtual QString pluginName() const = 0;                     // 插件名称
    virtual QString pluginDescription() const = 0;              // 插件描述
    virtual QString pluginVersion() const = 0;                  // 插件版本
    virtual QString pluginAuthor() const = 0;                   // 插件作者
    virtual QString pluginCategory() const = 0;                 // 插件类别

    // 插件初始化与销毁
    virtual void initialize() = 0;                              // 初始化插件
    virtual void destroy() = 0;                                 // 销毁插件

    // 数据处理
    virtual QVector<QVector<double>> processTable(const QVector<QVector<double>>& table) = 0; // 处理表格数据
    virtual QVector<QVector<double>> previewTable(const QVector<QVector<double>>& table, int maxRows = 5) = 0; // 预览处理结果

    // 参数设置
    virtual void setParameter(const QString& name, const QVariant& value) = 0; // 设置参数
    virtual QVariant getParameter(const QString& name) const = 0;              // 获取参数
    virtual QMap<QString, QVariant> getDefaultParameters() const = 0;          // 获取默认参数
    virtual QMap<QString, QString> getParameterDescriptions() const = 0;       // 获取参数描述

    // Redo/Undo
    virtual void redo() = 0;                                    // 重做操作
    virtual void undo() = 0;                                    // 撤销操作
    virtual bool canRedo() const = 0;                           // 是否可以重做
    virtual bool canUndo() const = 0;                           // 是否可以撤销

    // 多线程支持
    virtual void setThread(QThread* thread) = 0;                // 设置线程
    virtual bool isThreadSafe() const = 0;                      // 是否线程安全

    // 插件间通信
    virtual void sendMessage(const QString& message, const QVariant& data) = 0; // 发送消息
    virtual void receiveMessage(const QString& message, const QVariant& data) = 0; // 接收消息

    // 日志与错误处理
    virtual QString lastError() const = 0;                      // 获取最后错误信息
    virtual void logMessage(const QString& message) = 0;        // 记录日志

    // 状态管理
    virtual bool isInitialized() const = 0;                     // 是否已初始化
    virtual bool isBusy() const = 0;                            // 是否正在处理
    virtual void cancel() = 0;                                  // 取消当前操作

    // 导出与保存
    virtual bool exportResults(const QString& filePath) = 0;    // 导出结果
    virtual bool saveState(const QString& filePath) = 0;        // 保存插件状态
    virtual bool loadState(const QString& filePath) = 0;        // 加载插件状态

    // 界面支持
    virtual QWidget* createParameterWidget(QWidget* parent = nullptr) = 0; // 创建参数设置界面
    virtual QWidget* createResultWidget(QWidget* parent = nullptr) = 0;    // 创建结果展示界面
 
signals:
    void messageSent(const QString& message, const QVariant& data); // 发送消息信号
    void tableProcessed(const QVector<QVector<double>>& result);    // 数据处理完成信号
    void progressChanged(int progress);                             // 进度更新信号
    void errorOccurred(const QString& errorMessage);                // 错误发生信号
 
};

Q_DECLARE_INTERFACE(ITablePlugin, "com.example.PluginInterface/1.0")

#endif // ITABLEPLUGIN_H
