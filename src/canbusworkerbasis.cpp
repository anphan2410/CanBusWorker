#include "canbusworkerbasis.h"

CanBusWorkerBasis::CanBusWorkerBasis(QObject *parent) : AbstractStateMachineBasis(parent)
{
    registerGlobalSignal;
    qRegisterMetaType<QCanBusFrame>("QCanBusFrame");
    qRegisterMetaType<QCanBusDevice::CanBusDeviceState>("QCanBusDevice::CanBusDeviceState");
    qRegisterMetaType<QCanBusDevice::CanBusError>("QCanBusDevice::CanBusError");
    anIf(CanBusWorkerBasisDbgEn, anAck("CanBusWorkerBasis Constructed !"));
}

CanBusWorkerBasis::~CanBusWorkerBasis()
{
    anIf(CanBusWorkerBasisDbgEn, anWarn("Destroy CanBusWorkerBasis"));
    dispose();    
}

void CanBusWorkerBasis::uninitiatedCanBusWorkerOnEntry()
{
    if (!isInitiated)
    {
        initiate();
    }
}

void CanBusWorkerBasis::idleCanBusWorkerOnEntry()
{
    if (previousStateName == QStringLiteral("uninitiatedCanBusWorker"))
    {
        GlobalSignal iamReady;
        iamReady.Type = QVariant::fromValue(readyToWork);
        iamReady.Data = QVariant::fromValue(parent()->objectName());
        iamReady.TimeStamp = NOW2String;
        iamReady.DstStrs.append(GlobalSignalCoordinatorObjName);
        iamReady.SignalPriority = 200;
        pushAGlobalSignalIntoPrioritizedBuffer(iamReady);
        emit GlobalSignalExecutionRequested();
    }
}

void CanBusWorkerBasis::runningCanBusWorkerOnEntry()
{
    clearCache();
    if (prioritizedBuffer.size())
    {
        currentGlobalSignal = popMostPrioritizedGlobalSignalOutOfPrioritizedBuffer();
        QString currentGlobalSignalTypeTypeName = currentGlobalSignal.Type.typeName();
        if (currentGlobalSignalTypeTypeName == QStringLiteral("CanBusWorkerBasis::Data"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case replyFrameWithTimeStamp:
            {
                anIf(CanBusWorkerBasisDbgEn, anAck("replyFrameWithTimeStamp"));
                emit Out(GlobalSignal(currentGlobalSignal));
                break;
            }
            case requestFrameTransmission:
            {
                anIf(CanBusWorkerBasisDbgEn, anAck("requestFrameTransmission"));
                lastFrameWritten = currentGlobalSignal.Data.value<QCanBusFrame>();
                emit writingFrame();
                return;
            }
            case clearBuffer:
            {
                anIf(CanBusWorkerBasisDbgEn, anWarn("clearBuffer"));
                clearPrioritizedBuffer();
                break;
            }
            default:
                break;
            }
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("CanBusWorkerBasis::Notification"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case readyToWork:
            {
                anIf(CanBusWorkerBasisDbgEn, anAck("readyToWork"));
                emit Out(GlobalSignal(currentGlobalSignal));
                break;
            }
            case FrameReceived:
            {
                anIf(CanBusWorkerBasisDbgEn, anAck("FrameReceived"));
                collectFramesReceived();
                break;
            }
            default:
                break;
            }
        }
    }
    if (prioritizedBuffer.isEmpty())
    {
        emit goIdle();
    }
    else
    {
        emit GlobalSignalExecutionRequested();
    }
}

void CanBusWorkerBasis::errorCanBusWorkerOnEntry()
{
    anIf(CanBusWorkerBasisDbgEn,
         anError("Emit CanBusWorkerBasis::Error");
         anInfo("ErrorType: " + QString(CanBusWorkerBasis::ErrorMetaEnum.valueToKey(static_cast<int>(ErrorType))));
         anInfo("ErrorInfo: " + ErrorInfo);
    );
    GlobalSignal errorGlobalSignal;
    errorGlobalSignal.Type = QVariant::fromValue(ErrorType);
    errorGlobalSignal.Data = QVariant::fromValue(ErrorInfo);
    errorGlobalSignal.Priority = 200;
    errorGlobalSignal.SignalPriority = 200;
    errorGlobalSignal.DstStrs.append(GlobalSignalCoordinatorObjName);
    emit Out(errorGlobalSignal);
}

void CanBusWorkerBasis::initiate()
{
    dispose();
    QString * DeviceCreationErrorString = nullptr;
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
    currentDev = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                          QStringLiteral("can0"),
                                                          DeviceCreationErrorString);
#else
    currentDev = QCanBus::instance()->createDevice(QByteArray("socketcan"),
                                                          QStringLiteral("can0"));
#endif
    if (currentDev)
    {
        anIf(CanBusWorkerBasisDbgEn,
            anAck("Device Created !");
            QObject::connect(currentDev, &QCanBusDevice::stateChanged, this, [&](QCanBusDevice::CanBusDeviceState devState){
                anWarn("Device State Changed To " << QCanBusDeviceStateMetaEnum.valueToKey(static_cast<int>(devState)));
            });)
        QObject::connect(currentDev, &QCanBusDevice::errorOccurred, this, [&](QCanBusDevice::CanBusError devError){
            setError(DeviceError,QCanBusErrorMetaEnum.valueToKey(static_cast<int>(devError)));
        });
        QObject::connect(currentDev, &QCanBusDevice::framesReceived, this, [&](){
            GlobalSignal queueFramesRead;
            queueFramesRead.Type = QVariant::fromValue(FrameReceived);
            pushAGlobalSignalIntoPrioritizedBuffer(queueFramesRead);
            if (currentStateName == QStringLiteral("idleCanBusWorker"))
            {
                emit GlobalSignalExecutionRequested();
            }
        });
        QObject::connect(currentDev, &QCanBusDevice::framesWritten, this, &CanBusWorkerBasis::FramesWritten,
                         uniqueQtConnectionType);
        if (currentDev->connectDevice())
        {
            isInitiated = true;
            emit goIdle();
        }
        else
        {
            anIf(CanBusWorkerBasisDbgEn, anError("Failed To Connect Device !"));
        }
    }
    else
    {
        setError(DeviceCreationFailed,DeviceCreationErrorString?*DeviceCreationErrorString:QStringLiteral());
    }
    delete DeviceCreationErrorString;
    DeviceCreationErrorString = nullptr;
    anIf(CanBusWorkerBasisDbgEn && isInitiated, anAck("CanBusWorkerBasis Initiated !"));
}

void CanBusWorkerBasis::dispose()
{
    anIf(CanBusWorkerBasisDbgEn && isInitiated, anWarn("Clean CanBusWorkerBasis !"));
    currentStateName.clear();
    previousStateName.clear();
    clearPrioritizedBuffer();
    clearCache();
    clearError();
    if (currentDev)
    {        
        currentDev->disconnectDevice();
        delete currentDev;
        currentDev = nullptr;
    }
    isInitiated = false;
}

void CanBusWorkerBasis::clearCache()
{
    currentGlobalSignal = GlobalSignal();
    lastFrameWritten = QCanBusFrame();
}

void CanBusWorkerBasis::setError(const CanBusWorkerBasis::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType!=NoError)
    {
        anIf(CanBusWorkerBasisDbgEn, anError("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void CanBusWorkerBasis::clearError()
{
    anIf(CanBusWorkerBasisDbgEn && (ErrorType!=NoError), anWarn("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
}

void CanBusWorkerBasis::collectFramesReceived()
{
    if (currentDev->framesAvailable())
    {
        GlobalSignal pendingReplyFrameWithTimeStamp;
        pendingReplyFrameWithTimeStamp.Type = QVariant::fromValue(replyFrameWithTimeStamp);
        pendingReplyFrameWithTimeStamp.DstStrs.append(CanDataProcessorObjName);
        while (currentDev->framesAvailable())
        {
            pendingReplyFrameWithTimeStamp.Data = QVariant::fromValue(currentDev->readFrame());
            pendingReplyFrameWithTimeStamp.TimeStamp = NOW2String;
            pushAGlobalSignalIntoPrioritizedBuffer(pendingReplyFrameWithTimeStamp);
        }
    }
}

void CanBusWorkerBasis::In(const GlobalSignal &aGlobalSignal)
{
    pushAGlobalSignalIntoPrioritizedBuffer(aGlobalSignal);
    if (currentStateName == QStringLiteral("idleCanBusWorker"))
    {
        emit GlobalSignalExecutionRequested();
    }
}

const QMetaEnum CanBusWorkerBasis::DataMetaEnum = QMetaEnum::fromType<CanBusWorkerBasis::Data>();
const QMetaEnum CanBusWorkerBasis::ErrorMetaEnum = QMetaEnum::fromType<CanBusWorkerBasis::Error>();
const QMetaEnum CanBusWorkerBasis::WarningMetaEnum = QMetaEnum::fromType<CanBusWorkerBasis::Warning>();
const QMetaEnum CanBusWorkerBasis::NotificationMetaEnum = QMetaEnum::fromType<CanBusWorkerBasis::Notification>();
const QMetaEnum CanBusWorkerBasis::QCanBusErrorMetaEnum = QMetaEnum::fromType<QCanBusDevice::CanBusError>();
const QMetaEnum CanBusWorkerBasis::QCanBusDeviceStateMetaEnum = QMetaEnum::fromType<QCanBusDevice::CanBusDeviceState>();
