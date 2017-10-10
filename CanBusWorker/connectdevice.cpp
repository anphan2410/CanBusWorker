#include "connectdevice.h"

connectDevice::connectDevice(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Constructed !"));
}

void connectDevice::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Entered !"));
    if (dbPtr->currentDev->connectDevice())
    {
        anIf(CanBusWorkerDBDbgEn, anAck("Successfully Connect Device !"));
    }
    else
    {
        anIf(CanBusWorkerDBDbgEn, anError("Failed To Connect Device !"));
    }
}

void connectDevice::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave State !"));
}
