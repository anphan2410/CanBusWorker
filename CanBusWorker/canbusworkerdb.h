#ifndef CANBUSWORKERDB_H
#define CANBUSWORKERDB_H

#define CanBusWorkerDBDbgEn 1

#include <QCoreApplication>
#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QMetaEnum>
#include <QPair>
#include <QMap>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include "anlogger.h"

class CanBusWorkerDB : public QObject
{
    Q_OBJECT
public:
    explicit CanBusWorkerDB(QObject *parent = 0);
    ~CanBusWorkerDB();

    enum Data
    {
        NoData = 0,
        replyCanFrameWithTimeStamp,
        replyPluginAndInterface,
        clearPendingFrameList,
        addAFrameIntoPendingFrameList,
        setConfigurationParameter,
        requestPluginAndInterface
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
        DeviceCreated,
        DeviceConnected,
        CanFrameTransmitted
    };
    Q_ENUM(Notification)

    typedef QPair<QCanBusFrame,QString> CanBusFrameWithTimeStamp;
    typedef QPair<QString,QString> PluginNameAndInterfaceName;

    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearError();

    PluginNameAndInterfaceName currentPluginAndInterface;
    QCanBusDevice * currentDev = Q_NULLPTR;
    Error ErrorType = NoError;
    QString ErrorInfo;
    QCanBusFrame * lastFrameTransmitted = Q_NULLPTR;
    QList<QCanBusFrame> pendingFrameList;

    void initialize();
    void dispose();

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;
    static const QMetaEnum QCanBusErrorMetaEnum;
    static const Qt::ConnectionType uniqueQtConnectionType = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);

signals:
    void Out(QVariant , QVariant = QVariant());
    void PluginAndInterfaceChanged();
    void directTransitionRequest(const QString &);
    void ErrorOccurred();
    void aFrameAdded();
    void freshRetry();
    void reconnectDevice();
    void retry();
    void FrameReceived();
    void FrameWritten();
    void DeviceStateChanged(QCanBusDevice::CanBusDeviceState);
    void DeviceConfigurationParameterSet();
public slots:
    void DeviceErrorOccurred(QCanBusDevice::CanBusError error);
};

Q_DECLARE_METATYPE(QCanBusFrame)
Q_DECLARE_METATYPE(QCanBusDevice::CanBusDeviceState)
Q_DECLARE_METATYPE(QCanBusDevice::CanBusError)
Q_DECLARE_METATYPE(CanBusWorkerDB::CanBusFrameWithTimeStamp)
Q_DECLARE_METATYPE(CanBusWorkerDB::PluginNameAndInterfaceName)

#endif // CANBUSWORKERDB_H
