#ifndef TELNETCLIENT_H
#define TELNETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QProcess>
#include <QDir>

/**
 * @brief The Telnet Client controls the client connection
 */
class TelnetClient : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief The default constructor
     * @param parent
     */
    explicit TelnetClient(QObject *parent = 0);

    /**
     * @brief Accepts an incomming connection
     * @param socketDescriptor
     */
    void acceptConnection(qintptr socketDescriptor);

signals:

    /**
     * @brief Authenticate the client
     * @param password
     */
    void authenticate(QString password);

public slots:

    /**
     * @brief When the system shell has data to read
     */
    void shellRead();

    /**
     * @brief Called when there is information to be read from the socket
     */
    void readyRead();

    /**
     * @brief Called when the socket is disconnected
     */
    void disconnected();

    /**
     * @brief Closes the socket
     */
    void close();

    /**
     * @brief Response for an authentication request
     * @param Passed
     */
    void authentication(bool Passed);

private:

    /**
     * @brief The socket identifier
     */
    int m_SocketID;

    /**
     * @brief The socket
     */
    QTcpSocket* m_Socket;

    /**
     * @brief The system process
     */
    QProcess* m_Shell;

    /*!
    * The buffer for incomming data
    */
    QString m_Buffer;

    /*!
    * Determines if the client is authenticated
    */
    bool m_Authenticated;

    /**
     * @brief Handles any errors
     * @param message
     */
    void handleError(QString message);

    /**
     * @brief Sends the command prompt
     */
    void sendPrompt();

};

#endif // TELNETCLIENT_H
