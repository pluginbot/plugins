#include "operatingsystem.h"

OperatingSystem::OperatingSystem(QObject *parent) :
    QObject(parent)
{

}

QString OperatingSystem::getOperatingSystem()
{
    try
    {
        #ifdef Q_OS_LINUX
            return "linux";
        #endif

        #ifdef Q_OS_MAC
            return "mac";
        #endif

        #ifdef Q_OS_WIN
            return "windows";
        #endif

        #ifdef Q_OS_SOLARIS
            return "solaris";
        #endif


        #ifdef Q_OS_ANDROID
            return "android";
        #endif

        #ifdef Q_OS_IOS
           return "ios";
        #endif

        return "unknown";

    }
    catch(...)
    {
        return "error";
    }
}

int OperatingSystem::getProcessorBits()
{
    try
    {
        return QSysInfo::WordSize;
    }
    catch(...)
    {
       return 0;
    }
}

bool OperatingSystem::isWindows()
{
#ifdef Q_OS_WIN
    return true;
#endif

    return false;
}

bool OperatingSystem::isMac()
{
#ifdef Q_OS_MAC
    return true;
#endif

    return false;
}

bool OperatingSystem::isAndroid()
{
#ifdef Q_OS_ANDROID
    return true;
#endif

    return false;
}

bool OperatingSystem::isIOS()
{
#ifdef Q_OS_IOS
    return true;
#endif

    return false;
}

bool OperatingSystem::isLinux()
{
#ifdef Q_OS_LINUX
    return true;
#endif

    return false;
}

bool OperatingSystem::isSolaris()
{
#ifdef Q_OS_SOLARIS
    return true;
#endif

    return false;
}

bool OperatingSystem::isUbuntu()
{
#ifdef Q_OS_LINUX

    QByteArray var = qgetenv("XDG_CURRENT_DESKTOP");

    QString os(var);
    if(os.toLower() == "unity") return true;
    return false;
#endif

    return false;
}
