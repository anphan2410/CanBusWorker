#include "canbusworkerdb.h"

CanBusWorkerDB::CanBusWorkerDB(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QCanBusFrame>("QCanBusFrame");
    qRegisterMetaType<QCanBusDevice::CanBusDeviceState>("QCanBusDevice::CanBusDeviceState");
    qRegisterMetaType<QCanBusDevice::CanBusError>("QCanBusDevice::CanBusError");
    qRegisterMetaType<CanBusWorkerDB::CanBusFrameWithTimeStamp>("CanBusWorkerDB::CanBusFrameWithTimeStamp");
    qRegisterMetaType<CanBusWorkerDB::PluginNameAndInterfaceName>("CanBusWorkerDB::PluginNameAndInterfaceName");
}

CanBusWorkerDB::~CanBusWorkerDB()
{
    sweepGarbage();
}

void CanBusWorkerDB::initialize()
{
    anAck("CanBusWorkerDB Initialized !");
    sweepGarbage();
    ErrorType = new Error(NoError);
    ErrorInfo = new QString("");
    pendingFrameList = new QList<QCanBusFrame*>();
}

void CanBusWorkerDB::sweepGarbage()
{
    if (currentPluginAndInterface)
        delete currentPluginAndInterface;
    if (currentDev)
        delete currentDev;
    if (ErrorType)
        delete ErrorType;
    if (ErrorInfo)
        delete ErrorInfo;
    if (lastFrameTransmitted)
        delete lastFrameTransmitted;
    if (pendingFrameList)
        delete pendingFrameList;
    currentPluginAndInterface = Q_NULLPTR;
    currentDev = Q_NULLPTR;
    ErrorType = Q_NULLPTR;
    ErrorInfo = Q_NULLPTR;
    lastFrameTransmitted = Q_NULLPTR;
    pendingFrameList = Q_NULLPTR;
}

void CanBusWorkerDB::setError(const Error &anErrorType, const QString &anErrorInfo)
{
    *ErrorType = anErrorType;
    *ErrorInfo = anErrorInfo;
    emit ErrorOccurred();
}

void CanBusWorkerDB::clearError()
{
    *ErrorType = NoError;
    ErrorInfo->clear();
}

const QMetaEnum CanBusWorkerDB::ErrorMetaEnum = QMetaEnum::fromType<CanBusWorkerDB::Error>();
const QMetaEnum CanBusWorkerDB::QCanBusErrorMetaEnum = QMetaEnum::fromType<QCanBusDevice::CanBusError>();
