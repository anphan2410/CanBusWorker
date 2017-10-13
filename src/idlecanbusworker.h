#ifndef IDLECANBUSWORKER_H
#define IDLECANBUSWORKER_H

#include "canbusworkerbasis.h"

class idleCanBusWorker : public QState
{
public:
    idleCanBusWorker(CanBusWorkerBasis *parentBasis,QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanBusWorkerBasis * basisptr = nullptr;
};

#endif // IDLECANBUSWORKER_H
