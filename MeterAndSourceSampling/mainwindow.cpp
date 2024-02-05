#include <QDir>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    on_refreshBtn_clicked();

    scene = new QGraphicsScene(this);
    ui->customPlot->setScene(scene);

    // meter
    functionTest = new FunctionTest(ui->serialPort->currentText(), this);
    connect(functionTest, SIGNAL(functionTestResult(FunctionTestResult*)), this, SLOT(get_meter_values(FunctionTestResult*)));

//    connect(functionTest, SIGNAL(fetchVIPass()), this, SLOT(get_meter_values(FunctionTestResult*)));
//    connect(functionTest, SIGNAL(packetReceived()), this, SLOT(get_meter_values(FunctionTestResult*)));

    // source
    powerSource = new PowerSource(ui->psSerialPort->currentText(), ui->cmriSerialPort->currentText(), this);

    connect(powerSource, SIGNAL(voltageReceived(QString)), this, SLOT(get_source_valuesV(QString)));
    connect(powerSource, SIGNAL(currentReceived(QString)), this, SLOT(get_source_valuesI(QString)));

    // Create a QGraphicsScene for plotting
    scene = new QGraphicsScene(this);
    ui->customPlot->setScene(scene);


    // Initialize variables
    meterVoltage = 0.0;
    sourceVoltage[0] = 0.0;
    meterCurrent = 0.0;
    sourceCurrent[0] = 0.0;
}

void MainWindow::timerEvent(QTimerEvent *event){
    if (event->timerId() == timerId)
    {
        updatePlot();

//        functionTest->fetchVI();
//        functionTest->processFetchingVI();
        functionTest->startTest();

        powerSource->checkCurrent();
        powerSource->checkVoltage();
//        unitTestUpdateDisplay("Timer event!");

    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startBtn_clicked()
{

    QStringList voltages= {ui->phase1Voltage->text(), ui->phase2Voltage->text(), ui->phase3Voltage->text()};
    QStringList currents= {ui->phase1Current->text(), ui->phase2Current->text(), ui->phase3Current->text()};

    powerSource->disablePowerOutput();
    powerSource->setVoltage(voltages[0].toFloat(), voltages[1].toFloat(), voltages[2].toFloat());
    powerSource->setCurrent(currents[0].toFloat(), currents[1].toFloat(), currents[2].toFloat());
    powerSource->setChannelState(PowerSource::POWER_CHANNEL_1 | PowerSource::POWER_CHANNEL_2 | PowerSource::POWER_CHANNEL_3, true);


    timerId = startTimer(2000);

}

void MainWindow::on_stopBtn_clicked()
{
    killTimer(timerId);
}


void MainWindow::get_meter_values(FunctionTestResult* result){
    if (result->isTestPass){
        meterCurrent = result->phaseCurrent;
        meterVoltage = result->voltage;
        unitTestUpdateDisplay("meter current: "+QString::number(result->phaseCurrent));
        unitTestUpdateDisplay("meter voltage: "+QString::number(result->voltage));
    }
}



void MainWindow::get_source_valuesV(QString voltage){
    QList<float> floatList;
    QStringList currents = voltage.split(",");
    for (const QString& str : currents) {
        bool ok;
        float floatValue = str.trimmed().toFloat(&ok);

        if (ok) {
            floatList.append(floatValue);
        } else {
            qDebug() << "Invalid float value:" << str;
        }
    }
    sourceVoltage[0] = floatList[0];
    unitTestUpdateDisplay("source voltage"+voltage);
}

void MainWindow::get_source_valuesI(QString current){
    QList<float> floatList;
    QStringList currents = current.split(",");
    for (const QString& str : currents) {
        bool ok;
        float floatValue = str.trimmed().toFloat(&ok);

        if (ok) {
            floatList.append(floatValue);
        } else {
            qDebug() << "Invalid float value:" << str;
        }
    }
    sourceCurrent[0] = floatList[0]; // since the value of power is coming
    unitTestUpdateDisplay("source power: "+current);
}




void MainWindow::unitTestUpdateDisplay(QString testUpdate)
{
    updateString = ui->DialogLog->toPlainText() + "\n" + QDateTime::currentDateTime().toString() + " :: " +testUpdate + "\n";

    ui->DialogLog->setText(updateString);
    ui->DialogLog->moveCursor (QTextCursor::End) ;
    ui->DialogLog->ensureCursorVisible() ;
}


void MainWindow::on_refreshBtn_clicked()
{
    QDir lsDev("/dev/");
    lsDev.setNameFilters(QStringList() << "tty*");
    QStringList ports = lsDev.entryList(QDir::System);

    QStringList meterPortNames={};
    QStringList scannerPortNames={};

    for (const QString port : ports) {
        if (port.startsWith("ttyUSB")){
                meterPortNames.append(port.mid(0,10));
        }
    }

    ui->serialPort->clear();
    ui->serialPort->addItems(meterPortNames);
    ui->serialPort->setCurrentIndex(0);

    ui->psSerialPort->clear();
    ui->psSerialPort->addItems(meterPortNames);
    ui->psSerialPort->setCurrentIndex(0);

    ui->cmriSerialPort->clear();
    ui->cmriSerialPort->addItems(meterPortNames);
    ui->cmriSerialPort->setCurrentIndex(0);

    // when simulator is running
    if (meterPortNames.contains("ttyUSB100")){
        ui->serialPort->setCurrentIndex(meterPortNames.indexOf("ttyUSB100"));
    }
    if (meterPortNames.contains("ttyUSB107")){
        ui->psSerialPort->setCurrentIndex(meterPortNames.indexOf("ttyUSB107"));
    }
    if (meterPortNames.contains("ttyUSB108")){
        ui->cmriSerialPort->setCurrentIndex(meterPortNames.indexOf("ttyUSB108"));
    }

    if (meterPortNames.isEmpty()) {
        qDebug() << "Error: USB port not found.";
        unitTestUpdateDisplay("Error - No USB found!");
    }
}




