#ifndef WINDOWSINFO_H
#define WINDOWSINFO_H

#include <QObject>
#include <QDebug>
#include <Windows.h>
#include <psapi.h>
#include <tlhelp32.h>


class WindowsInfo : public QObject
{
    Q_OBJECT
public:
    explicit WindowsInfo(QObject *parent = 0);

    /**
     * @brief Returns the title of the active window
     * @param window
     * @return The title of the window
     */
    QString getWindowTitle(HWND window);


    /**
     * @brief Get the active window
     * @return
     */
    HWND getActiveWindow();

    /**
     * @brief Get the active window process ID
     * @return
     */
    DWORD getWindowProcess(HWND window);

    /**
     * @brief Returns the name of the active window
     * @param process
     * @return The window name
     */
    QString getWindowName(DWORD process);

    /**
     * @brief Get the account the window is running as
     * @param process
     * @return
     */
    QString getWindowOwner(DWORD process);

signals:

public slots:

private:

    /**
     * @brief Handle an error
     * @param error
     */
    void handleError(QString error);

};

#endif // WINDOWSINFO_H
