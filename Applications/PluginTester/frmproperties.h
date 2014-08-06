#ifndef FRMPROPERTIES_H
#define FRMPROPERTIES_H

#include <QDialog>
#include <QVariantMap>
#include <QDebug>
#include "iPlugin.h"

namespace Ui {
class frmProperties;
}

class frmProperties : public QDialog
{
    Q_OBJECT

public:
    explicit frmProperties(QWidget *parent = 0);
    ~frmProperties();

    /**
     * @brief Load a plugins information
     * @param plugin
     */
    void loadPlugin(iPlugin* plugin);

private slots:
    void on_pushButton_clicked();

private:
    Ui::frmProperties *ui;

    /**
     * @brief Handle errors
     * @param err
     */
    void handleError(QString err);
};

#endif // FRMPROPERTIES_H
