#ifndef WRITEFRAMECANBUSWORKER_H
#define WRITEFRAMECANBUSWORKER_H

#include "canbusworkerbasis.h"
#include <QTimer>

class writeFrameCanBusWorker : public QState
{
public:
    writeFrameCanBusWorker(CanBusWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanBusWorkerBasis * basisptr = nullptr;
    QTimer timer;
};

#endif // WRITEFRAMECANBUSWORKER_H
