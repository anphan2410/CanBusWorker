#include "idlecanbusworker.h"

idleCanBusWorker::idleCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("idleCanBusWorker"));
    anIf(CanBusWorkerBasisDbgEn, anAck("idleCanBusWorker Constructed"));
}

void idleCanBusWorker::onEntry(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("idleCanBusWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->idleCanBusWorkerOnEntry();
}

void idleCanBusWorker::onExit(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("Leave idleCanBusWorker"));
    basisptr->previousStateName = objectName();
}
