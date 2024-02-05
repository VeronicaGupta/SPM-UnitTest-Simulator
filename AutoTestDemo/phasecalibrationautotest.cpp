//#include "phasecalibrationautotest.h"

//PhaseCalibrationAutoTest::PhaseCalibrationAutoTest(QString portName, int N, QString s_portname, QObject *parent)
//    : QObject{parent}
//{

//    PORTNAME = portName;
//    S_PORTNAME = s_portname;

//    passCounter = 0;
//    failCounter = 0;
//    testCounter = 0;

//    PORTNAME = portName;
//    MAXTESTS = N;

//    powerSource = new PowerSource(S_PORTNAME, QString(), this);
//    powerSource->setVoltage(240, 240, 240);
//    powerSource->setCurrent(10, 10, 10);
//    powerSource->disablePowerOutput();
////    powerSource->enablePowerOutput();

//    phaseCalibration = new MeterCalibration(PORTNAME, MeterCalibration::CALIBRATE_PHASE, this, "M1");

//    setAllTestResult(TEST_RESULT_IDEAL);

//    connect(powerSource, SIGNAL(allChannelReady()), this, SLOT(powerStablised()));

//    connect(phaseCalibration, SIGNAL(calibrationSuccess()), this, SLOT(phaseCalibrationPass()));
//    connect(phaseCalibration, SIGNAL(calibrationFail()), this, SLOT(phaseCalibrationFail()));

//}

//void PhaseCalibrationAutoTest::startTest()
//{
//    qDebug() << "*********************** CalibrationAutoTest::startTest ************************";
//    setAllTestResult(TEST_RESULT_RUNNING);
//    powerSource->setChannelState(PowerSource::POWER_CHANNEL_1 | PowerSource::POWER_CHANNEL_2 | PowerSource::POWER_CHANNEL_3, true);
//}

//void PhaseCalibrationAutoTest::restartTest()
//{
//    powerSource->disablePowerOutput();
//    QTimer::singleShot(6000, this, SLOT(startTest()));
//}

//void PhaseCalibrationAutoTest::powerStablised()
//{
//    qDebug() << "powerStablised";
//    phaseCalibration->calibrate();
//}

//void PhaseCalibrationAutoTest::phaseCalibrationPass()
//{
//    qDebug() << "phaseCalibrationPass";
//    phaseCalibrationResult = TEST_RESULT_PASS;
//    processTestResult();
//}

//void PhaseCalibrationAutoTest::phaseCalibrationFail()
//{
//    qDebug() << "phaseCalibrationFail";
//    phaseCalibrationResult = TEST_RESULT_FAIL;
//    processTestResult();
//}

//void PhaseCalibrationAutoTest::processTestResult()
//{
//    TestResults results;
//    results << phaseCalibrationResult;
//    qDebug() << results;

//    if(phaseCalibrationResult == TEST_RESULT_RUNNING)
//        return;

//    int passCount = 0;
//    foreach(auto result, results){
//        if(result == TEST_RESULT_PASS)
//            passCount++;
//    }

//    qDebug() << "**************** processTestResult ***********************" << results << ((passCount == 4) ? "AllPass": "somefail") << passCount;
//    restartTest();
////    emit calibrationAutoTestComplete(results);
//}

//void PhaseCalibrationAutoTest::setAllTestResult(TEST_RESULT result)
//{
//    phaseCalibrationResult = result;
//}
