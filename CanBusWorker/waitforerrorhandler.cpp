#include "waitforerrorhandler.h"

waitForErrorHandler::waitForErrorHandler(CanBusWorkerDB *database, quint32 TimerIntervalInMiliSecond) :
    TimerIntervalMSecs(TimerIntervalInMiliSecond),
    dbPtr(database)
{
    anAck("Construct A New State !");
    if (TimerIntervalInMiliSecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMiliSecond);
        QObject::connect(&timer, &QTimer::timeout,
                         this, [database](){
            anAck("Emit <currentCanBusWorkerDB>->ErrorType");
            anInfo("ErrorType : " +
                     QString(CanBusWorkerDB::ErrorMetaEnum.valueToKey(static_cast<int>(*(database->ErrorType)))));
            anInfo("ErrorInfo : " + *(database->ErrorInfo));
            emit database->Out(new QVariant(QVariant::fromValue(*(database->ErrorType))),
                               new QVariant(QVariant::fromValue(*(database->ErrorInfo))));
        }, CanBusWorkerDB::uniqueQtConnectionType);
    }
}

void waitForErrorHandler::onEntry(QEvent *)
{
    anAck("Enter State ...");
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void waitForErrorHandler::onExit(QEvent *)
{
    if (TimerIntervalMSecs > 0)
        timer.stop();
    dbPtr->clearError();
}
