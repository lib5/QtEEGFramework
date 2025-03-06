#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QDir>
#include <QPluginLoader>
#include "itableplugin.h"

class PluginManager : public QObject {
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = nullptr);
    ~PluginManager();

    // 加载所有插件
    void loadPlugins(const QString& pluginDir);

    // 卸载所有插件
    void unloadPlugins();

    // 获取插件列表
    QMap<QString, ITablePlugin*> getPlugins() const;

    // 获取指定插件
    ITablePlugin* getPlugin(const QString& pluginName) const;

    // 获取插件目录
    QString getPluginDir() const;

    // 设置插件目录
    void setPluginDir(const QString& pluginDir);

signals:
    void pluginLoaded(const QString& pluginName);
    void pluginUnloaded(const QString& pluginName);
    void errorOccurred(const QString& errorMessage);

private:
    QMap<QString, ITablePlugin*> plugins;          // 插件名称 -> 插件实例
    QMap<QString, QPluginLoader*> pluginLoaders;   // 插件名称 -> 插件加载器
    QString pluginDir;                             // 插件目录
};

#endif // PLUGINMANAGER_H
