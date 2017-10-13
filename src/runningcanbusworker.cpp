#include "runningcanbusworker.h"

runningCanBusWorker::runningCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisPtr(parentBasis)
{
    setObjectName(QStringLiteral("runningCanBusWorker"));
    anIf(CanBusWorkerBasisDbgEn, anAck("runningCanBusWorker Constructed"));
}

void runningCanBusWorker::onEntry(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("runningCanBusWorker Entered"));
    qApp->processEvents();
    basisPtr->executePrioritizedBuffer();
    if (basisPtr->prioritizedBuffer.size())
    {
        emit basisPtr->requestDirectTransition(QStringLiteral("runningCanBusWorker"));
    }
    else
    {
        emit basisPtr->requestDirectTransition(QStringLiteral("idleCanBusWorker"));
    }
}

void runningCanBusWorker::onExit(QEvent *)
{
    anIf(CanBusWorkerBasisDbgEn, anTrk("Leave runningCanBusWorker"));
    basisPtr->previousStateName = objectName();
}
