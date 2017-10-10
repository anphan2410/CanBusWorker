#include "connectdevice.h"

connectDevice::connectDevice(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anAck("Construct A New State !");
}

void connectDevice::onEntry(QEvent *)
{
    anAck("Enter State ...");
    if (dbPtr->currentDev->connectDevice())
    {
        anInfo("Successfully Connect Device !");
    }
    else
    {
        anInfo("Failed To Connect Device !");
    }
}
