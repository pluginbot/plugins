#include "telnetclient.h"

TelnetClient::TelnetClient(QObject *parent) :
    QObject(parent)
{
    try
    {

        //Create a new process
        m_Shell = new  QProcess(this);
        m_Shell->setReadChannelMode(QProcess::MergedChannels);
        //m_Shell->setWorkingDirectory(QDir::currentPath());

        connect(m_Shell,SIGNAL(readyReadStandardOutput()), this,SLOT(shellRead()));
        connect(m_Shell,SIGNAL(readyReadStandardError()), this,SLOT(shellRead()));
        connect(m_Shell,SIGNAL(readyRead()), this,SLOT(shellRead()));

        //Start the shell based on the OS type

    #ifdef Q_OS_WIN
        m_Shell->start("cmd");
    #else
        m_Shell->start("bash");
    #endif

        qDebug() << "Shell Error = " << m_Shell->errorString();

        //Create the socket
        m_Socket = new QTcpSocket(this);
        m_Authenticated = false;
        m_SocketID = 0;

        connect(m_Socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
        connect(m_Socket,SIGNAL(disconnected()),this,SLOT(disconnected()));


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

void TelnetClient::acceptConnection(qintptr socketDescriptor)
{
    try
    {
        //accept the incomming socket
        if(!m_Socket->setSocketDescriptor(socketDescriptor)) throw m_Socket->errorString();
        m_SocketID = socketDescriptor;

        qDebug() << "Telnet client connected: " << m_SocketID;

        sendPrompt();

        //Ask for the password
        authentication(false);

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

void TelnetClient::shellRead()
{
    try
    {
        if(!m_Shell) throw QString("System process not started!");
        if(!m_Socket) throw QString("Socket not started!");
        if(!m_Socket->isOpen()) return;
        QByteArray data = m_Shell->readAll();

        qDebug() << "Shell: " << m_SocketID << " = " << data;

        m_Socket->write(data);
        sendPrompt();
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

void TelnetClient::handleError(QString message)
{
    qCritical() << message;
}

void TelnetClient::sendPrompt()
{
    try
    {
        if(!m_Shell) throw QString("System process not started!");
        if(!m_Socket) throw QString("Socket not started!");
        m_Socket->write("\r\nTelnet> ");
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


void TelnetClient::readyRead()
{
    try
    {
        //process the command


        //Append the buffer
        m_Buffer.append(m_Socket->readAll());

        if(!m_Authenticated)
        {
            QString password = m_Buffer.trimmed();
            m_Buffer.clear();

            emit authenticate(password);
            return;
        }

        if(m_Buffer.endsWith("\n") || m_Buffer.endsWith("\r"))
        {
            qDebug() << "Telnet client to shell: " << m_SocketID << " " << m_Buffer;

            //Send the command to the system process

#ifdef Q_OS_WIN
            m_Shell->write(m_Buffer.toLatin1().trimmed() + "\r\n");
#else
            m_Shell->write(m_Buffer.toLatin1().trimmed() + "\n");
#endif
            //Clear the buffer
            m_Buffer.clear();

            //See if we have a response
            QByteArray ret = m_Shell->peek(1);
            if(ret.length() <= 0) sendPrompt();
        }

    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }
}

void TelnetClient::disconnected()
{
    try
    {
        close();
    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }
}

void TelnetClient::close()
{
    try
    {
        qDebug() << "Closing telnet socket: " << m_SocketID;

        if(m_Shell)
        {
            m_Shell->close();
        }

        if(m_Socket)
        {
            m_Socket->close();
        }

        deleteLater();
    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }

}

void TelnetClient::authentication(bool Passed)
{
    try
    {
        m_Authenticated = Passed;
        if(m_Authenticated)
        {
            m_Socket->write("\r\nAuthenticated! ");
            sendPrompt();
        }
        else
        {
            m_Socket->write("\r\nEnter the password> ");
        }
    }
    catch(...)
    {
        handleError("Unknown error in " + QString(Q_FUNC_INFO));
    }
}

