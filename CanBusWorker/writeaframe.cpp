#include "writeaframe.h"

writeAFrame::writeAFrame(QState *parent, CanBusWorkerDB *database, QTimer *timerFrameWrite) :
    QState(parent), dbPtr(database), timerFrameWritten(timerFrameWrite)
{
    anAck("Construct A New Sub-State");
}

void writeAFrame::onEntry(QEvent *)
{
    anAck("Enter Sub-State ...");
    if (dbPtr->lastFrameTransmitted)
    {
        delete dbPtr->lastFrameTransmitted;
        dbPtr->lastFrameTransmitted = Q_NULLPTR;
    }
    if (!(dbPtr->pendingFrameList->isEmpty()))
    {
        dbPtr->lastFrameTransmitted = dbPtr->pendingFrameList->takeFirst();
        dbPtr->currentDev->writeFrame(*(dbPtr->lastFrameTransmitted));
        timerFrameWritten->start();
    }
}

void writeAFrame::onExit(QEvent *)
{
    timerFrameWritten->stop();
}
