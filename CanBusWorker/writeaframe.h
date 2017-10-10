#ifndef WRITEAFRAME_H
#define WRITEAFRAME_H

#include <QState>
#include <QTimer>
#include "canbusworkerdb.h"

class writeAFrame : public QState
{
public:
    writeAFrame(QState * parent, CanBusWorkerDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
    QTimer TimerFrameWritten;
};

#endif // WRITEAFRAME_H
