#ifndef CANBUSWORKER_H
#define CANBUSWORKER_H

#define CanBusWorkerDbgEn 1

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
    void Out(QVariant, QVariant = QVariant());
public slots:
    void In(QVariant enumVar, QVariant dataVar = QVariant());
private:
    CanBusWorkerDB * currentDB = Q_NULLPTR;
};

#endif // CANBUSWORKER_H
