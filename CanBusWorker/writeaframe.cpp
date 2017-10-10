#include "writeaframe.h"

writeAFrame::writeAFrame(QState *parent, CanBusWorkerDB *database) :
    QState(parent), dbPtr(database)
{
    TimerFrameWritten.setParent(this);
    TimerFrameWritten.setInterval(3000);
    TimerFrameWritten.setSingleShot(true);
    QObject::connect(&TimerFrameWritten, &QTimer::timeout, this, [&](){
        dbPtr->setError(CanBusWorkerDB::FrameWrittenTimedOut, QStringLiteral(""));
    });
    anIf(CanBusWorkerDBDbgEn, anTrk("Sub-State Constructed !"));
}

void writeAFrame::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Sub-State Entered !"));
    if (dbPtr->lastFrameTransmitted)
    {
        delete dbPtr->lastFrameTransmitted;
        dbPtr->lastFrameTransmitted = Q_NULLPTR;
    }
    if (dbPtr->pendingFrameList.size())
    {
        dbPtr->lastFrameTransmitted = new QCanBusFrame(dbPtr->pendingFrameList.takeFirst());
        dbPtr->currentDev->writeFrame(*(dbPtr->lastFrameTransmitted));
        TimerFrameWritten.start();
    }
}

void writeAFrame::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave Sub-State !"));
    TimerFrameWritten.stop();
}
