#ifndef CANPROTOCOL_H
#define CANPROTOCOL_H
#define CanPtcDbgEn 0

#include <QCanBusFrame>
#include <QByteArray>
#include "anlogger.h"
#include "commonthings.h"


class CanProtocol : public QCanBusFrame
{
public:

    explicit CanProtocol();
    explicit CanProtocol(quint8 identifier, const QByteArray &data);
    explicit CanProtocol(const QCanBusFrame &CanMsg);

    void operator=(const QCanBusFrame &CanMsg);

    bool IsOfRightFormat() const;

    CanProtocol &setSdcsId(const quint8 sdcsid);
    CanProtocol &setChId(const quint8 chid);
    CanProtocol &setRFID(const QByteArray &rfid);
    CanProtocol &setValveControl(const bool SetBit,const bool ResetBit);
    CanProtocol &setValveOn();
    CanProtocol &setValveOff();
    CanProtocol &makeDataRequest();
    CanProtocol &makePresenceResponse();

    const QCanBusFrame &getMsg() const;
    const QString getMsgStr() const;

    const QString getMsgType() const;
    const QString getMsgMean() const;

    quint8 getSdcsId() const;
    quint8 getChId() const;
    const QByteArray getRFID() const;
    quint8 getValveControl() const;
    quint8 getValveStatus() const;


    static const CanProtocol PresenceRequest;
    static const CanProtocol &DataRequest(const quint8 sdcsid);
    static const CanProtocol &PresenceResponse(const quint8 sdcsid);
};

#endif // CANPROTOCOL_H
