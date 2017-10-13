#include "writeframecanbusworker.h"

writeFrameCanBusWorker::writeFrameCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisPtr(parentBasis)
{
    setObjectName(QStringLiteral("writeFrameCanBusWorker"));
    timer.setParent(this);
    timer.setInterval(3000);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [parentBasis](){
        parentBasis->setError(CanBusWorkerBasis::FrameWrittenTimedOut,QStringLiteral(""));
    });
    anIf(CanBusWorkerBasisDbgEn, anAck("writeFrameCanBusWorker Constructed"));
}

void writeFrameCanBusWorker::onEntry(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("writeFrameCanBusWorker Entered"));
    qApp->processEvents();
    basisPtr->currentDev->writeFrame(basisPtr->lastFrameWritten);
    timer.start();
}

void writeFrameCanBusWorker::onExit(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("Leave writeFrameCanBusWorker"));
    timer.stop();
}
