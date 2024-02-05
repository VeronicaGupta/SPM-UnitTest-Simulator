#include "plccontroller.h"
#include <QDebug>
#include <QThread>
#include <QTime>
#include <QEventLoop>
#include <QCoreApplication>
#include "detectserialports.h"

/*
 * First Meter Feed. <MoveToNextStage>
 * Run Test on each stages have meter.
 * Save Result in Each stage meter object.
 *
 *
 *
 */

PLCController::PLCController(QObject *parent) : QTcpSocket(parent)
{
    spmFullTest = new SPMFullTest(this);
    isPaused = false;
    tag = "PLC";

    QTimer* timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(temp2()));
//    timer->setInterval(3000);
//    timer->start();

    for(int i=0; i<11; i++){
        MeterInfo* meter = new MeterInfo(false);
        meterStageQueue.enqueue(meter);
    }

    currentMeterCount=0;
    meterToBeProduced=50;

    connect(spmFullTest, SIGNAL(spmFullTestCompleted()), this, SLOT(testCompleted()));
    connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(connected()), this, SLOT(socketConnectd()));
    connect(this, SIGNAL(disconnected()), this, SLOT(sockedDisconnectd()));
#ifndef SIMULATOR_MODE
    connectToHost("192.168.2.10", 55540);
#else
    connectToHost("127.0.0.1", 55540);
#endif
    qDebug() << tag << "SocketConnecting...";
//    qDebug() << tag << "connected" << waitForConnected();
}

void PLCController::startProduction(QString batchNumber, QString loaNumber2, QString laserLayout, QString printPrefix, int serialFrom, int serialTo)
{
    this->meterToBeProduced = serialTo - serialFrom;
    currentMeterCount = 0;
    currentSerial = serialFrom;

    QSettings settings;
    settings.setValue("curretnBatchNumber", batchNumber);
    settings.setValue("loaNumber2", loaNumber2);
    settings.setValue("currentPrintPrefix", printPrefix);
    settings.setValue("currentSerialFrom", serialFrom);
    settings.setValue("currentSerialTo", serialTo);
    settings.setValue("currentSerial", serialFrom);
    settings.sync();
}

void PLCController::startProduction()
{
    //settings
    QSettings settings;
    currentSerial = settings.value("currentSerial").toInt();
    int serialTo = settings.value("currentSerialTo").toInt();
    meterToBeProduced = serialTo - currentSerial;
    spmFullTest->startProduction(settings.value("curretnBatchNumber").toString(), settings.value("loaNumber2").toString(), QString(""), settings.value("currentPrintPrefix").toString(), currentSerial, settings.value("currentSerialTo").toInt());

    sendCommand("Start_of_Line");
}

void PLCController::pauseProduction()
{
    isPaused = true;
}

void PLCController::resumeProduction()
{
    isPaused = false;
    processCommands(lastCommand);
}

void PLCController::stopProduction()
{

}

void PLCController::processCommands(QString command)
{
    QString str = command;
    qDebug() << tag << "processCommands" << str;

    if(str.startsWith("Ack_Start_of_Line")){

    }
    if(str.startsWith("Start_test")){
        sendCommand(QString("Ack_") + str.trimmed());
        delay(2);
        sendCommand("Get_data");
    }
//    else if(str.startsWith("Error_while_Indexing")){

//    }
//    else if(str.startsWith("Error_while_QCD")){

//    }
//    else if(str.startsWith("Error_while_Contactor")){

//    }
    if(str.startsWith("Get_data_V")){
        sendCommand("Get_data_success");
        delay(2);
        sendCommand("Get_QR_data");
    }
    if(str.startsWith("Get_QR_data")){
        qDebug() << tag << "qr" << str.right(str.length()-12);
        spmFullTest->setQRCode(str.right(str.length()-12));
        sendCommand("QR_success");
        startTest();
//        delay(2);
//        sendCommand("Part_Result_Status_P-----------");
    }
    if(str.startsWith("Part_Result_update")){
        //ToDo: update status of 2 or 4  meter tested by SPM

        auto tmpStr = str.split(" ").last();
        TestResult ts;
//        qDebug() << tag << str << str.at(1) << str.at(2);

        ts.isTestPass = (tmpStr.at(1) == "P");
        getSpmString(*meterStageQueue.at(10), ts, "C");

        ts.isTestPass = (tmpStr.at(2) == "P");
        getSpmString(*meterStageQueue.at(9), ts, "D");

        moveNextStage();

        if(currentMeterCount < meterToBeProduced)
            sendCommand("Part_Result_Read_Success");
        else if(currentMeterCount == meterToBeProduced)
            sendCommand("End_Of_Line");
    }
}

void PLCController::readyRead()
{
    QString str = QString::fromStdString(readAll().toStdString());
    lastCommand = str;

    emit commandReceived(str);

    if(!isPaused)
        processCommands(str);
}

void PLCController::socketConnectd()
{
    qDebug() << tag << "SocketConnected";
}

void PLCController::sockedDisconnectd()
{
    qDebug() << tag << "SocketDisonnected";
}

void PLCController::sendCommand(QString command)
{
    qDebug() << tag << "sendingCommand" << command;
    write(command.toStdString().c_str());
}

int PLCController::getTmpSerial(int index)
{
    return meterStageQueue.at(index)->getTmpSerial();
}

int PLCController::getFinalSerial(int index)
{
    return meterStageQueue.at(index)->getFinalSerial();
}

void PLCController::printMeterInfo()
{
    MeterInfo* meter = new MeterInfo(true);
    meterStageQueue.enqueue(meter);
    qDebug() << tag << "printMeterInfo";
    for(int i=0; i<meterStageQueue.count(); i++){
        MeterInfo* meter = meterStageQueue.at(i);
        qDebug() << tag << "isValid" << i << meter->isValid;
    }
}

void PLCController::temp1()
{

}

void PLCController::temp2()
{
    static int counter;
    if(counter++ %2)
        emit commandReceived("Start_test");
    else
        emit commandReceived("Part_Result_update_   BPPPPPPPGP--");
}

void PLCController::calibrate()
{
    spmFullTest->calibrate();
}

void PLCController::delay(int second)
{
    QTime dieTime= QTime::currentTime().addSecs(second);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void PLCController::testCompleted()
{
    QString spmString;

    spmString += meterStageQueue.at(11)->functionTestResult.isTestPass ? "P" : "B";
    spmString += "--";
    spmString += getSpmString(*meterStageQueue.at(8), meterStageQueue.at(8)->phaseCalibrationResult, "E"); //  !meterStageQueue.at(8)->isValid ? "-" : (meterStageQueue.at(8)->phaseCalibration.isTestPass ? "P" : "E");
    spmString += getSpmString(*meterStageQueue.at(7), meterStageQueue.at(7)->neutralCalibrationResult, "F");
    spmString += getSpmString(*meterStageQueue.at(6), meterStageQueue.at(6)->startingCurrentTestResult, "G");
    spmString += getSpmString(*meterStageQueue.at(5), meterStageQueue.at(5)->lowCurrentTestResult, "H");
    spmString += getSpmString(*meterStageQueue.at(4), meterStageQueue.at(4)->highCurrentTestResult, "I");

    spmString += getSpmString(*meterStageQueue.at(3), meterStageQueue.at(3)->laserEngraveResult, "J");
    spmString += getSpmString(*meterStageQueue.at(2), meterStageQueue.at(2)->nicSyncTestResult, "K");
    spmString += getSpmString(*meterStageQueue.at(2), meterStageQueue.at(1)->nicSyncTestResult, "K");
    spmString += getSpmString(*meterStageQueue.at(2), meterStageQueue.at(0)->nicSyncTestResult, "K");
//    spmString += "--";

    sendCommand(QString("Part_Result_Status_") + spmString);
    qDebug() << tag << "PLCController::testCompleted" << spmString;
}

QString PLCController::getSpmString(MeterInfo &info, TestResult &testResult, QString failString)
{
    QString spmString;
    if(!info.isValid)
        spmString = "-";
    else if(info.failedAt.length() > 0)
        spmString = info.failedAt;
    else if(!testResult.isTestPass)
        spmString = failString;
    else
        spmString = "P";

    return spmString;
}

void PLCController::moveNextStage()
{
    qDebug() << tag << "C " <<  meterStageQueue.count();
    auto meter = meterStageQueue.dequeue();
    auto logMeter = meterStageQueue.at(1);
    if(meter->isValid){
        meter->saveLogs();
        meter->deleteLater();
    }
    if(logMeter->isValid){
        qDebug() << tag << "--WatchMe--";
        meter->saveLogs();
    }

    qDebug() << tag <<"D " <<  meterStageQueue.count();
    qDebug() << tag << "*********************************************** Cycle Completed *****************************************************";
}

void PLCController::startTest()
{
    int testsToRun = 0;

    DetectSerialPorts::firstTimeDetect();

    qDebug() << tag << "B " <<  meterStageQueue.count();

    MeterInfo* meter = new MeterInfo(currentMeterCount++ < meterToBeProduced);
    meter->meterTmpSerialNumber = currentSerial++;
    meter->meterSerialPrefix = spmFullTest->getPrintPrefix();
    meter->meterFinalSerialNumber = 0;
    meterStageQueue.enqueue(meter);

    qDebug() << tag << "*********************************************** Cycle Start " << (currentSerial - 1) << " *****************************************************";
    qDebug() << tag << "A " << meterStageQueue.count();

    for(int i=0; i<meterStageQueue.count(); i++){
        int mapIndex = 11-i;
        if(mapIndex < 0)
            continue;

        qDebug() << tag << "startTest" << i << meterStageQueue.at(mapIndex)->failedAt << mapIndex;
        if(meterStageQueue.at(mapIndex)->isValid && (meterStageQueue.at(mapIndex)->failedAt.length() == 0 || meterStageQueue.at(mapIndex)->failedAt == "-"))
            testsToRun |= (1<<(i));
    }

    qDebug() << tag << "startTest " << QString::number(testsToRun, 2);
    emit testStarted();

    spmFullTest->startTest(meterStageQueue, testsToRun);
}

//void PLCController::startTest(int testsToRun)
//{
//    MeterInfo* meter = new MeterInfo(currentMeterCount++ < meterToBeProduced);
//    meterStageQueue.enqueue(meter);

//    spmFullTest->startTest(meterStageQueue, testsToRun);
//}
