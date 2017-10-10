#include "createdevice.h"

createDevice::createDevice(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anAck("Construct A New State !");
}

void createDevice::onEntry(QEvent *)
{
    anAck("Enter State ...");
    if (dbPtr->currentDev)
    {
        if (dbPtr->currentDev->state() != QCanBusDevice::UnconnectedState)
        {
            dbPtr->currentDev->disconnectDevice();
        }
        delete dbPtr->currentDev;
        dbPtr->currentDev = Q_NULLPTR;
    }
    QString * ErrorString = Q_NULLPTR;
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
    dbPtr->currentDev = QCanBus::instance()->createDevice(*(dbPtr->currentPluginAndInterface->first),
                                                          *(dbPtr->currentPluginAndInterface->second),
                                                          ErrorString);
#else
    dbPtr->currentDev = QCanBus::instance()->createDevice(dbPtr->currentPluginAndInterface->first->toLocal8Bit(),
                                                          *(dbPtr->currentPluginAndInterface->second));
#endif
    if (dbPtr->currentDev)
    {
        anLogCode(
        QObject::connect(dbPtr->currentDev, &QCanBusDevice::stateChanged,
                         [&](QCanBusDevice::CanBusDeviceState state){
            anAck("DeviceStateChanged Emitted ! Current State Code : "
                    + QString::number(static_cast<int>(state)));
        });)
        QObject::connect(dbPtr->currentDev, &QCanBusDevice::errorOccurred,
                         [&](QCanBusDevice::CanBusError anErrorID) {
            *(dbPtr->ErrorInfo) = QStringLiteral("QCanBusDevice::CanBusError ")
                    + CanBusWorkerDB::QCanBusErrorMetaEnum.valueToKey(static_cast<int>(anErrorID));
            *(dbPtr->ErrorType) = CanBusWorkerDB::DeviceError;
            emit dbPtr->ErrorOccurred();
        });
        QObject::connect(dbPtr->currentDev, &QCanBusDevice::stateChanged,
                         dbPtr, &CanBusWorkerDB::DeviceStateChanged,
                         CanBusWorkerDB::uniqueQtConnectionType);
        QObject::connect(dbPtr->currentDev, &QCanBusDevice::framesReceived,
                         dbPtr, &CanBusWorkerDB::FrameReceived,
                         CanBusWorkerDB::uniqueQtConnectionType);
        QObject::connect(dbPtr->currentDev, &QCanBusDevice::framesWritten,
                         dbPtr, &CanBusWorkerDB::FrameWritten,
                         CanBusWorkerDB::uniqueQtConnectionType);
        emit dbPtr->Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::DeviceCreated)));
        emit dbPtr->directTransitionRequest(QStringLiteral("connectDevice"));
    }
    else
    {
        QString tmp("");
        if (ErrorString)
            tmp = *ErrorString;
        dbPtr->setError(CanBusWorkerDB::DeviceCreationFailed,
                        tmp);
    }
    delete ErrorString;
    ErrorString = Q_NULLPTR;
}
