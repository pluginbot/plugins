#ifndef TELNETSERVER_H
#define TELNETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QDebug>
#include "telnetclient.h"

/**
 * @brief The Telnet Server
 */
class TelnetServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TelnetServer(QObject *parent = 0);

    /**
     * @brief Sets the password
     * @param password
     */
    void setPassword(QString password);

signals:

    /**
     * @brief Emitted when the server is started
     */
    void started();

    /**
     * @brief Emitted when the server is stopped
     */
    void stopped();

    /**
     * @brief Response for an authentication request
     * @param Passed
     */
    void authentication(bool Passed);

public slots:

    /*!
    * Starts the TCP Server
    * @param port The port to start the server on
    */
    void start(qint16 port);

    /*!
    * Stops the TCP Server
    */
    void stop();

    /**
     * @brief Authenticate the client
     * @param password
     */
    void authenticate(QString password);



protected:

    /**
    * Processes incomming TCP connections
    * @param socketDescriptor The descriptor of the conection
    */
    void incomingConnection(qintptr socketDescriptor);

private:

    /**
     * @brief The password
     */
    QString m_Password;



    /**
     * @brief Handle Errors
     * @param message
     */
    void handleError(QString message);
};

#endif // TELNETSERVER_H
