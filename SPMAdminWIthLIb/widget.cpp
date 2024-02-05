#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTimer>
#include <QSettings>

#include "autotest/calibrationautotest.h"
#include "autotest/errorautotest.h"
#include "meterstartingcurrenttest.h"
#include "functiontest.h"
#include "plccontroller.h"
#include "spmfulltest.h"
#include "lasercommunication.h"
#include "cmricontrol.h"

#include "autotest/nicsyncautotest.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QSettings settings;

//    NicSyncTest* autoTest = new NicSyncTest(settings.value("networkSycnPort").toString(), EnCustomer::CUST_INTELLI);
//    autoTest->startTest("000000000");
//    NicSyncAutoTest* autoTest = new NicSyncAutoTest(settings.value("networkSycnPort").toString());
//    autoTest->starTest();

    CMRIControl* cmri = new CMRIControl( settings.value("cmriSourcePort").toString());
    cmri->cmriReset();

//    NICSync* nicSync = new NICSync(settings.value("networkSycnPort").toString(), EnCustomer::CUST_JAMMU, this);
////    NICSync* nicSync = new NICSync(settings.value("networkSycnPort").toString(), EnCustomer::CUST_JAMMU, this);
//    nicSync->startTest("KA6097054");

//    powerSource = new PowerSource(settings.value("powerSourcePort").toString());
//    powerSource->enablePowerOutput();
//    settings.setValue(settings.value("powerSourcePort").toString());


//    MeterStartingCurrentTest* startingTest = new MeterStartingCurrentTest(240, 0.04, 10, settings.value("errorTestStartingCurrentPort").toString(), this, "SC");
//    startingTest->startErrorTest();

//    MeterErrorTest* lowTest = new MeterErrorTest(240, 1, 0.5, settings.value("errorTestLowCurrentPort").toString(), this, "LowErr");
//    lowTest->startErrorTest();

    MeterErrorTest* highTest = new MeterErrorTest(240, 60, 0.5, settings.value("errorTestHighCurrentPort").toString(), this, "HighErr");
    highTest->startErrorTest();

//    FunctionTest* test = new FunctionTest("/dev/ttyUSB3");
//    test->startTest();

//    laser = new LaserCommunication(this);
//    laser->connectLaser();


//    MeterInfo* meter = new MeterInfo();
//    meterStageQueue.enqueue(meter);


//    SPMFullTest* fullTest = new SPMFullTest(this);
//    fullTest->startTest(meterStageQueue, TEST_FUNCTION);


//    PLCController* plcController = new PLCController(this);
//    plcController->startTest();

//    FunctionTest* test = new FunctionTest(settings.value("functionTestPort").toString());
//    QTimer::singleShot(6000, test, SLOT(startTest()));

//    CalibrationAutoTest *test = new CalibrationAutoTest(this);
//    QTimer::singleShot(6000, test, SLOT(startTest()));
//    test->startTest();

//    meterKey
//    NetworkConfiguration

//    ErrorAutoTest* test = new ErrorAutoTest(this);
//    QTimer::singleShot(6000, test, SLOT(restartTest()));

//    calErrorTest = new CalibrationErrorTest(this);
//    QTimer::singleShot(6000, calErrorTest, SLOT(startTest()));
//    calErrorTest->startTest(PHASE_CALIBRATION_TEST | NEUTRAL_CALIBRATION_TEST | LOW_CURRENT_TEST | HIGH_CURRENT_TEST);

//    Calib

//    calibration = new MeterCalibration(settings.value("neutralCalibrationPort").toString(), MeterCalibration::CALIBRATE_NEUTRAL);
//    calibration->calibrate();
//    errorTest = new MeterErrorTest(240, 10, 0.5, "/dev/ttyUSB0", this);
//    errorTest->startErrorTest();

//    comm = new MeterCommunication("/dev/ttyUSB1");
//    connect(comm, SIGNAL(packetReceived(QByteArray)), this, SLOT(meterPacketReceived(QByteArray)));

//    powerSource = new PowerSource(settings.value("powerSourcePort").toString(), settings.value("cmriSourcePort").toString());
//    powerSource->setChannelState(7, true);
//    connect(powerSource, SIGNAL(packetReceived(QByteArray)), this, SLOT(packetReceived(QByteArray)));
//    comm->authenticate();

//    detectSerial = new DetectSerialPorts(this);
//    detectSerial->detectPorts();

//    connect(ui->btnDialer, &QPushButton::clicked, []() {
//         qDebug() << "Dialer Clicked";
//    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::meterPacketReceived(QByteArray packet)
{
//    qDebug() << "packetReceived" << packet.toHex();

}


void Widget::on_btnAuthenticate_clicked()
{
    comm->authenticate();
}


void Widget::on_btnCalibrate_clicked()
{
    calibration->calibrate();
}


void Widget::on_btnGetSerial_clicked()
{
    powerSource->getSerial();
}


void Widget::on_btnOffOutput_clicked()
{
    powerSource->disablePowerOutput();
}


void Widget::on_btnOnOutput_clicked()
{
    powerSource->enablePowerOutput();
}


void Widget::on_btnCheckCurrent_clicked()
{
    powerSource->checkCurrent();
}


void Widget::on_btnApplyPower_clicked()
{
    powerSource->setVoltage(1, ui->leL1Voltage->text().toFloat());
    powerSource->setVoltage(2, ui->leL2Voltage->text().toFloat());
    powerSource->setVoltage(3, ui->leL3Voltage->text().toFloat());

    powerSource->setCurrent(1, ui->leL1Current->text().toFloat());
    powerSource->setCurrent(2, ui->leL2Current->text().toFloat());
    powerSource->setCurrent(3, ui->leL3Current->text().toFloat());
}


void Widget::on_btnCheckVoltage_clicked()
{
    powerSource->checkVoltage();

}


void Widget::on_btnApplyPowerState_clicked()
{
    powerSource->setChannelState(PowerSource::POWER_CHANNEL_1 | PowerSource::POWER_CHANNEL_2 | PowerSource::POWER_CHANNEL_3, true);
}


void Widget::on_btnClearCalibration_clicked()
{
    QByteArray streamData, crcData;
    QDataStream stream (&streamData, QIODevice::WriteOnly);

    stream << (quint8)0x02 << (quint8)0xFF;// cmd for clear meter.

    auto finalPacket = calibration->createPacket(MeterCommunication::MEM_SELECT_DATA_FLASH, streamData);
    calibration->sendPacket(finalPacket);
}


void Widget::on_btnErrorTest1_clicked()
{
    errorTest->startEnergyAccumulation();
}


void Widget::on_btnErrorTest2_clicked()
{
    errorTest->stopEnergyAccumulation();
}


void Widget::on_btnTest_clicked()
{
    errorTest->startErrorTest();
}


void Widget::on_btnDialer_clicked()
{
    QStringList paraList;

    laser->startMeterEngraving("APDCL/CGM(PP&D)/SMART METER (DBFOOT)/PKG-1/2021/28 Dated. 12.11.2021",
                               "APDCL/CGM(PP&D)/SMART METER (DBFOOT)/PKG-2/2021/29 Dated. 12.11.2021", "11/2023", "QC6087648");
}


void Widget::on_pushButton_clicked()
{

}

