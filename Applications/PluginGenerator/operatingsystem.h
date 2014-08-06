#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include <QObject>
#include <QSysInfo>

/**
 * @brief Helpful functions for os specifics
 */
class OperatingSystem : public QObject
{
    Q_OBJECT
public:
    explicit OperatingSystem(QObject *parent = 0);

    /**
     * @brief Returns a string containing the name of the os
     * @return QString
     */
    static QString getOperatingSystem();

    /**
     * @brief Get the processor bit size
     * @return int
     */
    static int getProcessorBits();

    /**
     * @brief Determines if the OS is Windows
     * @return boolean
     */
    static bool isWindows();

    /**
     * @brief Determines if the OS is MAC
     * @return boolean
     */
    static bool isMac();

    /**
     * @brief Determines if the OS is Android
     * @return boolean
     */
    static bool isAndroid();

    /**
     * @brief Determines if the OS is IOS
     * @return boolean
     */
    static bool isIOS();

    /**
     * @brief Determines if the OS is Linux
     * @return boolean
     */
    static bool isLinux();

    /**
     * @brief Determines if the OS is Solaris
     * @return boolean
     */
    static bool isSolaris();

    /**
     * @brief Determines if the Desktop is Ubuntu
     * @return boolean
     */
    static bool isUbuntu();
signals:

public slots:

};

#endif // OPERATINGSYSTEM_H
