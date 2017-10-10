#ifndef COMMONTHINGS_H
#define COMMONTHINGS_H

#include <QHash>
#include <QByteArray>

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
const QHash<TN, QString> SwapKeyValOnOneToOneQHash(const QHash<QString, TN> &AQHashKeyValset)
{
    QHash<TN, QString> &tmp = * new QHash<TN, QString>();
    QString tmp2 = "";
    auto KeyItr = AQHashKeyValset.keyBegin();
    for (; KeyItr!=AQHashKeyValset.keyEnd(); KeyItr++)
    {
        tmp2 = *KeyItr;
        tmp.insert(AQHashKeyValset.value(tmp2), tmp2);
    }
    return tmp;
}

inline static quint8 XORofAllBytesInQByteArr(const QByteArray &QBArr)
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
inline static const QByteArray IntStr2QBArr0Pad(const quint32 Num, const quint8 SizeInByte, char padChar = '0')
{
    QString QStrTmp = QString::number(Num);
    return QStrTmp.prepend(QString("").fill(padChar,SizeInByte-QStrTmp.size())).toLocal8Bit();
}


#endif // COMMONTHINGS_H
