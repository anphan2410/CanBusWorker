#ifndef RUNNINGCANBUSWORKER_H
#define RUNNINGCANBUSWORKER_H

#include "canbusworkerbasis.h"

class runningCanBusWorker : public QState
{
public:
    runningCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanBusWorkerBasis * basisptr = nullptr;
};

#endif // RUNNINGCANBUSWORKER_H
