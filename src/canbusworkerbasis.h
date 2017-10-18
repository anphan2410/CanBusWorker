#ifndef CANBUSWORKERBASIS_H
#define CANBUSWORKERBASIS_H

#define CanBusWorkerBasisDbgEn 1

#include "shared/abstractstatemachinebasis.h"
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>

class CanBusWorkerBasis : public AbstractStateMachineBasis
{
    Q_OBJECT
public:
    explicit CanBusWorkerBasis(QObject *parent = nullptr);
    ~CanBusWorkerBasis();

    enum Data
    {
        NoData = 0,
        requestFrameTransmission,
        replyFrameWithTimeStamp,
        clearBuffer
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0,
        DeviceCreationFailed,
        DeviceError,
        FrameWrittenTimedOut
    };
    Q_ENUM(Error)

    enum Warning
    {
        NoWarning = 0
    };
    Q_ENUM(Warning)

    enum Notification
    {
        NoNotification = 0,
        readyToWork,
        FrameWritten,
        FrameReceived
    };
    Q_ENUM(Notification)

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;
    static const QMetaEnum QCanBusErrorMetaEnum;
    static const QMetaEnum QCanBusDeviceStateMetaEnum;

    QCanBusDevice * currentDev = nullptr;
    Error ErrorType = NoError;
    QString ErrorInfo;

    //Cache
    QCanBusFrame lastFrameWritten;

    void initialize();
    void dispose();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearError();

    void executePrioritizedBuffer();
    void collectFramesReceived();
    void emitErrorGlobalSignal();
    void queueNotificationReadyToWork();
signals:
    void FramesWritten();
    void writingFrame();
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    void clearCache();
};

Q_DECLARE_METATYPE(QCanBusFrame)
Q_DECLARE_METATYPE(QCanBusDevice::CanBusDeviceState)
Q_DECLARE_METATYPE(QCanBusDevice::CanBusError)

#endif // CANBUSWORKERBASIS_H
