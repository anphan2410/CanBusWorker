#ifndef WRITEFRAME_H
#define WRITEFRAME_H

#include <QState>
#include <QFinalState>
#include <QTimer>
#include "canbusworkerdb.h"
#include "framesent.h"
#include "writeaframe.h"

class writeFrame : public QState
{
public:
    writeFrame(CanBusWorkerDB * database);
protected:
    void onEntry(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
    QTimer TimerFrameWritten;
};

#endif // WRITEFRAME_H
