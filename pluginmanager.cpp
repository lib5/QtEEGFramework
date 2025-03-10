#include "PluginManager.h"
#include <QDebug>


PluginManager::PluginManager(QObject *parent) : QObject(parent) {}


PluginManager::~PluginManager() {
    unloadPlugins();
}

void PluginManager::loadPlugins(const QString& pluginDir) {
    this->pluginDir = pluginDir;
    QDir dir(pluginDir);

    if (!dir.exists()) {
        emit errorOccurred("Plugin directory does not exist: " + pluginDir);
        return;
    }

    foreach (QString fileName, dir.entryList(QDir::Files)) {
        QPluginLoader* pluginLoader = new QPluginLoader(dir.absoluteFilePath(fileName), this);
        QObject* pluginInstance = pluginLoader->instance();

        if (pluginInstance) {
            IEEGPlugin* tablePlugin = qobject_cast<IEEGPlugin*>(pluginInstance);
            if (tablePlugin) {
                QString pluginName = tablePlugin->pluginName();
                plugins.insert(pluginName, tablePlugin);
                pluginLoaders.insert(pluginName, pluginLoader);
                tablePlugin->initialize();
                emit pluginLoaded(pluginName);
                qDebug() << "Loaded plugin:" << pluginName;
            } else {
                emit errorOccurred("Invalid plugin: " + fileName);
                delete pluginLoader;
            }
        } else {
            emit errorOccurred("Failed to load plugin: " + fileName + " - " + pluginLoader->errorString());
            delete pluginLoader;
        }
    }
}

void PluginManager::unloadPlugins() {
    foreach (const QString& pluginName, plugins.keys()) {
        IEEGPlugin* plugin = plugins.value(pluginName);
        QPluginLoader* pluginLoader = pluginLoaders.value(pluginName);

        plugin->destroy();
        pluginLoader->unload();
        delete pluginLoader;

        plugins.remove(pluginName);
        pluginLoaders.remove(pluginName);
        emit pluginUnloaded(pluginName);
        qDebug() << "Unloaded plugin:" << pluginName;
    }
}


QMap<QString, IEEGPlugin*> PluginManager::getPlugins() const {
    return plugins;
}


IEEGPlugin* PluginManager::getPlugin(const QString& pluginName) const {
    return plugins.value(pluginName, nullptr);
}


QString PluginManager::getPluginDir() const {
    return pluginDir;
}


void PluginManager::setPluginDir(const QString& pluginDir) {
    this->pluginDir = pluginDir;
}
