#ifndef FRAMESENT_H
#define FRAMESENT_H

#include <QSignalTransition>
#include "canbusworkerdb.h"

class FrameSent : public QSignalTransition
{
public:
    FrameSent(CanBusWorkerDB * database);
protected:
    void onTransition(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
};

#endif // FRAMESENT_H
