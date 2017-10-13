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
        GlobalSignal pendingNotificationFrameWritten;
        pendingNotificationFrameWritten.Type = QVariant::fromValue(CanBusWorkerBasis::FrameWritten);
        pendingNotificationFrameWritten.Data = QVariant::fromValue(basisptr->lastFrameWritten);
        pendingNotificationFrameWritten.TimeStamp = NOW2String;
        pendingNotificationFrameWritten.DstStrs.append(SmallCoordinatorObjName);
        anIf(CanBusWorkerBasisDbgEn, anAck("Frame Written !");
        #if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
                anInfo(basisptr->lastFrameWritten.toString());
        #else
                anInfo("" + QString().setNum(basisptr->lastFrameWritten.frameId(), 16)
                         + " | " + basisptr->lastFrameWritten.payload().toHex());
        #endif
        );
        basisptr->addAGlobalSignal(pendingNotificationFrameWritten);
    }
}
