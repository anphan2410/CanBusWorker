#ifndef CANBUSWORKER_H
#define CANBUSWORKER_H

#include <QStateMachine>
#include "canbusworkerdb.h"
#include "waitforpluginandinterface.h"
#include "createdevice.h"
#include "connectdevice.h"
#include "readframe.h"
#include "writeframe.h"
#include "waitforerrorhandler.h"
#include "directtransition.h"
#include "deviceconnected.h"

class CanBusWorker : public QStateMachine
{
    Q_OBJECT
public:
    explicit CanBusWorker(QObject *parent = 0);
    ~CanBusWorker();
signals:
    void Out(QVariant *, QVariant * = Q_NULLPTR);
public slots:
    void In(QVariant * enumVar, QVariant * dataVar = Q_NULLPTR);
private:
    CanBusWorkerDB * currentDB = Q_NULLPTR;
};

#endif // CANBUSWORKER_H
