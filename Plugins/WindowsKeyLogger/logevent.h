#ifndef LOGEVENT_H
#define LOGEVENT_H

#include <QObject>
#include <QDateTime>
#include <windows.h>

/**
 * @brief holds information about a key press event
 */
class LogEvent : public QObject
{
    Q_OBJECT
public:
    explicit LogEvent(QObject *parent = 0);

    /**
     * @brief Determines if the shift key was pressed
     */
    short key_shift;

    /**
     * @brief Determines if the caps lock was on
     */
    short key_capslock;

    /**
     * @brief Determines if the control key was pressed
     */
    short key_control;

    /**
     * @brief Determines if the alt key was pressed
     */
    short key_alt;

    /**
     * @brief The actual key value
     */
    int value;

    /**
     * @brief The key text
     */
    QString text;

    /**
     * @brief The date and time of the keystroke
     */
    QDateTime date;

signals:

public slots:

};

#endif
