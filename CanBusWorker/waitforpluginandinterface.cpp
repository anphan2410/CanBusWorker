#include "waitforpluginandinterface.h"

waitForPluginAndInterface::waitForPluginAndInterface(CanBusWorkerDB *database, quint32 TimerIntervalInMiliSecond) :
    TimerIntervalMSecs(TimerIntervalInMiliSecond)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Constructed !"));
    if (TimerIntervalInMiliSecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMiliSecond);
        QObject::connect(&timer, &QTimer::timeout,
                         this, [database](){
            anIf(CanBusWorkerDBDbgEn, anAck("Emit CanBusWorkerDB::requestPluginAndInterface"));
            emit database->Out(QVariant::fromValue(CanBusWorkerDB::requestPluginAndInterface));
        });
    }

}

void waitForPluginAndInterface::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Entered !"));
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void waitForPluginAndInterface::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave State !"));
    if (TimerIntervalMSecs > 0)
        timer.stop();
}
