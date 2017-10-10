#include "canbusworkerdb.h"

CanBusWorkerDB::CanBusWorkerDB(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QCanBusFrame>("QCanBusFrame");
    qRegisterMetaType<QCanBusDevice::CanBusDeviceState>("QCanBusDevice::CanBusDeviceState");
    qRegisterMetaType<QCanBusDevice::CanBusError>("QCanBusDevice::CanBusError");
    qRegisterMetaType<CanBusWorkerDB::CanBusFrameWithTimeStamp>("CanBusWorkerDB::CanBusFrameWithTimeStamp");
    qRegisterMetaType<CanBusWorkerDB::PluginNameAndInterfaceName>("CanBusWorkerDB::PluginNameAndInterfaceName");
    anIf(CanBusWorkerDBDbgEn, anTrk("Object Constructed !"));
}

CanBusWorkerDB::~CanBusWorkerDB()
{
    dispose();
    anIf(CanBusWorkerDBDbgEn, anTrk("Object Destroyed !"));
}

void CanBusWorkerDB::setError(const Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType!=NoError)
    {
        anIf(CanBusWorkerDBDbgEn, anError("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void CanBusWorkerDB::clearError()
{
    anIf(CanBusWorkerDBDbgEn && (ErrorType!=NoError), anInfo("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
}

void CanBusWorkerDB::initialize()
{
    dispose();
    ErrorInfo.clear();
    currentPluginAndInterface.first.clear();
    currentPluginAndInterface.second.clear();
    if (pendingFrameList.size())
        pendingFrameList.clear();
    clearError();
    anIf(CanBusWorkerDBDbgEn, anTrk("Object Initialized !"));
}

void CanBusWorkerDB::dispose()
{
    if (currentDev)
    {
        delete currentDev;
        currentDev = Q_NULLPTR;
    }
    if (lastFrameTransmitted)
    {
        delete lastFrameTransmitted;
        lastFrameTransmitted = Q_NULLPTR;
    }
    anIf(CanBusWorkerDBDbgEn && currentDev, anTrk("Object Cleaned !"));
}

void CanBusWorkerDB::DeviceErrorOccurred(QCanBusDevice::CanBusError error)
{
    setError(CanBusWorkerDB::DeviceError,CanBusWorkerDB::QCanBusErrorMetaEnum.valueToKey(static_cast<int>(error)));
}

const QMetaEnum CanBusWorkerDB::DataMetaEnum = QMetaEnum::fromType<CanBusWorkerDB::Data>();
const QMetaEnum CanBusWorkerDB::ErrorMetaEnum = QMetaEnum::fromType<CanBusWorkerDB::Error>();
const QMetaEnum CanBusWorkerDB::WarningMetaEnum = QMetaEnum::fromType<CanBusWorkerDB::Warning>();
const QMetaEnum CanBusWorkerDB::NotificationMetaEnum = QMetaEnum::fromType<CanBusWorkerDB::Notification>();
const QMetaEnum CanBusWorkerDB::QCanBusErrorMetaEnum = QMetaEnum::fromType<QCanBusDevice::CanBusError>();
