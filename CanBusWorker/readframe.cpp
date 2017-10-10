#include "readframe.h"

readFrame::readFrame(CanBusWorkerDB *database) :
    dbPtr(database)
{
    anAck("Construct A New State");
}

void readFrame::onEntry(QEvent *)
{
    anAck("Enter State ...");
    while (dbPtr->currentDev->framesAvailable())
    {
        QCanBusFrame tmpFirst = dbPtr->currentDev->readFrame();
        qApp->processEvents();
        CanBusWorkerDB::CanBusFrameWithTimeStamp tmp(&tmpFirst,
                                                     new QString(QTime::currentTime().toString(QStringLiteral("hh:mm:ss"))));
        emit dbPtr->Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::replyCanFrameWithTimeStamp)),
                        new QVariant(QVariant::fromValue(tmp)));
    }
}
