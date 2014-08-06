#include "telnetserver.h"

TelnetServer::TelnetServer(QObject *parent) :
    QTcpServer(parent)
{
    try
    {
        m_Password = "";
    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }
}

void TelnetServer::setPassword(QString password)
{
        try
        {
            m_Password = password;
        }
        catch(...)
        {
            handleError("Unknown error in " + QString(Q_FUNC_INFO));
        }
}

void TelnetServer::handleError(QString message)
{
    qCritical() << message;
}

void TelnetServer::start(qint16 port)
{
    try
    {

        qDebug() << "Telenet server starting on port: " << port;

        //Start listening for connections
        if(!listen(QHostAddress::Any,port)) throw errorString();

        emit started();

        qDebug() << "Telenet server listening on port: " << port;

    }
    catch(QString error)
    {
        handleError(error);
    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }
}

void TelnetServer::stop()
{
    try
    {
        this->close();
        emit stopped();
    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }
}

void TelnetServer::authenticate(QString password)
{
    try
    {
        if(m_Password == password)
        {
            emit authentication(true);
        }
        else
        {
            authentication(false);
        }
    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }
}

void TelnetServer::incomingConnection(qintptr socketDescriptor)
{
    try
    {
        //Create a new socket and accept the connection
        TelnetClient* client = new TelnetClient(this);
        connect(this,SIGNAL(stopped()),client,SLOT(close()));
        connect(client,SIGNAL(authenticate(QString)),this,SLOT(authenticate(QString)));
        connect(this,SIGNAL(authentication(bool)),client,SLOT(authentication(bool)));

        client->acceptConnection(socketDescriptor);
    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }
}
