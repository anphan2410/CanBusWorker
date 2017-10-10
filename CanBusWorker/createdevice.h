#ifndef CREATEDEVICE_H
#define CREATEDEVICE_H

#include <QState>
#include "canbusworkerdb.h"

class createDevice : public QState
{
public:
    createDevice(CanBusWorkerDB * database);
protected:
    void onEntry(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
};

#endif // CREATEDEVICE_H
