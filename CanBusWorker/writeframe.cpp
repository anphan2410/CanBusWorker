#include "writeframe.h"

writeFrame::writeFrame()
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Constructed !"));
}

void writeFrame::onEntry(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("State Entered !"));
}

void writeFrame::onExit(QEvent *)
{
    anIf(CanBusWorkerDBDbgEn, anTrk("Leave State !"));
}
