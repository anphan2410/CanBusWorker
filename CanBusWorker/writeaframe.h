#ifndef WRITEAFRAME_H
#define WRITEAFRAME_H

#include <QState>
#include <QTimer>
#include "canbusworkerdb.h"

class writeAFrame : public QState
{
public:
    writeAFrame(QState * parent, CanBusWorkerDB * database, QTimer * timerFrameWrite);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanBusWorkerDB * dbPtr = Q_NULLPTR;
    QTimer * timerFrameWritten = Q_NULLPTR;
};

#endif // WRITEAFRAME_H
