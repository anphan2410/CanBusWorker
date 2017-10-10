#ifndef WAITFORERRORHANDLER_H
#define WAITFORERRORHANDLER_H

#include <QState>
#include <QTimer>
#include "canbusworkerdb.h"

class waitForErrorHandler : public QState
{
public:
    waitForErrorHandler(CanBusWorkerDB * database, quint32 TimerIntervalInMiliSecond = 0);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    quint32 TimerIntervalMSecs = 0;
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
};

#endif // WAITFORERRORHANDLER_H
