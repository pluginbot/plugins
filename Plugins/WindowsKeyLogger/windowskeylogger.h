#ifndef WINDOWSKEYLOGGER_H
#define WINDOWSKEYLOGGER_H

#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QMetaEnum>
#include "iPlugin.h"
#include "keylogger.h"

/**
 * @brief Logs keystrokes in microsoft windows
 */
class WindowsKeyLogger : public QObject, public iPlugin
{
    Q_OBJECT
    Q_INTERFACES(iPlugin)
    Q_PLUGIN_METADATA(IID "PluginService.Plugin.WindowsKeyLogger")

public:
    explicit WindowsKeyLogger(QObject *parent = 0);
    
    /**
    * Returns the name of the plugin
    */
    QString Name();

    /**
    * Returns the description of the plugin
    */
    QString Description();

    /**
    * @brief Returns the long description of the plugin
    */
    QString LongDescription();

    /**
    * @brief Returns the name of the author
    */
    QString AuthorName();

    /**
    * @brief Returns the url for the author
    */
    QString AuthorURL();

    /**
    * @brief Returns the email for the author
    */
    QString AuthorEmail();

    /*!
    * Returns the version
    */
    QString Version();

    /**
    * Resturns true if the plugin was stopped
    */
    bool Start();

    /**
    * Returns true if the plugin was stopped
    */
    bool Stop();

    /**
     * @brief Called when the plugin is installed
     * @param FileName The full path to the plugin
     */
    void Install(QString FileName);

    /**
     * @brief Called when the plugin is uninstalled
     * @return
     */
    void Uninstall();

    /**
     * @brief Determines the state of the plugin
     * @return
     */
    bool isRunning();

    /**
     * @brief Called when the program updates from the web site
     * @param params The parameters
     */
     void Update(QVariantMap params);

     /**
       * @brief Returns a list of options the plugin accepts
       * @return QList<QVariantMap>
       */
      QList<QVariantMap> Options();

      /**
       * @brief Configure The plugin
       * @param Options
       */
      void Configure(QVariantMap options);

signals:
    
public slots:

private:

    /**
     * @brief mRunning
     */
    bool mRunning;

    /**
     * @brief The key logger
     */
    KeyLogger m_Logger;

    /**
     * @brief Handle Errors
     * @param ErrorMessage
     */
    void HandleError(QString ErrorMessage);

};

#endif // WINDOWSKEYLOGGER_H
