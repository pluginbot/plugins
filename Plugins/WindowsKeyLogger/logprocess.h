#ifndef LOGPROCESS_H
#define LOGPROCESS_H

#include <QObject>
#include <QList>
#include "logevent.h"

/**
 * @brief Hold information about the process
 */
class LogProcess : public QObject
{
    Q_OBJECT
public:
    explicit LogProcess(QObject *parent = 0);

    ~LogProcess();

    /**
     * @brief The HWND of the process
     */
    int handle;

    /**
     * @brief The process ID
     */
    int process;

    /**
     * @brief The name of the process
     */
    QString name;

    /**
     * @brief the user account
     */
    QString user;

    /**
     * @brief the window title
     */
    QString title;

    /**
     * @brief The key log for the process
     */
    QList<LogEvent*> events;

signals:

public slots:

};

#endif
