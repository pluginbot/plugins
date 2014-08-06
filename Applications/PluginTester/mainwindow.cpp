#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    try
    {
        ui->setupUi(this);
        this->setCentralWidget(ui->splitter);

        //Start intercepting qDebug messages
        DebugWindow = ui->textEdit;
        qInstallMessageHandler(qtMessageHook);

        qDebug() << "Intercepting log messages.";

        loadSettings();

        refreshGUI();


    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

MainWindow::~MainWindow()
{
    qDeleteAll(mPlugins);
    delete ui;
}

void MainWindow::handleError(QString err)
{
    qCritical() << err;
}

void MainWindow::loadPlugin(QString filename)
{
    try
    {
        //Check to make sure the plugin is not already in the map
        if(containsPlugin(filename)) throw QString("Plugin already exists!");

        //Load the file and get the instance
        QScopedPointer<QPluginLoader> loader(new QPluginLoader(filename, this));
        QObject* obj = loader->instance();
        if(!obj) throw QString("Could not load an instance of the plugin");

        //Convert the object into an iPlugin interface
        iPlugin* plugin = qobject_cast<iPlugin*>(obj);
        if(!plugin) throw QString("Not a valid plugin!");


        //Add the plugin to the map
        mPlugins.insert(plugin,loader.take());

        //Refresh teh GUI
        refreshGUI();

    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::refreshGUI()
{
    try
    {
        //Check for missing items tree items
        foreach(iPlugin* plugin, mPlugins.keys())
        {
            QPluginLoader* loader = mPlugins.value(plugin);
            if(!containsTreeItem(loader->fileName()))
            {
                QTreeWidgetItem* item = new QTreeWidgetItem();
                item->setText(0,plugin->Name());
                item->setText(1,loader->fileName());
                item->setIcon(0,QIcon(":/images/icons/pluginstopped.png"));
                ui->treeWidget->insertTopLevelItem(0,item);
            }
        }

        //Check for deleted items
        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i ++)
        {
            QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
            if(!containsPlugin(item->data(1, Qt::DisplayRole).toString()))
            {
                delete item;
            }
        }

        //check the status
        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i ++)
        {
            QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
            iPlugin* plugin = pluginFromItem(item);
            if(plugin)
            {
                if(plugin->isRunning())
                {
                    item->setIcon(0,QIcon(":/images/icons/pluginstarted.png"));
                }
                else
                {
                    item->setIcon(0,QIcon(":/images/icons/pluginstopped.png"));
                }

            }
        }


    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

bool MainWindow::containsPlugin(QString filename)
{
    try
    {
        foreach(QPluginLoader* loader, mPlugins.values())
        {
            if(loader->fileName() == filename) return true;
        }

        return false;
    }
    catch(QString err)
    {
        handleError(err);
        return false;
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

bool MainWindow::containsTreeItem(QString filename)
{
    try
    {
        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i ++)
        {
            QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
            if(item->data(1,Qt::DisplayRole).toString() == filename) return true;
        }

        return false;
    }
    catch(QString err)
    {
        handleError(err);
        return false;
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

iPlugin* MainWindow::pluginFromItem(QTreeWidgetItem* item)
{
    try
    {
        foreach(iPlugin* plugin, mPlugins.keys())
        {
            QPluginLoader* loader = mPlugins.value(plugin);
            if(loader->fileName() == item->data(1,Qt::DisplayRole).toString()) return plugin;
        }

        return 0;
    }
    catch(QString err)
    {
        handleError(err);
        return 0;
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return 0;
    }
}

void MainWindow::qtMessageHook(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    try
    {
        if(context.line == 0)
        {
            fprintf(stdout,"Line is zero");
        }

        if(msg.isNull())
        {
            fprintf(stdout,"Message is null");
        }

        //QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            DebugWindow->append("Info: " + msg);
            break;
        case QtWarningMsg:
            DebugWindow->append("Warning: " + msg);
            break;
        case QtCriticalMsg:
            DebugWindow->append("Critical: " + msg);
            break;
        case QtFatalMsg:
            DebugWindow->append("Fatal: " + msg);
            abort();
        }
    }
    catch(...)
    {
        fprintf(stdout,"Error in qtMessage hook for logger!");
    }
}

void MainWindow::on_actionAdd_triggered()
{
    try
    {
        //Add a plugin to the list
        QString filter = "Plugins *.s (*.so)";

#ifdef Q_OS_WIN
        //If we are on windows then change the filter
        filter = "Plugins *.dll (*.dll)";
#endif

        QString filename = QFileDialog::getOpenFileName(this,"Choose a Plugin","",filter);
        if(filename.isEmpty()) return;
        QFileInfo fi(filename);
        if(!fi.exists()) throw QString("The plugin does not exist!");

       loadPlugin(filename);

       saveSettings();

    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::on_actionDelete_triggered()
{
    try
    {
        if(ui->treeWidget->selectedItems().count() <= 0)
        {
            QMessageBox::warning(this,"Nothing Selected","Please select an item from the list of plugins!");
            return;
        }

        foreach(QTreeWidgetItem* item,ui->treeWidget->selectedItems())
        {
            iPlugin* plugin = pluginFromItem(item);
            if(!plugin) throw QString("Plugin not located: " + item->data(1,Qt::DisplayRole).toString());
            mPlugins.remove(plugin);
            refreshGUI();
        }

        saveSettings();
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::on_actionRefresh_triggered()
{
    try
    {
        refreshGUI();
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::on_actionProperties_triggered()
{
    try
    {
        if(ui->treeWidget->selectedItems().count() <= 0)
        {
            QMessageBox::warning(this,"Nothing Selected","Please select an item from the list of plugins!");
            return;
        }

        QScopedPointer<frmProperties> frm(new frmProperties(this));

        iPlugin* plugin = pluginFromItem(ui->treeWidget->selectedItems().at(0));
        if(!plugin) throw QString("Not a valid plugin!");

        frm.data()->loadPlugin(plugin);
        frm.data()->exec();

    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::on_actionInstall_triggered()
{
    try
    {
        if(ui->treeWidget->selectedItems().count() <= 0)
        {
            QMessageBox::warning(this,"Nothing Selected","Please select an item from the list of plugins!");
            return;
        }

        qWarning() << "During a normal install, your plugin is decompressed and placed into a special folder, it is then installed and configured!";

        foreach(QTreeWidgetItem* item, ui->treeWidget->selectedItems())
        {
            iPlugin* plugin = pluginFromItem(item);
            if(plugin)
            {
                if(plugin->isRunning())
                {
                    qWarning() << "Skipping: " << plugin->Name() << " as it is running!";
                    continue;
                }

                QPluginLoader* loader = mPlugins.value(plugin);
                plugin->Install(loader->fileName());
            }

            refreshGUI();
        }
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::on_actionUninstall_triggered()
{
    try
    {
        if(ui->treeWidget->selectedItems().count() <= 0)
        {
            QMessageBox::warning(this,"Nothing Selected","Please select an item from the list of plugins!");
            return;
        }

        qWarning() << "During a normal uninstall, your plugin and all associated files will be deleted!";

        foreach(QTreeWidgetItem* item, ui->treeWidget->selectedItems())
        {
            iPlugin* plugin = pluginFromItem(item);
            if(plugin)
            {
                if(plugin->isRunning()) plugin->Stop();
                plugin->Uninstall();
            }

            refreshGUI();
        }
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::on_actionStart_triggered()
{
    try
    {
        if(ui->treeWidget->selectedItems().count() <= 0)
        {
            QMessageBox::warning(this,"Nothing Selected","Please select an item from the list of plugins!");
            return;
        }

        //get the options of the first selected plugin
        iPlugin* selected = pluginFromItem(ui->treeWidget->selectedItems().at(0));
        if(!selected) throw QString("Could not get the selected plugin");
        QVariantMap params;
        foreach(QVariantMap map, selected->Options())
        {
            if(map.contains("name") && map.contains("default"))
            {
                params.insert(map.value("name").toString().toLower(),map.value("default").toString());
            }
        }


        //Get the config settings to start the plugins with
        QScopedPointer<frmUpdate> frm(new frmUpdate(this));
        frm.data()->setWindowTitle("Start Plugins");
        frm.data()->addParams(params);
        int ret = frm.data()->exec();
        if(ret != QDialog::Accepted) return;

        foreach(QTreeWidgetItem* item, ui->treeWidget->selectedItems())
        {
            iPlugin* plugin = pluginFromItem(item);
            if(!plugin)
            {
                qCritical() << item->text(0) << ": could not get a valid pointer!";
            }
            else
            {
                if(!plugin->isRunning())
                {
                    plugin->Configure(frm.data()->params());
                    plugin->Start();
                }
                else
                {
                    qWarning() << "Skipping plugin: " << plugin->Name() << " as it is already running!";
                }
            }
        }

        refreshGUI();
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::on_actionStop_triggered()
{
    try
    {
        if(ui->treeWidget->selectedItems().count() <= 0)
        {
            QMessageBox::warning(this,"Nothing Selected","Please select an item from the list of plugins!");
            return;
        }

        foreach(QTreeWidgetItem* item, ui->treeWidget->selectedItems())
        {
            iPlugin* plugin = pluginFromItem(item);
            if(plugin) plugin->Stop();
            refreshGUI();
        }
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::on_actionUpdate_triggered()
{
    try
    {
        if(ui->treeWidget->selectedItems().count() <= 0)
        {
            QMessageBox::warning(this,"Nothing Selected","Please select an item from the list of plugins!");
            return;
        }

        //get the options of the first selected plugin
        iPlugin* selected = pluginFromItem(ui->treeWidget->selectedItems().at(0));
        if(!selected) throw QString("Could not get the selected plugin");
        QVariantMap params;
        foreach(QVariantMap map, selected->Options())
        {
            if(map.contains("name") && map.contains("default"))
            {
                params.insert(map.value("name").toString().toLower(),map.value("default").toString());
            }
        }

        QScopedPointer<frmUpdate> frm(new frmUpdate(this));
        frm.data()->setWindowTitle("Update Plugins");
        frm.data()->addParams(params);
        int ret = frm.data()->exec();
        if(ret != QDialog::Accepted) return;

        foreach(QTreeWidgetItem* item, ui->treeWidget->selectedItems())
        {
            iPlugin* plugin = pluginFromItem(item);
            if(!plugin)
            {
                qCritical() << item->text(0) << ": could not get a valid pointer!";
            }
            else
            {
                if(plugin->isRunning())
                {
                    plugin->Update(frm->params());
                }
                else
                {
                    qWarning() << "Skipping plugin: " << plugin->Name() << " as it is not running!";
                }
            }
        }
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::loadSettings()
{
    try
    {
        QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini",QSettings::IniFormat);

        settings.beginGroup("plugins");
        foreach(QString key, settings.allKeys())
        {
            loadPlugin(settings.value(key,"").toString());
        }

        settings.endGroup();
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void MainWindow::saveSettings()
{
    try
    {
        QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini",QSettings::IniFormat);

        settings.remove("plugins");
        settings.beginGroup("plugins");

        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
        {
            QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
            QString num = QString::number(i);
            settings.setValue(num, item->data(1, Qt::DisplayRole).toString());
        }
        settings.endGroup();
    }
    catch(QString err)
    {
        handleError(err);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}
