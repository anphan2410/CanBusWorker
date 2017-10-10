#ifndef WRITEFRAME_H
#define WRITEFRAME_H

#include <QState>
#include "canbusworkerdb.h"

class writeFrame : public QState
{
public:
    writeFrame();
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
};

#endif // WRITEFRAME_H
