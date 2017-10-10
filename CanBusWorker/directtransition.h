#ifndef DIRECTTRANSITION_H
#define DIRECTTRANSITION_H

#include <QSignalTransition>
#include <QAbstractState>
#include <QStateMachine>
#include "canbusworkerdb.h"

class directTransition : public QSignalTransition
{
public:
    directTransition(CanBusWorkerDB * database, QAbstractState * destinationState);
protected:
    bool eventTest(QEvent *e) override;
};

#endif // DIRECTTRANSITION_H
