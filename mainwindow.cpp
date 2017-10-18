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
                     uniqueQtConnectionType);
    QObject::connect(this, &MainWindow::Out,
                     currentCanBusWorker, &CanBusWorker::In,
                     uniqueQtConnectionType);
    QObject::connect(currentCanBusWorker, &CanBusWorker::Out,
                     this, &MainWindow::In,
                     uniqueQtConnectionType);

    aNewThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::In(const GlobalSignal &aGlobalSignal)
{
    anTrk("Signal-From-CanBusWorker Received !");
    QString enumVarTypeName(aGlobalSignal.Type.typeName());
    if (enumVarTypeName == QStringLiteral("CanBusWorkerBasis::Data"))
    {
        anTrk(enumVarTypeName);
        switch (aGlobalSignal.Type.toInt()) {
        case CanBusWorkerBasis::replyFrameWithTimeStamp:
        {
            anInfo("replyFrameWithTimeStamp");
            QCanBusFrame tmpFrame = aGlobalSignal.Data.value<QCanBusFrame>();
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
            ui->textEdit_Read->append(aGlobalSignal.TimeStamp + " : " + tmpFrame.toString());
#else
            ui->textEdit_Read->append(aGlobalSignal.TimeStamp + " : " + QString().setNum(tmpFrame.frameId(),16)
                                      + " | " + tmpFrame.payload().toHex());
#endif
            break;
        }
        default:
            break;
        }
    }
    else if (enumVarTypeName == QStringLiteral("CanBusWorkerBasis::Error"))
    {
        anTrk(enumVarTypeName);
        ui->textEdit_Read->append(QString(CanBusWorkerBasis::ErrorMetaEnum.valueToKey(aGlobalSignal.Type.toInt())));
        if (!aGlobalSignal.Data.isNull())
        {
            ui->textEdit_Read->append(aGlobalSignal.Data.toString());
        }
    }
    else if (enumVarTypeName == QStringLiteral("CanBusWorkerBasis::Notification"))
    {
        anTrk(enumVarTypeName);
        switch (aGlobalSignal.Type.toInt()) {
        case CanBusWorkerBasis::FrameWritten:
        {
            anInfo("FrameWritten");
            QCanBusFrame tmp = aGlobalSignal.Data.value<QCanBusFrame>();
            ui->textEdit_Send->append("=> Successfully Transmit : ");
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
            ui->textEdit_Send->append(tmp.toString());
#else
            ui->textEdit_Send->append(QString().setNum(tmp.frameId(),16)
                                      + " | " + tmp.payload().toHex());
#endif
            break;
        }
        case CanBusWorkerBasis::readyToWork:
        {
            anAck("CanBusWorker Is Ready To Work !");
            ui->textEdit_Read->append("=> CanBusWorker Is Ready !");
            break;
        }
        default:
            break;
        }
    }
}

void MainWindow::on_pushButtonQUIT_clicked()
{
    invoker.setInterval(1000);
    QObject::connect(&invoker, &QTimer::timeout, this,[&](){qApp->quit();});
    invoker.start();
}

void MainWindow::on_pushButtonSendAPresenceReq_clicked()
{
    GlobalSignal aCanPresenceRequest;
    aCanPresenceRequest.Type = QVariant::fromValue(CanBusWorkerBasis::requestFrameTransmission);
    aCanPresenceRequest.Data = QVariant::fromValue(CanProtocol::PresenceRequest.getMsg());
    emit Out(aCanPresenceRequest);
}

void MainWindow::on_pushButtonSendADataReq_clicked()
{
    GlobalSignal aCanDataRequest;
    aCanDataRequest.Type = QVariant::fromValue(CanBusWorkerBasis::requestFrameTransmission);
    aCanDataRequest.Data = QVariant::fromValue(CanProtocol::DataRequest(ui->spinBoxDataReqSdcsId->value()).getMsg());
    emit Out(aCanDataRequest);
}

void MainWindow::on_pushButton_SendARandomMessage_clicked()
{
    QCanBusFrame tmp(quint8(qrand()%0xff),QByteArray::number(qrand()%0xffff));
#if QT_VERSION >= QT_VERSION_CHECK(5,8,0)
    ui->textEdit_Send->append("Try Send: " + tmp.toString());
#else
    ui->textEdit_Send->append("Try Send: " + QString().setNum(tmp.frameId(),16)
                              + " | " + tmp.payload().toHex());
#endif
    GlobalSignal aRandomFrame;
    aRandomFrame.Type = QVariant::fromValue(CanBusWorkerBasis::requestFrameTransmission);
    aRandomFrame.Data = QVariant::fromValue(tmp);
    emit Out(aRandomFrame);
}

void MainWindow::on_pushButton_clearBuffer_clicked()
{
    GlobalSignal aBufferClear;
    aBufferClear.Type = QVariant::fromValue(CanBusWorkerBasis::clearBuffer);
    emit Out(aBufferClear);
}
