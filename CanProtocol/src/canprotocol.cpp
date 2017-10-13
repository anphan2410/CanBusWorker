#include "canprotocol.h"

CanProtocol::CanProtocol()
{
    anIf(CanPtcDbgEn,
           anTrk("Object Constructed"));
}

CanProtocol::CanProtocol(const quint8 &identifier, const QByteArray &data)
    :QCanBusFrame(identifier,data)
{
    //This part of code causes segmentation fault on Windows,
    //but run fine on pi.
    //Reason is unidentified.
    #ifndef __anWINOS__
        anIf(CanPtcDbgEn,
               anTrk("Object Constructed")
               anInfo("Id/hex="<<QByteArray::number(frameId(),16))
               anInfo("Data/hex="<<payload().toHex()));
    #endif
}

CanProtocol::CanProtocol(const QCanBusFrame &CanMsg)
    :QCanBusFrame(CanMsg.frameId(),CanMsg.payload())
{
    anIf(CanPtcDbgEn,
           anTrk("Object Imported")
           anInfo("Id/hex="<<QByteArray::number(frameId(),16))
           anInfo("Data/hex="<<payload().toHex()));
}

void CanProtocol::operator=(const QCanBusFrame &CanMsg)
{
    setFrameId(CanMsg.frameId());
    setPayload(CanMsg.payload());
    anIf(CanPtcDbgEn,
           anTrk("Object Assigned")
           anInfo("Id/hex="<<QByteArray::number(frameId(),16))
           anInfo("Data/hex="<<payload().toHex()));
}

bool CanProtocol::IsOfRightFormat() const
{
    if ((payload().size()==8) && ((frameId()>>8)==0))
    {
        anIf(CanPtcDbgEn,anTrk("OK Valid Format"));
        return true;
    }
    anIf(CanPtcDbgEn,anTrk("Invalid Format"));
    return false;
}

CanProtocol &CanProtocol::setSdcsId(const quint8 &sdcsid)
{
    setFrameId((frameId()&0x0f)|(sdcsid<<4));
    anIf(CanPtcDbgEn,
           anInfo("NEW Id/hex="<<QByteArray::number(frameId(),16)));
    return *this;
}

CanProtocol &CanProtocol::setChId(const quint8 &chid)
{
    setFrameId((frameId()&0xf0)|chid);
    anIf(CanPtcDbgEn,
           anInfo("NEW Id/hex="<<QByteArray::number(frameId(),16)));
    return *this;
}

CanProtocol &CanProtocol::setRFID(const QByteArray &rfid)
{
    QByteArray QBArrTmp(4-rfid.size(), '\0');
    QBArrTmp << rfid;
    quint8 QInt8Tmp = payload().size();
    if (QInt8Tmp >= 8)
        setPayload(payload().replace(QInt8Tmp-8,4,QBArrTmp));
    else
    {
        if (QInt8Tmp >= 5)
        {
            setPayload(payload().remove(0,QInt8Tmp-4));
            if (QInt8Tmp < 5)
                setPayload(QByteArray(payload().size(),'\0'));
        }
        setPayload(payload().prepend(QBArrTmp));
    }
    anIf(CanPtcDbgEn,
           anInfo("NEW Data/hex="<<payload().toHex()));
    return *this;
}

CanProtocol &CanProtocol::setValveControl(const bool &SetBit, const bool &ResetBit)
{
    quint8 QInt8Tmp = payload().size()-1;
    quint8 AByte = payload().at(QInt8Tmp);
    if (SetBit)
        AByte|=0x02;
    else
        AByte&=0xfd;
    if (ResetBit)
        AByte|=0x01;
    else
        AByte&=0xfe;
    setPayload(payload().remove(QInt8Tmp,1)<<AByte);
    anIf(CanPtcDbgEn,
           anInfo("NEW Data/hex="<<payload().toHex()));
    return *this;
}

CanProtocol &CanProtocol::setValveOn()
{
    return setValveControl(true,false);
}

CanProtocol &CanProtocol::setValveOff()
{
    return setValveControl(false,true);
}

CanProtocol &CanProtocol::makeDataRequest()
{
    this->setChId(7).setPayload(".");
    return *this;
}

CanProtocol &CanProtocol::makePresenceResponse()
{
    this->setChId(15).setPayload(".");
    return *this;
}

const CanProtocol CanProtocol::getDataRequest()
{
    CanProtocol tmpReturn=*this;
    tmpReturn.makeDataRequest();
    return tmpReturn;
}

const CanProtocol CanProtocol::getPresenceResponse()
{
    CanProtocol tmpReturn=*this;
    tmpReturn.makePresenceResponse();
    return tmpReturn;
}

const QCanBusFrame &CanProtocol::getMsg() const
{
    return *this;
}

const QString CanProtocol::getMsgStr() const
{
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
    return getMsg().toString();
#else
    QString IdStr=QString::number(frameId(),16);
    IdStr=QString("").fill(' ',8-IdStr.size())+IdStr;
    QString PayloadStr;
    for (quint8 i=0;i<payload().size();++i)
    {
        PayloadStr+=QStringLiteral(" ");
        PayloadStr+=payload().mid(i,1).toHex();
    }
    return IdStr + QStringLiteral("   [") + QString::number(payload().size())
            + QStringLiteral("]  ") + PayloadStr.trimmed().toUpper();
#endif
}

const QString CanProtocol::getMsgType() const
{
    QString tmpReturn;
    if (payload().size()>1)
        tmpReturn=QStringLiteral("Receive");
    else if (payload().size()==1)
    {
        if (getChId()==15)
            tmpReturn=QStringLiteral("Receive");
        else
            tmpReturn=QStringLiteral("Transmit");
    }
    else
        tmpReturn=QStringLiteral("Unknown");
    return tmpReturn;
}

const QString CanProtocol::getMsgMean() const
{
    quint8 tmpInt = getChId();
    QString tmpReturn;
    if (payload().size()>1)
        tmpReturn=QStringLiteral("Data response ")+QString::number(tmpInt+1);
    else if (payload().size()==1)
    {
        if (tmpInt==15)
            tmpReturn=QStringLiteral("Presence response");
        else if (tmpInt==0)
            tmpReturn=QStringLiteral("Presence request");
        else if (tmpInt==7)
            tmpReturn=QStringLiteral("Data request");
    }
    else
        tmpReturn=QStringLiteral("Unknown");
    return tmpReturn;
}

quint8 CanProtocol::getSdcsId() const
{
    return (frameId()&0xf0)>>4;
}

quint8 CanProtocol::getChId() const
{
    return frameId()&0x0f;
}

const QByteArray CanProtocol::getRFID() const
{
    quint8 QInt8Tmp = payload().size();
    if (QInt8Tmp>=8)
        return payload().mid(QInt8Tmp-8,4);
    else
        return QByteArray(4,'\0');
}

quint8 CanProtocol::getValveControl() const
{
    return payload().at(payload().size()-1)&0x03;
}

quint8 CanProtocol::getValveStatus() const
{
    return (payload().at(payload().size()-1)&0x0c)>>2;
}

bool CanProtocol::getValveControlSetBit() const
{
    return payload().at(payload().size()-1)&0x01;
}

bool CanProtocol::getValveControlResetBit() const
{
    return payload().at(payload().size()-1)&0x02;
}

const CanProtocol &CanProtocol::PresenceRequest = CanProtocol(0xf0,".");

const CanProtocol CanProtocol::DataRequest(const quint8 &sdcsid)
{
    CanProtocol tmpReturn;
    return tmpReturn.setSdcsId(sdcsid).makeDataRequest();
}

const CanProtocol CanProtocol::PresenceResponse(const quint8 &sdcsid)
{
    CanProtocol tmpReturn;
    return tmpReturn.setSdcsId(sdcsid).makePresenceResponse();
}
