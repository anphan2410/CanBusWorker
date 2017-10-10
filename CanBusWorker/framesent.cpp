#include "framesent.h"

FrameSent::FrameSent(CanBusWorkerDB * database) :
    QSignalTransition(database, &CanBusWorkerDB::FrameWritten),
    dbPtr(database)
{

}

void FrameSent::onTransition(QEvent *)
{
    if (dbPtr->lastFrameTransmitted)
    {
        anIf(CanBusWorkerDBDbgEn, anAck("Successfully Send A Frame !");
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
        anInfo(dbPtr->lastFrameTransmitted->toString());
#else
        anInfo("" + QString().setNum(dbPtr->lastFrameTransmitted->frameId(), 16)
                 + " | " + dbPtr->lastFrameTransmitted->payload().toHex());
#endif
        );
        emit dbPtr->Out(QVariant::fromValue(CanBusWorkerDB::CanFrameTransmitted),
                        QVariant::fromValue(*(dbPtr->lastFrameTransmitted)));
        delete dbPtr->lastFrameTransmitted;
        dbPtr->lastFrameTransmitted = Q_NULLPTR;
    }
}
