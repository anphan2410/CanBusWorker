#include "frameiswritten.h"

FrameIsWritten::FrameIsWritten(CanBusWorkerBasis *parentBasis, QAbstractState *target) :
    QSignalTransition(parentBasis, &CanBusWorkerBasis::FramesWritten),
    basisptr(parentBasis)
{
    setTargetState(target);
}

void FrameIsWritten::onTransition(QEvent *)
{
    if (basisptr->lastFrameWritten.frameId() != 0 || basisptr->lastFrameWritten.payload().size())
    {
        GlobalSignal notifyFrameWritten;
        notifyFrameWritten.Type = QVariant::fromValue(CanBusWorkerBasis::FrameWritten);
        notifyFrameWritten.Data = QVariant::fromValue(basisptr->lastFrameWritten);
        notifyFrameWritten.TimeStamp = NOW2String;
        notifyFrameWritten.DstStrs.append(SmallCoordinatorObjName);
        anIf(CanBusWorkerBasisDbgEn, anAck("Frame Written !");
        #if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
                anInfo(basisptr->lastFrameWritten.toString());
        #else
                anInfo("" + QString().setNum(basisptr->lastFrameWritten.frameId(), 16)
                         + " | " + basisptr->lastFrameWritten.payload().toHex());
        #endif
        );
        emit basisptr->Out(notifyFrameWritten);
        basisptr->isCurrentRunningCycleCompleted = true;
    }
}
