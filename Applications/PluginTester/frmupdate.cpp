#include "frmupdate.h"
#include "ui_frmupdate.h"

frmUpdate::frmUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmUpdate)
{
    try
    {
        ui->setupUi(this);

        //Load any old settings
        QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini",QSettings::IniFormat);

        settings.beginGroup("update");
        foreach(QString key, settings.allKeys())
        {
            addTreeItem(key,settings.value(key,"").toString());
        }
        settings.endGroup();

        //Show the defaults if there are no items
        if(ui->treeWidget->topLevelItemCount() == 0) loadDefaults();
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

frmUpdate::~frmUpdate()
{
    delete ui;
}

void frmUpdate::on_btnAdd_clicked()
{
    try
    {
        addTreeItem("Key","Value");
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

void frmUpdate::on_btnDelete_pressed()
{
    try
    {
        foreach(QTreeWidgetItem* item, ui->treeWidget->selectedItems())
        {
            delete item;
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

void frmUpdate::on_btnOK_pressed()
{
    try
    {
        //Save the changes

        //Load any old settings
        QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini",QSettings::IniFormat);

        settings.remove("update");
        settings.beginGroup("update");
        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
        {
            QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
            settings.setValue(item->data(0,Qt::DisplayRole).toString(), item->data(1, Qt::DisplayRole).toString());
        }
        settings.endGroup();

        this->accept();
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

void frmUpdate::on_btnCancel_pressed()
{
    try
    {
        this->reject();
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

void frmUpdate::addTreeItem(QString key,QString value)
{
    try
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
        item->setText(0,key);
        item->setText(1,value);
        ui->treeWidget->insertTopLevelItem(ui->treeWidget->topLevelItemCount(), item);

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

bool frmUpdate::keyExists(QString key)
{
    try
    {
        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
        {
            if(ui->treeWidget->topLevelItem(i)->text(0) == key) return true;
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

void frmUpdate::loadDefaults()
{
    try
    {
        ui->treeWidget->clear();

        //Load the default items
        addTreeItem("datemissing","");
        addTreeItem("dateupdated",QDateTime::currentDateTime().toString("MM-dd-yyyy hh:mm:ss"));
        addTreeItem("ipaddress","127.0.0.1");
        addTreeItem("missing","0");
        addTreeItem("name","My Device");
        addTreeItem("status","running");
        addTreeItem("updateinterval","60");
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

void frmUpdate::handleError(QString err)
{
    qCritical() << err;
    QMessageBox::critical(this,"Error",err);
}

void frmUpdate::on_btnDefault_clicked()
{
    try
    {
        loadDefaults();
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

QVariantMap frmUpdate::params()
{
    QVariantMap map;
    try
    {
        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
        {
            QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
            map.insert(item->data(0,Qt::DisplayRole).toString(), item->data(1,Qt::DisplayRole).toString());
        }

        return map;
    }
    catch(QString err)
    {
        handleError(err);
        map.clear();
        return map;
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        map.clear();
        return map;
    }
}

void frmUpdate::addParams(QVariantMap map)
{
    try
    {
        foreach(QString key, map.keys())
        {
            if(!keyExists(key)) addTreeItem(key,map.value(key).toString());
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
