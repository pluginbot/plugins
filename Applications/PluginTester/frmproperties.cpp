#include "frmproperties.h"
#include "ui_frmproperties.h"

frmProperties::frmProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmProperties)
{
    try
    {
        ui->setupUi(this);
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

frmProperties::~frmProperties()
{
    delete ui;
}

void frmProperties::loadPlugin(iPlugin *plugin)
{
    try
    {
        if(!plugin) throw QString("Not a valid plugin!");

        ui->treeWidget->clear();

        //Load the general info
        ui->txtAuthor->setText(plugin->AuthorName());
        ui->txtEmail->setText(plugin->AuthorEmail());
        ui->txtLong->setPlainText(plugin->LongDescription());
        ui->txtName->setText(plugin->Name());
        ui->txtShort->setText(plugin->Description());
        ui->txtURL->setText(plugin->AuthorURL());
        ui->txtVersion->setText(plugin->Version());

        //Load the options
        foreach(QVariantMap option, plugin->Options())
        {
            QTreeWidgetItem* rootitem = new QTreeWidgetItem();
            QString name = option.values().at(0).toString();
            if(option.contains("name")) name = option.value("name").toString();
            rootitem->setText(0,name);

            foreach(QString key, option.keys())
            {
                QTreeWidgetItem* item = new QTreeWidgetItem();
                item->setText(0,key);
                item->setText(1,option.value(key).toString());
                rootitem->addChild(item);
            }
            rootitem->setExpanded(true);
            ui->treeWidget->addTopLevelItem(rootitem);
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

void frmProperties::on_pushButton_clicked()
{
    try
    {
        accept();
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

void frmProperties::handleError(QString err)
{
    qCritical() << err;
}
