#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QThread * aNewThread = new QThread();
    CanBusWorker * currentCanBusWorker = new CanBusWorker();
    currentCanBusWorker->moveToThread(aNewThread);

    QObject::connect(aNewThread, &QThread::started,
                     currentCanBusWorker, &CanBusWorker::start,
                     CanBusWorkerDB::uniqueQtConnectionType);
    QObject::connect(this, &MainWindow::Out,
                     currentCanBusWorker, &CanBusWorker::In,
                     CanBusWorkerDB::uniqueQtConnectionType);
    QObject::connect(currentCanBusWorker, &CanBusWorker::Out,
                     this, &MainWindow::In,
                     CanBusWorkerDB::uniqueQtConnectionType);

    QObject::connect(ui->pushButton_Start, &QPushButton::clicked,
                     currentCanBusWorker, &CanBusWorker::start,
                     CanBusWorkerDB::uniqueQtConnectionType);
    QObject::connect(ui->pushButton_Stop, &QPushButton::clicked,
                     currentCanBusWorker, &CanBusWorker::stop,
                     CanBusWorkerDB::uniqueQtConnectionType);

    aNewThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::In(QVariant *enumVar, QVariant *dataVar)
{
    anAck("Data Received From CanBusWorker");
    QString enumVarTypeName(enumVar->typeName());
    if (enumVarTypeName == QStringLiteral("CanBusWorkerDB::DataGet"))
    {
        anAck("" + enumVarTypeName + " Parsed !");
        switch (enumVar->toInt()) {
        case CanBusWorkerDB::requestPluginAndInterface:
        {
            CanBusWorkerDB::PluginNameAndInterfaceName tmp;
            tmp.first = new QString("socketcan");
            tmp.second = new QString("can0");
            emit Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::replyPluginAndInterface)),
                     new QVariant(QVariant::fromValue(tmp)));
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
        case CanBusWorkerDB::replyCanFrameWithTimeStamp:
        {
            anInfo("replyCanFrameWithTimeStamp");
            CanBusWorkerDB::CanBusFrameWithTimeStamp tmp =
                    dataVar->value<CanBusWorkerDB::CanBusFrameWithTimeStamp>();
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
            ui->textEdit_Read->append(*(tmp.second) + " : " + tmp.first->toString());
#else
            ui->textEdit_Read->append(*(tmp.second) + " : " + QString().setNum(tmp.first->frameId(),16)
                                      + " | " + tmp.first->payload().toHex());
#endif
            break;
        }
        default:
            break;
        }
    }
    else if (enumVarTypeName == QStringLiteral("CanBusWorkerDB::Error"))
    {
        anAck("" + enumVarTypeName + " Parsed !");
        ui->textEdit_Read->append(QString(CanBusWorkerDB::ErrorMetaEnum.valueToKey(enumVar->toInt())));
        if (dataVar)
        {
            ui->textEdit_Read->append(dataVar->value<QString>());
        }
    }
    else if (enumVarTypeName == QStringLiteral("CanBusWorkerDB::Notification"))
    {
        anAck("" + enumVarTypeName + " Parsed !");
        switch (enumVar->toInt()) {
        case CanBusWorkerDB::CanFrameTransmitted:
        {
            anInfo("CanFrameTransmitted");
            QCanBusFrame tmp = dataVar->value<QCanBusFrame>();
            ui->textEdit_Send->append("=> Successfully Transmit : ");
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
            ui->textEdit_Send->append(tmp.toString());
#else
            ui->textEdit_Send->append(QString().setNum(tmp.frameId(),16)
                                      + " | " + tmp.payload().toHex());
#endif
            break;
        }
        case CanBusWorkerDB::DeviceCreated:
        {
            anInfo("DeviceCreated");
            ui->textEdit_Read->append("=> Device Created !");
            break;
        }
        case CanBusWorkerDB::DeviceConnected:
        {
            anInfo("DeviceConnected");
            ui->textEdit_Read->append("=> Device Connected !");
            break;
        }
        default:
            break;
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

void MainWindow::on_pushButton_SendARandomMessage_clicked()
{
    QCanBusFrame tmp(quint8(qrand()%0xff),QByteArray::number(qrand()%0xffff));
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
    ui->textEdit_Send->append(tmp.toString());
#else
    ui->textEdit_Send->append(QString().setNum(tmp.frameId(),16)
                              + " | " + tmp.payload().toHex());
#endif
    emit Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::addAFrameIntoPendingFrameList)),
             new QVariant(QVariant::fromValue(tmp)));
}

void MainWindow::on_pushButton_clearPendingFrameList_clicked()
{
    emit Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::clearPendingFrameList)));
}

void MainWindow::on_pushButton_clicked()
{
    tmr.setInterval(1000);
    QObject::connect(&tmr, &QTimer::timeout,
                     [&](){qApp->quit();});
    tmr.start();
}

void MainWindow::on_pushButtonSendAPresenceReq_clicked()
{
    emit Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::addAFrameIntoPendingFrameList)),
             new QVariant(QVariant::fromValue(CanProtocol::PresenceRequest.getMsg())));
}

void MainWindow::on_pushButtonSendADataReq_clicked()
{
    emit Out(new QVariant(QVariant::fromValue(CanBusWorkerDB::addAFrameIntoPendingFrameList)),
             new QVariant(QVariant::fromValue(CanProtocol::DataRequest(ui->spinBoxDataReqSdcsId->value()).getMsg())));
}
