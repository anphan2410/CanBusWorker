#ifndef ERRORCANBUSWORKER_H
#define ERRORCANBUSWORKER_H

#include "canbusworkerbasis.h"

class errorCanBusWorker : public QState
{
public:
    errorCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanBusWorkerBasis *basisptr = nullptr;
};

#endif // ERRORCANBUSWORKER_H
