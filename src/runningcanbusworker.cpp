#include "runningcanbusworker.h"

runningCanBusWorker::runningCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("runningCanBusWorker"));
    anIf(CanBusWorkerBasisDbgEn, anAck("runningCanBusWorker Constructed"));
}

void runningCanBusWorker::onEntry(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("runningCanBusWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->runningCanBusWorkerOnEntry();
}

void runningCanBusWorker::onExit(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("Leave runningCanBusWorker"));
    basisptr->previousStateName = objectName();
}
