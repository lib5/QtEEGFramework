#ifndef IEEGPLUGIN_H
#define IEEGPLUGIN_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QMap>
#include <QThread>
#include <QWidget>

class IEEGPlugin {

public:
    explicit IEEGPlugin() = default;
    virtual ~IEEGPlugin() = default;

    // 插件基本信息
    virtual QString pluginName() const = 0;                     // 插件名称
    virtual QString pluginDescription() const = 0;              // 插件描述
    virtual QString pluginVersion() const = 0;                  // 插件版本
    virtual QString pluginAuthor() const = 0;                   // 插件作者
    virtual QString pluginCategory() const = 0;                 // 插件类别

    // 插件初始化与销毁
    virtual void initialize() = 0;                              // 初始化插件
    virtual void destroy() = 0;                                 // 释放资源

    // EEG 数据处理
    virtual QVector<QVector<double>> processEEGData(const QVector<QVector<double>>& eegData) = 0; // 处理 EEG 数据
    virtual QVector<QVector<double>> previewEEGData(const QVector<QVector<double>>& eegData, int maxSamples = 500) = 0; // 预览处理结果

    // 可视化功能
    virtual QWidget* generateVisualization(QWidget* parent = nullptr) = 0; // 生成 EEG 波形图或频谱图

    // 参数管理
    virtual void setParameter(const QString& name, const QVariant& value) = 0;
    virtual QVariant getParameter(const QString& name) const = 0;
    virtual QMap<QString, QVariant> getDefaultParameters() const = 0;
    virtual QMap<QString, QString> getParameterDescriptions() const = 0;

    // Redo/Undo
    virtual void redo() = 0;
    virtual void undo() = 0;
    virtual bool canRedo() const = 0;
    virtual bool canUndo() const = 0;

    // 多线程支持
    virtual void setThread(QThread* thread) = 0;
    virtual bool isThreadSafe() const = 0;

    // 插件间通信
    virtual void sendMessage(const QString& message, const QVariant& data) = 0;
    virtual void receiveMessage(const QString& message, const QVariant& data) = 0;

    // 日志与错误处理
    virtual QString lastError() const = 0;
    virtual void logMessage(const QString& message, int level = 0) = 0;  // level: 0=INFO, 1=WARNING, 2=ERROR

    // 状态管理
    virtual bool isInitialized() const = 0;
    virtual bool isBusy() const = 0;
    virtual void cancel() = 0;

    // 导出与保存
    virtual bool exportResults(const QString& filePath) = 0;
    virtual bool saveState(const QString& filePath) = 0;
    virtual bool loadState(const QString& filePath) = 0;

    // 界面支持
    virtual QWidget* createParameterWidget(QWidget* parent = nullptr) = 0;
    virtual QWidget* createResultWidget(QWidget* parent = nullptr) = 0;

signals:
    void messageSent(const QString& message, const QVariant& data);
    void eegProcessed(const QVector<QVector<double>>& result);   // EEG 数据处理完成信号
    void visualizationUpdated();                                 // 可视化更新信号
    void progressChanged(int progress);
    void errorOccurred(const QString& errorMessage);

};

Q_DECLARE_INTERFACE(IEEGPlugin, "com.example.EEGPluginInterface/1.0")

#endif // IEEGPLUGIN_H
