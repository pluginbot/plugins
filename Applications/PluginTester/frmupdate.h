#ifndef FRMUPDATE_H
#define FRMUPDATE_H

#include <QDialog>
#include <QDebug>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QDateTime>
#include <QVariantMap>

namespace Ui {
class frmUpdate;
}

class frmUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit frmUpdate(QWidget *parent = 0);
    ~frmUpdate();

    /**
     * @brief Returns the params used to update a plugin
     * @return QVariantMap
     */
    QVariantMap params();

    /**
     * @brief Add params to the list
     * @param map
     */
    void addParams(QVariantMap map);


private slots:
    /**
     * @brief Add clicked
     */
    void on_btnAdd_clicked();

    /**
     * @brief Delete clicked
     */
    void on_btnDelete_pressed();

    /**
     * @brief OK clicked
     */
    void on_btnOK_pressed();

    /**
     * @brief Cancel clicked
     */
    void on_btnCancel_pressed();

    /**
     * @brief Default button
     */
    void on_btnDefault_clicked();


private:
    Ui::frmUpdate *ui;

    /**
     * @brief Loads the default items
     */
    void loadDefaults();

    /**
     * @brief Adds a item to the tree view
     * @param key
     * @param value
     */
    void addTreeItem(QString key,QString value);

    /**
     * @brief Determines if the key exists
     * @param key
     */
    bool keyExists(QString key);

    /**
     * @brief Handle any errors
     * @param err
     */
    void handleError(QString err);
};

#endif // FRMUPDATE_H
