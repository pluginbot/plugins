#include "telnetsvr.h"

TelnetSvr::TelnetSvr(QObject *parent) : QObject(parent)
{
    try
    {
        m_Password = "password";
        m_Port = 5000;
        mRunning = false;

    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

QString TelnetSvr::Name()
{
    try
    {
        return "Telnet Server";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }

}

QString TelnetSvr::Description()
{
    try
    {
        return "A telnet server";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString TelnetSvr::LongDescription()
{
    try
    {
        return "Listens for connections and executes commands on the system command line. Any telnet client can be used to connect to the plugin.";
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString TelnetSvr::AuthorName()
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

QString TelnetSvr::AuthorURL()
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

QString TelnetSvr::AuthorEmail()
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

QString TelnetSvr::Version()
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

bool TelnetSvr::Start()
{
    try
    {
        //if we are running a process or server we would attempt to start

        m_Server.start(m_Port);
        m_Server.setPassword(m_Password);

        if(m_Server.errorString() == "")
        {
            qDebug() << this->Name() << " started";
            mRunning = true;
        }
        else
        {
            qCritical() << this->Name() << " " << m_Server.errorString();
            mRunning = false;
        }

        return mRunning;
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

bool TelnetSvr::Stop()
{
    try
    {
        //if we are running a process or server we would attempt to stop
        m_Server.stop();

        if(m_Server.errorString() == "")
        {
            qDebug() << this->Name() << " stopped";
            mRunning = false;
        }
        else
        {
            qCritical() << this->Name() << " " << m_Server.errorString();
            mRunning = false;
        }

        return mRunning;
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

void TelnetSvr::Install(QString FileName)
{

    try
    {
        qDebug() <<  this->Name() << " installed at " << FileName;

        //save the defaults to the settings file
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

void TelnetSvr::Uninstall()
{
    try
    {
        qDebug() <<  this->Name() << " uninstall called";

        //remove the the settings file

    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

bool TelnetSvr::isRunning()
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

void TelnetSvr::Update(QVariantMap params)
{
    try
    {
        //Called when the program updates
        foreach(QString key, params.keys())
        {
            qDebug() <<  Name() << " Update: " << key << " = " << params.value(key).toString();
        }
    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

QList<QVariantMap> TelnetSvr::Options()
{
    QList<QVariantMap> Options;
    try
    {


        QVariantMap optPass;
        optPass.insert("name","Password");
        optPass.insert("type","string");
        optPass.insert("default","password");
        optPass.insert("description","This is the password");
        Options.append(optPass);


        QVariantMap optPort;
        optPort.insert("name","Port");
        optPort.insert("type","number");
        optPort.insert("default","1000");
        optPort.insert("description","This is the port to run the telnet server on.");
        Options.append(optPort);

        //Return the list of options
        return Options;

    }
    catch(...)
    {
        HandleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return Options;
    }
}

void TelnetSvr::Configure(QVariantMap Options)
{
    //Called before the plugin is started and updateed
    foreach(QString option, Options.keys())
    {
        qDebug() << Name() << " = " << option << " - " << Options.value(option).toString();

        if(option.toLower() == "password")
        {
            m_Password = Options.value(option).toString();
        }

        if(option.toLower() == "port")
        {
            bool bok = false;
            int port = Options.value(option).toInt(&bok);

            //Restart the server if it is running
            if(bok && port != m_Port)
            {
                m_Port = port;
                if(isRunning())
                {
                    qDebug() << Name() << " needs to be restarted";
                    Stop();
                    Start();
                }

            }
        }
    }
}

void TelnetSvr::HandleError(QString ErrorMessage)
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

