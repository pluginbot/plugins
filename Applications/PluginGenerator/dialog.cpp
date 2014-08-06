#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    try
    {
        ui->setupUi(this);

        //connect the signals and slots
        connect(ui->btnChoose,SIGNAL(clicked()),this,SLOT(choosePlugin()));
        connect(ui->btnOk,SIGNAL(clicked()),this,SLOT(savePlugin()));

        QString strBits = QString::number(OperatingSystem::getProcessorBits());
        strBits.number(OperatingSystem::getProcessorBits());
        ui->cmbOS->setCurrentIndex(ui->cmbOS->findText(OperatingSystem::getOperatingSystem(),Qt::MatchContains));
        ui->cmbBits->setCurrentIndex(ui->cmbBits->findText(strBits,Qt::MatchExactly));
    }
    catch(QString err)
    {
           QMessageBox::critical(this,"Error",err);
    }
    catch(...)
    {
        QMessageBox::critical(this,"Error","Unknonwn error!");
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::choosePlugin()
{
    try
    {

        //Make sure to remind them to only publich in release mode
#ifdef QT_DEBUG
        QMessageBox::warning(this,"Debug mode detected!","We detected that you compiled in debug mode, only plugins compiled in release mode will function correctly!");
#endif


        //Let the user choose a plugin
        QString Extention = "Plugins *.so (*.so)";
        if(OperatingSystem::getOperatingSystem() == "windows") Extention = "Plugins *.dll (*.dll)";
        QString Filename = QFileDialog::getOpenFileName(this,"Choose a Plugin","",Extention);
        QFileInfo fi(Filename);

        //Exit out if the file does not exist.
        if(!fi.exists()) return;

        ui->txtPlugin->setText(Filename);

        //Load the plugin
        QScopedPointer<QPluginLoader> Loader(new QPluginLoader(fi.filePath(),this));
        QScopedPointer<QObject>  Obj(Loader->instance());

        if(Obj.data())
        {
           iPlugin* Plugin = qobject_cast<iPlugin*>(Obj.data());

            if(Plugin)
            {
                //Load the information from the plugin
                ui->txtAuthor->setText(Plugin->AuthorName());
                ui->txtEmail->setText(Plugin->AuthorEmail());
                ui->txtName->setText(Plugin->Name());
                ui->txtVersion->setText(Plugin->Version());
                ui->txtDescription->setText(Plugin->Description());
                ui->txtLongDescription->setPlainText(Plugin->LongDescription());

                ui->treeWidget->clear();

                QList<QVariantMap> Options = Plugin->Options();
                foreach(QVariantMap opt, Options)
                {
                    QString oName = "No Name";
                    QString oType = "string";
                    QString oDefault = "";
                    QString oDescription = "";

                    if(opt.contains("name")) oName = opt.value("name").toString();
                    if(opt.contains("type")) oType = opt.value("type").toString();
                    if(opt.contains("default")) oDefault = opt.value("default").toString();
                    if(opt.contains("description")) oDescription = opt.value("description").toString();

                    QTreeWidgetItem* item = new QTreeWidgetItem();
                    item->setText(0,oName);
                    item->setText(1,oType);
                    item->setText(2,oDefault);
                    item->setText(3,oDescription);
                    ui->treeWidget->addTopLevelItem(item);

                }


            }
            else
            {
                throw QString("Not a valid plugin!");
            }
        }
        else
        {
            throw Loader.data()->errorString();
        }
    }
    catch(QString err)
    {
           QMessageBox::critical(this,"Error",err);
    }
    catch(...)
    {
        QMessageBox::critical(this,"Error","Unknonwn error!");
    }
}

void Dialog::savePlugin()
{
    try
    {
        //verify the plugin exists
        QFileInfo fi(ui->txtPlugin->text());
        if(!fi.exists()) throw QString("The plugin does not exist!");

        //make sure we have valid data
        if(ui->txtName->text() == "") throw QString("Name can not be blank!");
        if(ui->txtVersion->text() == "") throw QString("Version can not be blank!");
        if(ui->txtAuthor->text() == "") throw QString("Author can not be blank!");
        if(ui->txtEmail->text() == "") throw QString("Email can not be blank!");
        if(ui->txtDescription->text() == "") throw QString("Description can not be blank!");
        if(ui->txtLongDescription->toPlainText() == "") throw QString("Long description can not be blank!");


        //Save to zip
        QString filename = QFileDialog::getSaveFileName(this,"Create Plugin Submition","","Zip File *.zip (*.zip)");
        QFileInfo zfi(filename);
        QFile zfile(zfi.filePath());

        //Check for overwrite
        if(zfi.exists())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"Overwrite Warning","File already exists! Would you like to overwrite it?", QMessageBox::Yes | QMessageBox::No);
            if(reply != QMessageBox::Yes) throw QString("Your changes have not been saved!");
            zfile.remove();
        }

        //Generate the xml file
        QTemporaryFile temp;
        if(!temp.open())throw temp.errorString();

        QXmlStreamWriter xml;
        xml.setDevice(&temp);
        xml.writeStartDocument();

        xml.writeStartElement("plugin");
        xml.writeAttribute("name",ui->txtName->text());
        xml.writeAttribute("os",ui->cmbOS->currentText());
        xml.writeAttribute("bits",ui->cmbBits->currentText());
        xml.writeAttribute("version",ui->txtVersion->text());
        xml.writeAttribute("authorname",ui->txtAuthor->text());
        xml.writeAttribute("authoremail",ui->txtEmail->text());

        //write each option
        xml.writeStartElement("options");

        for( int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i )
        {
           QTreeWidgetItem *item = ui->treeWidget->topLevelItem( i );
           xml.writeStartElement("option");
           xml.writeAttribute("name",item->data(0,Qt::DisplayRole).toString());
           xml.writeAttribute("type",item->data(1,Qt::DisplayRole).toString());
           xml.writeAttribute("default",item->data(2,Qt::DisplayRole).toString());
           xml.writeAttribute("description",item->data(3,Qt::DisplayRole).toString());
           xml.writeEndElement();
        }

        xml.writeEndElement();

        xml.writeTextElement("short",ui->txtDescription->text());
        xml.writeTextElement("long",ui->txtLongDescription->toPlainText());

        xml.writeEndElement();



        xml.writeEndDocument();

        //Write the changes to disk
        temp.flush();
        temp.seek(0);


        //Save the zip file
        if(!zfile.open(QIODevice::WriteOnly)) throw zfile.errorString();

        ZipWriter zip(&zfile);

        //write the plugin.xml file
        zip.addFile("plugin.xml", temp.readAll());
        temp.close();

        //Write the plugin to the file
        QFile pluginfile(fi.filePath());
        if(!pluginfile.open(QIODevice::ReadOnly)) throw pluginfile.errorString();
        zip.addFile(fi.fileName(),pluginfile.readAll());
        pluginfile.close();

        //Close the zip
        zip.close();
        zfile.close();

        //remove the temp file
        temp.remove();

        QMessageBox::information(this,"Plugin Submition Created!","The plugin submition was created, add any extra files you need to the archive!");

    }
    catch(QString err)
    {
           QMessageBox::critical(this,"Error",err);
    }
    catch(...)
    {
        QMessageBox::critical(this,"Error","Unknonwn error!");
    }
}

void Dialog::on_pushButton_clicked()
{
    try
    {
        frmAbout* frm = new frmAbout(this);
        frm->setModal(true);
        frm->exec();
        frm->deleteLater();
    }
    catch(QString err)
    {
        QMessageBox::critical(this,"Error",err);
    }
    catch(...)
    {
        QMessageBox::critical(this,"Error","Unknonwn error!");
    }
}
