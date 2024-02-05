#ifndef DETECTSERIALPORTS_H
#define DETECTSERIALPORTS_H

#include <QObject>
#include <QSerialPortInfo>
#include <QMap>

//#define FUNCTION_TEST_PORT              "AB0KTZF8"
//#define POWER_SOURCE_PORT               "1027-24577"
////#define POWER_SOURCE_PORT               "B001YM51"

//#define CMRI_PORT                       "A50285BI"
//#define PHASE_CALIBRATION_PORT          "B0006Q1E"
//#define NEUTRAL_CALIBRATION_PORT        "AB0KTZ72"
//#define STARTING_CURRENT_PORT           "AB0KTZ7P"
//#define ERRORTEST_LOWCURRENT_PORT       "AB0KH9LB"
//#define ERRORTEST_HIGHCURRENT_PORT      "AB0KTZ7A"
//#define NIC_SYNC_PORT                   "AB0KTZ7B"

#define FUNCTION_TEST_PORT              "B0006K64"
#define PHASE_CALIBRATION_PORT          "B0006K6F"
#define NEUTRAL_CALIBRATION_PORT        "B0006K68"
#define STARTING_CURRENT_PORT           "B0006Q2Y"
#define ERRORTEST_LOWCURRENT_PORT       "B0006Q0Y"
#define ERRORTEST_HIGHCURRENT_PORT      "B0006Q2I"
#define NIC_SYNC_PORT                   "B0006H25"

#define POWER_SOURCE_PORT               "1027-24577"
//#define POWER_SOURCE_PORT               "B001YM51"
#define CMRI_PORT                       "A50285BI"

class VIInfo{
public:
    float voltage;
    float phaseCurrent;
    float neutralCurrent;
};

class DetectSerialPorts : public QObject
{
    Q_OBJECT
public:
    explicit DetectSerialPorts(QObject *parent = nullptr);



    void detectPorts();
    static void firstTimeDetect();

private:
    bool authenticate(QString port);
    bool getVI(QString port, VIInfo &info);
    void delay(int ms);

signals:

private:
    QSerialPortInfo powerSourcePort;
    QMap<QString, QString> portMap;
    static QString tag;
};

#endif // DETECTSERIALPORTS_H
