#ifndef CANBUSWORKER_H
#define CANBUSWORKER_H

#define CanBusWorkerDbgEn 1

#include <QStateMachine>
#include "canbusworkerbasis.h"
#include "errorcanbusworker.h"
#include "frameiswritten.h"
#include "idlecanbusworker.h"
#include "runningcanbusworker.h"
#include "writeframecanbusworker.h"
#include "uninitiatedcanbusworker.h"
#include "shared/directtransition.h"

class CanBusWorker : public QStateMachine
{
    Q_OBJECT
public:
    explicit CanBusWorker(QObject *parent = 0);
signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    CanBusWorkerBasis * currentBasis = nullptr;
};

#endif // CANBUSWORKER_H
