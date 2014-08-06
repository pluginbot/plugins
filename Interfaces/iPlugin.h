

#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QVariantMap>

/**
 * @brief Interface for Plugins
 */
class iPlugin
{

public:

    /**
    * @brief Returns the name of the plugin
    */
    virtual QString Name() = 0;

    /**
    * @brief Returns the description of the plugin
    */
    virtual QString Description() = 0;

    /**
    * @brief Returns the long description of the plugin
    */
    virtual QString LongDescription() = 0;

    /**
    * @brief Returns the name of the author
    */
    virtual QString AuthorName() = 0;

    /**
    * @brief Returns the url for the author
    */
    virtual QString AuthorURL() = 0;

    /**
    * @brief Returns the email for the author
    */
    virtual QString AuthorEmail() = 0;

    /**
    * @brief Returns the version
    */
    virtual QString Version() = 0;

    /**
    * Resturns true if the plugin was started
    */
    virtual bool Start() = 0;

    /**
    * @brief Returns true if the plugin was stopped
    */
    virtual bool Stop() = 0;

    /**
     * @brief Called when the plugin is installed
     * @param FileName The full path to the plugin
     */
    virtual void Install(QString FileName) = 0;

    /**
     * @brief Called when the plugin is uninstalled
     * @return
     */
    virtual void Uninstall() = 0;

    /**
     * @brief Determines the state of the plugin
     * @return
     */
    virtual bool isRunning() = 0;

    /**
     * @brief Configure The plugin
     * @param options
     */
    virtual void Configure(QVariantMap options) = 0;

    /**
     * @brief Called when the program updates, the params are options from the site
     * @param Pparams The options
     */
     virtual void Update(QVariantMap params) = 0;

    /**
      * @brief Returns a list of options the plugin accepts
      * @return QList<QVariantMap>
      */
     virtual QList<QVariantMap> Options() = 0;
};

Q_DECLARE_INTERFACE( iPlugin, "iPlugin/0.1" )

#endif // IPLUGIN_H
