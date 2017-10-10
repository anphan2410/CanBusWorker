#ifndef DEVICECONNECTED_H
#define DEVICECONNECTED_H

#include <QSignalTransition>
#include <QStateMachine>
#include <QAbstractState>
#include "canbusworkerdb.h"

class DeviceConnected : public QSignalTransition
{
public:
    DeviceConnected(CanBusWorkerDB * database);
protected:
    bool eventTest(QEvent *e) override;
    void onTransition(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
};

#endif // DEVICECONNECTED_H
