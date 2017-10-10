#include "canbusworker.h"

CanBusWorker::CanBusWorker(QObject *parent) : QStateMachine(parent)
{
    anAck("Construct A New CanBusWorker");
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
    QObject::connect(this, &CanBusWorker::started,
                     currentDB, &CanBusWorkerDB::initialize,
                     CanBusWorkerDB::uniqueQtConnectionType);
}

CanBusWorker::~CanBusWorker()
{
    delete currentDB;
    currentDB = Q_NULLPTR;
}

void CanBusWorker::In(QVariant *enumVar, QVariant *dataVar)
{
    if (this->isRunning())
    {
        anAck("A Signal Received !");
        QString enumVarTypeName(enumVar->typeName());
        if (enumVarTypeName == QStringLiteral("CanBusWorkerDB::DataGet"))
        {
            anAck("" + enumVarTypeName + " Parsed !");
            switch (enumVar->toInt()) {
            case CanBusWorkerDB::requestPluginAndInterface:
            {
                anInfo("requestPluginAndInterface");
                emit Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::requestPluginAndInterface)),
                         new QVariant(QVariant::fromValue(*(currentDB->currentPluginAndInterface))));
                break;
            }
            default:
                break;
            }
        }
        else if (enumVarTypeName == QStringLiteral("CanBusWorkerDB::DataSet"))
        {
            anAck("" + enumVarTypeName + " Parsed !");
            switch (enumVar->toInt()) {
            case CanBusWorkerDB::replyPluginAndInterface:
            {
                anInfo("replyPluginAndInterface");
                currentDB->currentPluginAndInterface = new CanBusWorkerDB::PluginNameAndInterfaceName(
                            dataVar->value<CanBusWorkerDB::PluginNameAndInterfaceName>());
                emit currentDB->PluginAndInterfaceChanged();
                break;
            }
            case CanBusWorkerDB::clearPendingFrameList:
            {
                anInfo("clearPendingFrameList");
                currentDB->pendingFrameList->clear();
                break;
            }
            case CanBusWorkerDB::addAFrameIntoPendingFrameList:
            {
                anInfo("addAFrameIntoPendingFrameList");
                QCanBusFrame * tmp = new QCanBusFrame();
                *tmp = dataVar->value<QCanBusFrame>();
                currentDB->pendingFrameList->append(tmp);
                emit currentDB->aFrameAdded();
                break;
            }
            default:
                break;
            }
        }
    }
    delete enumVar;
    enumVar = Q_NULLPTR;
    if (dataVar)
    {
        delete dataVar;
        dataVar = Q_NULLPTR;
    }
}
