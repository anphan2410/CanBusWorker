#include "waitforerrorhandler.h"

waitForErrorHandler::waitForErrorHandler(CanBusWorkerDB *database, quint32 TimerIntervalInMiliSecond) :
    TimerIntervalMSecs(TimerIntervalInMiliSecond),
    dbPtr(database)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Constructed !"));
    if (TimerIntervalInMiliSecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMiliSecond);
        QObject::connect(&timer, &QTimer::timeout,
                         this, [database](){
            anIf(CanBusWorkerDBDbgEn,
                anError("Emit <currentCanBusWorkerDB>->Error");
                anInfo("ErrorType : " +
                         QString(CanBusWorkerDB::ErrorMetaEnum.valueToKey(static_cast<int>(database->ErrorType))));
                anInfo("ErrorInfo : " + database->ErrorInfo);
            );
            emit database->Out(QVariant::fromValue(database->ErrorType),
                               QVariant::fromValue(database->ErrorInfo));
        }, CanBusWorkerDB::uniqueQtConnectionType);
    }
}

void waitForErrorHandler::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Entered !"));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void waitForErrorHandler::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave State !"));
    if (TimerIntervalMSecs > 0)
        timer.stop();
    dbPtr->clearError();
}
