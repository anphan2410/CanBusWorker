#ifndef CONNECTDEVICE_H
#define CONNECTDEVICE_H

#include <QState>
#include "canbusworkerdb.h"

class connectDevice : public QState
{
public:
    connectDevice(CanBusWorkerDB * database);
protected:
    void onEntry(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
};

#endif // CONNECTDEVICE_H
