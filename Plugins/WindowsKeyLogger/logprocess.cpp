#include "logprocess.h"

LogProcess::LogProcess(QObject *parent) :
    QObject(parent)
{
}

LogProcess::~LogProcess()
{
    qDeleteAll(events);
}
