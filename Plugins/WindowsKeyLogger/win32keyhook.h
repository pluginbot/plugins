#ifndef WIN32KEYHOOK_H
#define WIN32KEYHOOK_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <Windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include "singleton.h"
#include "logevent.h"

/**
 * @brief The global object for the key hook
 */
static HHOOK mHook;

class Win32KeyHook : public QObject
{
    Q_OBJECT
public:
    explicit Win32KeyHook(QObject *parent = 0);

    /**
     * @brief Start listening to key strokes
     */
    void start();

    /**
     * @brief Stop listening to keystrokes
     */
    void stop();

    /**
     * @brief Handle an error
     * @param error
     */
    void handleError(QString error);

signals:

    /**
     * @brief Emitted when a key is pressed
     * @param event
     */
    void keyPress(LogEvent* event);

public slots:

private:

    /**
    * The callback function for the hook
    * @param nCode  Used to determine how to process the hook information
    * @param wParam WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP
    * @param lParam The KBDLLHOOKSTRUCT
    */
    static LRESULT CALLBACK  KeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam);

};

/**
 * @brief Create a global single instance
 */
typedef Singleton<Win32KeyHook> KeyHook;

#endif // WIN32KEYHOOK_H
