#ifndef COMMONTHINGS_H
#define COMMONTHINGS_H

#include <QHash>
#include <QByteArray>
#include <QVariant>
#include "anLogger/src/anlogger.h"

inline QByteArray &operator <<(QByteArray &QBArr, const quint8 Data)
{
    QBArr.append(Data);
    return QBArr;
}

inline QByteArray &operator <<(QByteArray &QBArr, const quint16 Data)
{
    return QBArr<<quint8(Data>>8)<<quint8(Data);
}

inline QByteArray &operator <<(QByteArray &QBArr, const quint32 Data)
{
    return QBArr<<quint16(Data>>16)<<quint16(Data);
}

inline QByteArray &operator <<(QByteArray &QBArr, const quint64 Data)
{
    return QBArr<<quint32(Data>>32)<<quint32(Data);
}

inline QByteArray &operator <<(QByteArray &QBArr, const QByteArray &Data)
{
    QBArr.append(Data);
    return QBArr;
}

template <typename TN>
inline QHash<TN, QString> SwapKeyValOnOneToOneQHash(const QHash<QString, TN> &AQHashKeyValSet)
{
    QHash<TN, QString> tmp;
    QString tmp2 = "";
    auto KeyItr = AQHashKeyValSet.keyBegin();
    for (; KeyItr!=AQHashKeyValSet.keyEnd(); KeyItr++)
    {
        tmp2 = *KeyItr;
        tmp.insert(AQHashKeyValSet.value(tmp2), tmp2);
    }
    return tmp;
}

inline quint8 XORofAllBytesInQByteArr(const QByteArray &QBArr)
{
    if (!(QBArr.isNull() || QBArr.isEmpty()))
    {
        QByteArray::const_iterator ConstItr = QBArr.constBegin();
        quint8 IntTmp = *(ConstItr++);
        for (;ConstItr!=QBArr.cend(); ConstItr++)
        {
            IntTmp ^=*ConstItr;
        }
        return IntTmp;
    }
    else
        return 0;
}

///
/// \brief IntStr2QBArr0Pad
/// \param Num
/// \param SizeInByte
/// \return QByteArray contains a hex number
///         representing an integer number encoded by Ascii code
///
inline QByteArray IntStr2QBArr0Pad(const quint32 Num, const quint8 SizeInByte)
{
    QString QStrTmp = QString::number(Num);
    return QStrTmp.prepend(QString("").fill('0',SizeInByte-QStrTmp.size())).toLocal8Bit();
}

///
///\brief GlobalSignal
///
typedef qint16 TypeOfGlobalSignalContentPriority;
typedef qint16 TypeOfGlobalSignalPriority;
extern const TypeOfGlobalSignalContentPriority maxGlobalSignalContentPriority;
extern const TypeOfGlobalSignalContentPriority minGlobalSignalContentPriority;
extern const TypeOfGlobalSignalPriority maxGlobalSignalPriority;
extern const TypeOfGlobalSignalPriority minGlobalSignalPriority;
typedef struct
{
    QVariant Type;
    QVariant Data;
    QString TimeStamp;
    QString Key = QStringLiteral("NULL");
    QList<QString> DstStrs;
    TypeOfGlobalSignalContentPriority Priority = 0;
    TypeOfGlobalSignalPriority SignalPriority = 0;
} GlobalSignal;
Q_DECLARE_METATYPE(GlobalSignal)

#define registerGlobalSignal qRegisterMetaType<GlobalSignal>("GlobalSignal");

extern const Qt::ConnectionType uniqueQtConnectionType;

#define NOW2String QDateTime::currentDateTime().toString(Qt::ISODate);

extern const QString piLocalDBWorkerObjName;
extern const QString UHV2SerialPortWorkerObjName;
extern const QString UHV4SerialPortWorkerObjName;
extern const QString UHV2SerialPortDataProcessorObjName;
extern const QString UHV4SerialPortDataProcessorObjName;
extern const QString CanBusWorkerObjName;
extern const QString CanDataProcessorObjName;
extern const QString GlobalSignalCoordinatorObjName;

#endif // COMMONTHINGS_H
