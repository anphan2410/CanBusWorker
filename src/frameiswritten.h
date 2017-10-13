#ifndef FRAMEISWRITTEN_H
#define FRAMEISWRITTEN_H

#include <QSignalTransition>
#include "canbusworkerbasis.h"

class FrameIsWritten : public QSignalTransition
{
public:
    FrameIsWritten(CanBusWorkerBasis *parentBasis, QAbstractState *target);
protected:
    void onTransition(QEvent *) override;
private:
    CanBusWorkerBasis * basisptr = nullptr;
};

#endif // FRAMEISWRITTEN_H
