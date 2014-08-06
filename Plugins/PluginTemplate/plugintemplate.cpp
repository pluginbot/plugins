#include "plugintemplate.h"

PluginTemplate::PluginTemplate(QObject *parent) : QObject(parent)
{
    try
    {
        mRunning = false;

    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

QString PluginTemplate::Name()
{
    try
    {
        return "Plugin Template";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }

}

QString PluginTemplate::Description()
{
    try
    {
        return "Put a short description here.";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString PluginTemplate::LongDescription()
{
    try
    {
        return "This is the long description of the plugin. This is where you would put details you want the end users to know.";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString PluginTemplate::AuthorName()
{
    try
    {
        return "Bryan Cairns";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString PluginTemplate::AuthorURL()
{
    try
    {
        return "http://www.voidrealms.com";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString PluginTemplate::AuthorEmail()
{
    try
    {
        return "bcairns@voidrealms.com";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString PluginTemplate::Version()
{
    try
    {
        return "1.0";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return 0;
    }
}

bool PluginTemplate::Start()
{
    try
    {
        //if we are running a process or server we would attempt to start
        qDebug() << this->Name() << " started";
        mRunning = true;
        return mRunning;
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

bool PluginTemplate::Stop()
{
    try
    {
        //if we are running a process or server we would attempt to stop
        qDebug() <<  this->Name() << " stopped";
        mRunning = false;
        return mRunning;
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

void PluginTemplate::Install(QString FileName)
{

    try
    {
        qDebug() <<  this->Name() << " installed at " << FileName;
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void PluginTemplate::Uninstall()
{
    try
    {
        qDebug() <<  this->Name() << " uninstall called";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

bool PluginTemplate::isRunning()
{
    try
    {
        return mRunning;
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void PluginTemplate::Update(QVariantMap params)
{
    try
    {
        //Called when the program updates
        foreach(QString key, params.keys())
        {
            qDebug() << "Plugin Update: " << key << " = " << params.value(key).toString();
        }
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

QList<QVariantMap> PluginTemplate::Options()
{
    QList<QVariantMap> Options;

    try
    {
        //create a list of options




        QVariantMap optPass;
        optPass.insert("name","Password");
        optPass.insert("type","string");
        optPass.insert("default","password");
        optPass.insert("description","This is the password");

        Options.append(optPass);

        //Return the list of options
        return Options;

    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return Options;
    }
}

void PluginTemplate::Configure(QVariantMap options)
{
    //Called before the plugin is started and updateed
    foreach(QString option, options.keys())
    {
        qDebug() << Name() << " = " << option << " - " << options.value(option).toString();
    }
}

void PluginTemplate::HandleError(QString ErrorMessage)
{
    try
    {
        qDebug() << ErrorMessage;
    }
    catch(...)
    {
        //If we can't qDebug() then something is really wrong.
    }
}
