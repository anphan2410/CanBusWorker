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
    if (basisptr->previousStateName == QStringLiteral("uninitiatedCanBusWorker"))
    {
        GlobalSignal iamReady;
        iamReady.Type = QVariant::fromValue(CanBusWorkerBasis::readyToWork);
        iamReady.Data = QVariant::fromValue(machine()->objectName());
        iamReady.TimeStamp = NOW2String;
        iamReady.DstStrs.append(SmallCoordinatorObjName);
        iamReady.SignalPriority = 200;
        basisptr->addAGlobalSignal(iamReady);
    }
}

void idleCanBusWorker::onExit(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("Leave idleCanBusWorker"));
    basisptr->previousStateName = objectName();
}
