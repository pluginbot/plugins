#include "windowsinfo.h"

WindowsInfo::WindowsInfo(QObject *parent) :
    QObject(parent)
{
    try
    {

    }
    catch(QString error)
    {
        handleError(error);
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
    }
}

QString WindowsInfo::getWindowTitle(HWND window)
{
    try
    {
        char lpszTitle[255] = {0};

        if(GetWindowText(window,(LPTSTR)lpszTitle,255)) return QString::fromUtf16((ushort*)lpszTitle);

        return "";
    }
    catch(QString error)
    {
        handleError(error);
        return "";
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

HWND WindowsInfo::getActiveWindow()
{
    try
    {
        HWND hHandle = GetForegroundWindow();
        if(GetLastError() > 0) return 0;

        return hHandle;
    }
    catch(QString error)
    {
        handleError(error);
        return 0;
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return 0;
    }
}

DWORD WindowsInfo::getWindowProcess(HWND window)
{
    try
    {
        DWORD dwPID;
        GetWindowThreadProcessId(window,&dwPID);
        if(GetLastError() > 0) return 0;

        return dwPID;
    }
    catch(QString error)
    {
        handleError(error);
        return 0;
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return 0;
    }
}

QString WindowsInfo::getWindowName(DWORD process)
{
    try
    {
        //Get a list of running processes and match it to the process id
        HANDLE hProcessSnap;
        PROCESSENTRY32 pe32;

        // Take a snapshot of all processes in the system.
        hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
        if(GetLastError() > 0) throw QString("Could not get process list");

        //Get the first process
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if( !Process32First(hProcessSnap, &pe32))
        {
            // clean the snapshot object
            CloseHandle( hProcessSnap );
            if(GetLastError() > 0) return "";
        }

        do
        {
            if(pe32.th32ProcessID == process)
            {
                CloseHandle( hProcessSnap );
                return QString::fromWCharArray(pe32.szExeFile);
            }
        } while( Process32Next( hProcessSnap, &pe32 ) );

        // clean the snapshot object
        CloseHandle( hProcessSnap );

        return "";
    }
    catch(QString error)
    {
        handleError(error);
        return "";
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

QString WindowsInfo::getWindowOwner(DWORD process)
{
    try
    {

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, process);
        if(GetLastError() > 0) throw QString("Could not open the process");

        HANDLE hProcessToken;
        if(OpenProcessToken(hProcess,TOKEN_READ,&hProcessToken))
        {
            if(GetLastError() > 0) throw QString("Could not open process token");
            //http://stackoverflow.com/questions/3912117/get-a-process-owner-name-in-mfc
            //https://www.google.com/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=C%2B%2B+process+owner+example

            // First get size needed, TokenUser indicates we want user information from given token
              DWORD dwProcessTokenInfoAllocSize = 0;
              GetTokenInformation(hProcessToken, TokenUser, NULL, 0, &dwProcessTokenInfoAllocSize);
              //TOKEN_USER pUserToken = reinterpret_cast<ptoken_user>(new BYTE[dwProcessTokenInfoAllocSize]);
              PTOKEN_USER pUserToken = reinterpret_cast<PTOKEN_USER>( new BYTE[dwProcessTokenInfoAllocSize] );

              if (pUserToken != NULL)
              {
                  // Now get user information in the allocated buffer
                  if (GetTokenInformation(hProcessToken, TokenUser, pUserToken, dwProcessTokenInfoAllocSize, &dwProcessTokenInfoAllocSize ))
                  {
                      // Some vars that we may need
                      SID_NAME_USE   snuSIDNameUse;
                      WCHAR          szUser[MAX_PATH] = { 0 };
                      DWORD          dwUserNameLength = MAX_PATH;
                      WCHAR          szDomain[MAX_PATH] = { 0 };
                      DWORD          dwDomainNameLength = MAX_PATH;

                      if ( LookupAccountSid( NULL,pUserToken->User.Sid,szUser,&dwUserNameLength,szDomain,&dwDomainNameLength,&snuSIDNameUse ))
                      {

                          QString user = QString::fromWCharArray(szDomain) + "\\" + QString::fromWCharArray(szUser);
                          //qDebug() << "User: " << QString::fromWCharArray(szUser); //<< QString::fromWCharArray(szUser);
                          //qDebug() << "Domain: " << QString::fromWCharArray(szDomain);

                          CloseHandle( hProcessToken );
                          delete [] pUserToken;
                          return user;
                      }


                  }
                  else
                  {
                     // qDebug() << "We did not get the token";
                  }

              }
              else
              {
                  //qDebug() << "Token was null!";
              }

        }
        else
        {
            qDebug() << "ERROR: " << GetLastError();
        }

        CloseHandle(hProcess);

        return "";
    }
    catch(QString error)
    {
        handleError(error);
        return "";
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return "";
    }
}

void WindowsInfo::handleError(QString error)
{
    qCritical() << error;
}
