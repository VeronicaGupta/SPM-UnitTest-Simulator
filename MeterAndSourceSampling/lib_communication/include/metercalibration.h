#ifndef METERCALIBRATION_H
#define METERCALIBRATION_H

#include <QObject>
#include <QFinalState>

#include "pvstatemachine.h"
#include "metercommunication.h"
#include "meterinfo.h"
#include "pvstate.h"

class QSerialPort;

class MeterCalibration : public MeterCommunication
{
    Q_OBJECT
public:
    enum CALIBRATION{
        CALIBRATE_PHASE,
        CALIBRATE_NEUTRAL = 3,
        CALIBRATE_BOTH = 4
    };
    explicit MeterCalibration(QString portName, CALIBRATION calibrate = CALIBRATE_PHASE,  QObject *parent = nullptr, QString tag = "");

public slots:
    void calibrate();
    void meterSendCalibrate();
    void printState();

protected slots:
    void packetReceived(QByteArray packet);

private slots:
    void processCalibrationPass();
    void processCalibrationFail();
    void processStateMachineStoped();

signals:
    void calibrationSuccess();
    void calibrationFail();
    void calibrationTestResult(CalibrationResult* result);

private:
    CalibrationResult testResult;
    MeterCommunication* meterComm;
    int receivedPacketLength;
    QByteArray calibratePacketData;
    QTimer* timeout;

    CALIBRATION calibrateWhat;

    PVState *authenticating;
    PVState *calibrating;
    QFinalState *failed;
    QFinalState *calibrated;

    PVStateMachine* calibrateMachine;
};

#endif // METERCALIBRATION_H
