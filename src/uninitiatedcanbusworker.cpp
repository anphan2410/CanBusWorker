#include "uninitiatedcanbusworker.h"

uninitiatedCanBusWorker::uninitiatedCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("uninitiatedCanBusWorker"));
    anIf(CanBusWorkerBasisDbgEn, anAck("uninitiatedCanBusWorker Constructed"));
}

void uninitiatedCanBusWorker::onEntry(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("uninitiatedCanBusWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    if (!(basisptr->isInitiated))
    {
        basisptr->initialize();
    }
}

void uninitiatedCanBusWorker::onExit(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("Leave uninitiatedCanBusWorker"));
    basisptr->previousStateName = objectName();
}
