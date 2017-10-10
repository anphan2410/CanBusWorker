#include "deviceconnected.h"

DeviceConnected::DeviceConnected(CanBusWorkerDB *database) :
    QSignalTransition(database, &CanBusWorkerDB::DeviceStateChanged),
    dbPtr(database)
{

}

bool DeviceConnected::eventTest(QEvent *e)
{
    if (!QSignalTransition::eventTest(e))
        return false;
    QStateMachine::SignalEvent *se = static_cast<QStateMachine::SignalEvent*>(e);
    return (se->arguments().at(0).toInt() == QCanBusDevice::ConnectedState);
}

void DeviceConnected::onTransition(QEvent *)
{
    emit dbPtr->Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::DeviceConnected)));
}
