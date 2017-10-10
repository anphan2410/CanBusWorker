#include "writeframe.h"

writeFrame::writeFrame(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Constructed !"));
    TimerFrameWritten.setParent(this);
    TimerFrameWritten.setInterval(3000);
    TimerFrameWritten.setSingleShot(true);
    QObject::connect(&TimerFrameWritten, &QTimer::timeout, this, [&](){
        dbPtr->setError(CanBusWorkerDB::FrameWrittenTimedOut, QStringLiteral(""));
    });
    writeAFrame * substate = new writeAFrame(this,database,&TimerFrameWritten);
    QFinalState * done = new QFinalState(this);
    FrameSent * signalFrameSent = new FrameSent(database);
    signalFrameSent->setTargetState(done);
    substate->addTransition(signalFrameSent);
    this->setInitialState(substate);
}

void writeFrame::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Entered !"));
}

void writeFrame::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave State !"));
}
