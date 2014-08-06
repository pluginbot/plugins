#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTextEdit>
#include <QSettings>
#include <QFileInfo>
#include <QFile>
#include <QPluginLoader>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QVariantMap>
#include "iPlugin.h"
#include "frmupdate.h"
#include "frmproperties.h"


extern QTextEdit* DebugWindow;

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /**
     * @brief Add a plugin
     */
    void on_actionAdd_triggered();

    /**
     * @brief Remove a plugin
     */
    void on_actionDelete_triggered();

    /**
     * @brief Refresh the GUI
     */
    void on_actionRefresh_triggered();

    /**
     * @brief Show the properties of a plugin
     */
    void on_actionProperties_triggered();

    /**
     * @brief Install a plugin
     */
    void on_actionInstall_triggered();

    /**
     * @brief Uninstall a plugin
     */
    void on_actionUninstall_triggered();

    /**
     * @brief Start a plugin
     */
    void on_actionStart_triggered();

    /**
     * @brief Stop a plugin
     */
    void on_actionStop_triggered();

    /**
     * @brief Update a plugin
     */
    void on_actionUpdate_triggered();

private:
    Ui::MainWindow *ui;

    /**
     * @brief Load settings
     */
    void loadSettings();

    /**
     * @brief Saves the settings
     */
    void saveSettings();

    /**
    * @brief A QMap of the plugins and their associated QPluginLoader
    */
    QMap<iPlugin*,QPluginLoader*> mPlugins;

    /**
     * @brief Handle any errors
     * @param err
     */
    void handleError(QString err);

    /**
     * @brief Load a plugin
     * @param filename
     */
    void loadPlugin(QString filename);

    /**
     * @brief refresh the GUI
     */
    void refreshGUI();

    /**
     * @brief Determines if the map contains a plugin based on the fileanem
     * @param filename The filename of the plugin
     * @return Boolean
     */
    bool containsPlugin(QString filename);

    /**
     * @brief Determines if the Treewidget contains a plugin based on the fileanem
     * @param filename The filename of the plugin
     * @return Boolean
     */
    bool containsTreeItem(QString filename);

    /**
     * @brief Get a plugin from a QTreeWidgetItem
     * @param item The QTreeWidgetItem
     * @return A pointer to the plugin or 0
     */
    iPlugin* pluginFromItem(QTreeWidgetItem* item);


    /**
     * @brief qtMessageHook for traping qDebug qWarning and qCritical messages
     * @param type
     * @param context
     * @param msg
     */
    static void qtMessageHook(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // MAINWINDOW_H
