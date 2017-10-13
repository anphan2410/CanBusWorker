#include "directtransition.h"

directTransition::directTransition(const QObject *aSender, const char *aSignal, QAbstractState *aTarget) :
    QSignalTransition(aSender,aSignal)
{
    this->setTargetState(aTarget);
}

bool directTransition::eventTest(QEvent *e)
{
    if (!(QSignalTransition::eventTest(e) && this->targetState()))
        return false;
    QStateMachine::SignalEvent * se = static_cast<QStateMachine::SignalEvent *>(e);
    return (se->arguments().at(0).toString() == this->targetState()->objectName());
}
