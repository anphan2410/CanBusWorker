#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "shared/commonthings.h"
#include "src/canbusworker.h"
#include "CanProtocol/src/canprotocol.h"
#include "anLogger/src/anlogger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void Out(const GlobalSignal &);
private slots:
    void In(const GlobalSignal &aGlobalSignal);


    void on_pushButtonQUIT_clicked();

    void on_pushButtonSendAPresenceReq_clicked();

    void on_pushButtonSendADataReq_clicked();

    void on_pushButton_SendARandomMessage_clicked();

    void on_pushButton_clearBuffer_clicked();

private:
    Ui::MainWindow *ui;
    QTimer invoker;
};

#endif // MAINWINDOW_H
