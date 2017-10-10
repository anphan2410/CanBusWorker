#include "waitforpluginandinterface.h"

waitForPluginAndInterface::waitForPluginAndInterface(CanBusWorkerDB *database, quint32 TimerIntervalInMiliSecond) :
    TimerIntervalMSecs(TimerIntervalInMiliSecond)
{
    anAck("Construct A New State !");
    if (TimerIntervalInMiliSecond > 0)
    {
        timer.setParent(this);
        timer.setInterval(TimerIntervalInMiliSecond);
        QObject::connect(&timer, &QTimer::timeout,
                         this, [database](){
            anAck("Emit CanBusWorkerDB::requestPluginAndInterface");
            emit database->Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::requestPluginAndInterface)));
        });
    }

}

void waitForPluginAndInterface::onEntry(QEvent *)
{
    anAck("Enter State ...");
    if (TimerIntervalMSecs > 0)
        timer.start();
}

void waitForPluginAndInterface::onExit(QEvent *)
{
    if (TimerIntervalMSecs > 0)
        timer.stop();
}
