#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "metercommunication.h"
#include "metercalibration.h"
#include "metererrortest.h"
#include "powersource.h"
#include "detectserialports.h"
#include "calibrationerrortest.h"
#include "meterinfo.h"
#include "lasercommunication.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void meterPacketReceived(QByteArray packet);
    void on_btnAuthenticate_clicked();

    void on_btnCalibrate_clicked();

    void on_btnGetSerial_clicked();

    void on_btnOffOutput_clicked();

    void on_btnOnOutput_clicked();

    void on_btnCheckCurrent_clicked();

    void on_btnApplyPower_clicked();

    void on_btnCheckVoltage_clicked();

    void on_btnApplyPowerState_clicked();

    void on_btnClearCalibration_clicked();

    void on_btnErrorTest1_clicked();

    void on_btnErrorTest2_clicked();

    void on_btnTest_clicked();

    void on_btnDialer_clicked();

    void on_pushButton_clicked();

private:
    MeterCommunication* comm;
    PowerSource* powerSource;
    MeterCalibration* calibration;
    MeterErrorTest* errorTest;

    DetectSerialPorts* detectSerial;
    CalibrationErrorTest* calErrorTest;

    QQueue<MeterInfo*> meterStageQueue;

    LaserCommunication* laser;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
