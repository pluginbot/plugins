#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QPluginLoader>
#include <QScopedPointer>
#include <QXmlStreamWriter>
#include <QFile>
#include <QTemporaryFile>
#include "operatingsystem.h"
#include "iPlugin.h"
#include "zipwriter.h"
#include "frmabout.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:

    /**
     * @brief Choose a plugin
     */
    void choosePlugin();

    /**
     * @brief Save a plugin
     */
    void savePlugin();

private slots:

    /**
     * @brief show the about window
     */
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;

};

#endif // DIALOG_H
