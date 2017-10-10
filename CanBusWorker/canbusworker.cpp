#include "canbusworker.h"

CanBusWorker::CanBusWorker(QObject *parent) : QStateMachine(parent)
{
    anIf(CanBusWorkerDbgEn, anTrk("Object Constructed !"));
    currentDB = new CanBusWorkerDB(this);

    waitForPluginAndInterface * state0 = new waitForPluginAndInterface(currentDB,1000);
    state0->setObjectName(QStringLiteral("waitForPluginAndInterface"));
    createDevice * state1 = new createDevice(currentDB);
    state1->setObjectName(QStringLiteral("createDevice"));
    connectDevice * state2 = new connectDevice(currentDB);
    state2->setObjectName(QStringLiteral("connectDevice"));
    readFrame * state3 = new readFrame(currentDB);
    state3->setObjectName(QStringLiteral("readFrame"));
    writeFrame * state4 = new writeFrame(currentDB);
    state4->setObjectName(QStringLiteral("writeFrame"));
    waitForErrorHandler * state7 = new waitForErrorHandler(currentDB,1000);
    state7->setObjectName(QStringLiteral("waitForErrorHandler"));

    DeviceConnected * state2Tostate3 = new DeviceConnected(currentDB);
    state2Tostate3->setTargetState(state3);

    state0->addTransition(currentDB, &CanBusWorkerDB::PluginAndInterfaceChanged, state1);
    state1->addTransition(new directTransition(currentDB, state2));
    state1->addTransition(currentDB, &CanBusWorkerDB::ErrorOccurred, state7);
    state2->addTransition(currentDB, &CanBusWorkerDB::PluginAndInterfaceChanged, state1);
    state2->addTransition(currentDB, &CanBusWorkerDB::ErrorOccurred, state7);
    state2->addTransition(state2Tostate3);
    state3->addTransition(currentDB, &CanBusWorkerDB::PluginAndInterfaceChanged, state1);
    state3->addTransition(currentDB, &CanBusWorkerDB::ErrorOccurred, state7);
    state3->addTransition(currentDB, &CanBusWorkerDB::aFrameAdded, state4);
    state3->addTransition(currentDB, &CanBusWorkerDB::FrameReceived, state3);
    state4->addTransition(currentDB, &CanBusWorkerDB::PluginAndInterfaceChanged, state1);
    state4->addTransition(currentDB, &CanBusWorkerDB::ErrorOccurred, state7);
    state4->addTransition(currentDB, &CanBusWorkerDB::aFrameAdded, state4);
    state4->addTransition(currentDB, &CanBusWorkerDB::FrameReceived, state3);
    state7->addTransition(currentDB, &CanBusWorkerDB::freshRetry, state0);
    state7->addTransition(currentDB, &CanBusWorkerDB::PluginAndInterfaceChanged, state1);
    state7->addTransition(currentDB, &CanBusWorkerDB::reconnectDevice, state2);
    state7->addTransition(currentDB, &CanBusWorkerDB::retry, state3);

    this->addState(state0);
    this->addState(state1);
    this->addState(state2);
    this->addState(state3);
    this->addState(state4);
    this->addState(state7);
    this->setInitialState(state0);
    this->setErrorState(state7);

    QObject::connect(currentDB, &CanBusWorkerDB::Out, this, &CanBusWorker::Out,
                     CanBusWorkerDB::uniqueQtConnectionType);
}

CanBusWorker::~CanBusWorker()
{
    if (currentDB)
    {
        delete currentDB;
        currentDB = Q_NULLPTR;
    }
    anIf(CanBusWorkerDbgEn, anTrk("Object Destroyed !"));
}

void CanBusWorker::In(QVariant enumVar, QVariant dataVar)
{    
    if (this->isRunning())
    {
        anIf(CanBusWorkerDbgEn, anTrk("Signal-To-CanBusWorker Received !"));
        QString enumVarTypeName(enumVar.typeName());
        if (enumVarTypeName == QStringLiteral("CanBusWorkerDB::Data"))
        {
            switch (enumVar.toInt()) {
            case CanBusWorkerDB::requestPluginAndInterface:
            {
                anIf(CanBusWorkerDbgEn, anInfo("requestPluginAndInterface"));
                emit Out(QVariant::fromValue(CanBusWorkerDB::requestPluginAndInterface),
                         QVariant::fromValue(currentDB->currentPluginAndInterface));
                break;
            }
            case CanBusWorkerDB::replyPluginAndInterface:
            {
                anIf(CanBusWorkerDbgEn, anInfo("replyPluginAndInterface"));
                currentDB->currentPluginAndInterface = dataVar.value<CanBusWorkerDB::PluginNameAndInterfaceName>();
                emit currentDB->PluginAndInterfaceChanged();
                break;
            }
            case CanBusWorkerDB::clearPendingFrameList:
            {
                anIf(CanBusWorkerDbgEn, anInfo("clearPendingFrameList"));
                currentDB->pendingFrameList.clear();
                break;
            }
            case CanBusWorkerDB::addAFrameIntoPendingFrameList:
            {
                anIf(CanBusWorkerDbgEn, anInfo("addAFrameIntoPendingFrameList"));
                currentDB->pendingFrameList.append(dataVar.value<QCanBusFrame>());
                emit currentDB->aFrameAdded();
                break;
            }
            default:
                break;
            }
        }
    }
}
