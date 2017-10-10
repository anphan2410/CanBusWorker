#include "writeaframe.h"

writeAFrame::writeAFrame(QState *parent, CanBusWorkerDB *database, QTimer *timerFrameWrite) :
    QState(parent), dbPtr(database), timerFrameWritten(timerFrameWrite)
{
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
        timerFrameWritten->start();
    }
}

void writeAFrame::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave Sub-State !"));
    timerFrameWritten->stop();
}
