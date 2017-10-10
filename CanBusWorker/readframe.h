#ifndef READFRAME_H
#define READFRAME_H

#include <QState>
#include <QTime>
#include "canbusworkerdb.h"

class readFrame : public QState
{
public:
    readFrame(CanBusWorkerDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
};

#endif // READFRAME_H
