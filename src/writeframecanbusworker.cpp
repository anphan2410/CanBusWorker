#include "writeframecanbusworker.h"

writeFrameCanBusWorker::writeFrameCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("writeFrameCanBusWorker"));
    timer.setParent(this);
    timer.setInterval(3000);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [parentBasis](){
        parentBasis->isCurrentRunningCycleCompleted = true;
        parentBasis->setError(CanBusWorkerBasis::FrameWrittenTimedOut,QStringLiteral(""));
    });
    anIf(CanBusWorkerBasisDbgEn, anAck("writeFrameCanBusWorker Constructed"));
}

void writeFrameCanBusWorker::onEntry(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("writeFrameCanBusWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->currentDev->writeFrame(basisptr->lastFrameWritten);
    timer.start();
}

void writeFrameCanBusWorker::onExit(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("Leave writeFrameCanBusWorker"));
    timer.stop();
    basisptr->previousStateName = objectName();
}
