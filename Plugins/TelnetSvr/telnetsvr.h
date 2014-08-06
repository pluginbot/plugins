#ifndef PLUGINSVR_H
#define PLUGINSVR_H

#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QMetaEnum>
#include <QSettings>
#include "iPlugin.h"
#include "telnetserver.h"

/**
 * @brief The Plugin Template to use when building a plugin
 * Plugins are DLL or SO files
 * When you submit them to the site repository, they must be in the root of a zip file
 * The zip file you submit will be extracted to a sub folder that is named automatically
 */
class TelnetSvr : public QObject, public iPlugin
{
    Q_OBJECT
    Q_INTERFACES(iPlugin)
    Q_PLUGIN_METADATA(IID "PluginService.Plugin.telnetSvr")

public:
    explicit TelnetSvr(QObject *parent = 0);
    
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
       * @brief Returns a list of pptions the plugin accepts
       * @return QList<QVariantMap>
       */
      QList<QVariantMap> Options();

      /**
       * @brief Configure The plugin
       * @param Options
       */
      void Configure(QVariantMap Options);

signals:
    
public slots:

private:

      /**
       * @brief The password
       */
      QString m_Password;

      /**
       * @brief The port
       */
      int m_Port;

      /**
       * @brief The telnet server
       */
      TelnetServer m_Server;


     /**
     * @brief mRunning
     */
      bool mRunning;


     /**
     * @brief Handle Errors
     * @param ErrorMessage
     */
      void HandleError(QString ErrorMessage);


};

#endif // PLUGINSVR_H
