#include "errorcanbusworker.h"

errorCanBusWorker::errorCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("errorCanBusWorker"));
    anIf(CanBusWorkerBasisDbgEn, anAck("errorCanBusWorker Constructed"));
}

void errorCanBusWorker::onEntry(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("errorCanBusWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->emitErrorGlobalSignal();
}

void errorCanBusWorker::onExit(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("Leave errorCanBusWorker"));
    basisptr->previousStateName = objectName();
}
