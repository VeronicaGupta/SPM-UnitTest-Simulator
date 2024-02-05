#include "detectserialports.h"
#include "powersource.h"
#include "metercommunication.h"
#include "aes/qaesencryption.h"
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <QDataStream>
#include <QSettings>
#include <QMap>

QString DetectSerialPorts::tag = "";

DetectSerialPorts::DetectSerialPorts(QObject *parent)
    : QObject{parent}
{
    auto ports = QSerialPortInfo::availablePorts();
    QSettings settings;
    tag = "DP";

    portMap.clear();

    for(int i=0; i<ports.count(); i++){
        QSerialPortInfo port = ports.at(i);
        portMap.insert(&""[port.productIdentifier()], port.systemLocation());

        if(port.serialNumber().trimmed().length() == 0){
            powerSourcePort = port;
            ports.removeAt(i--);
        }
    }
}

void DetectSerialPorts::firstTimeDetect()
{
    auto ports = QSerialPortInfo::availablePorts();
    QSettings settings;
    QStringList foundPorts;

    QMap<QString, QString > map;

    for(int i=0; i<ports.count(); i++){
        QSerialPortInfo port = ports.at(i);
        if(port.serialNumber().length())
            map[port.serialNumber()] = port.systemLocation();
        else
            map[QString("%1-%2").arg(port.vendorIdentifier()).arg(port.productIdentifier())] =  port.systemLocation();

        qDebug() << tag << port.portName() << port.systemLocation() << port.vendorIdentifier() << port.serialNumber() << port.productIdentifier() << map[port.serialNumber()] ;
    }

#ifdef SIMULATOR_MODE
    settings.setValue("powerSourcePort", "/dev/ttyUSB107");
    settings.setValue("cmriSourcePort", "/dev/ttyUSB108");
    settings.setValue("functionTestPort", "/dev/ttyUSB100");
    settings.setValue("phaseCalibrationPort", "/dev/ttyUSB101");
    settings.setValue("neutralCalibrationPort", "/dev/ttyUSB102");
    settings.setValue("errorTestStartingCurrentPort", "/dev/ttyUSB103");
    settings.setValue("errorTestLowCurrentPort", "/dev/ttyUSB104");
    settings.setValue("errorTestHighCurrentPort", "/dev/ttyUSB105");
    settings.setValue("networkSycnPort", "/dev/ttyUSB106");
#else
    settings.setValue("powerSourcePort", map[POWER_SOURCE_PORT]);
    settings.setValue("cmriSourcePort", map[CMRI_PORT]);
    settings.setValue("functionTestPort", map[FUNCTION_TEST_PORT]);
    settings.setValue("phaseCalibrationPort", map[PHASE_CALIBRATION_PORT]);
    settings.setValue("neutralCalibrationPort", map[NEUTRAL_CALIBRATION_PORT]);
    settings.setValue("errorTestStartingCurrentPort", map[STARTING_CURRENT_PORT]);
    settings.setValue("errorTestLowCurrentPort", map[ERRORTEST_LOWCURRENT_PORT]);
    settings.setValue("errorTestHighCurrentPort", map[ERRORTEST_HIGHCURRENT_PORT]);
    settings.setValue("networkSycnPort", map[NIC_SYNC_PORT]);
#endif

    qDebug() << tag << "powerSourcePort" << settings.value("powerSourcePort");
    qDebug() << tag << "cmriSourcePort" << settings.value("cmriSourcePort");
    qDebug() << tag << "functionTestPort" << settings.value("functionTestPort");
    qDebug() << tag << "phaseCalibrationPort" << settings.value("phaseCalibrationPort");
    qDebug() << tag << "neutralCalibrationPort" << settings.value("neutralCalibrationPort");
    qDebug() << tag << "errorTestStartingCurrentPort" << settings.value("errorTestStartingCurrentPort");
    qDebug() << tag << "errorTestLowCurrentPort" << settings.value("errorTestLowCurrentPort");
    qDebug() << tag << "errorTestHighCurrentPort" << settings.value("errorTestHighCurrentPort");
    qDebug() << tag << "networkSycnPort" << settings.value("networkSycnPort");

//#ifndef SIMULATOR_MODE
//    Q_ASSERT(map[POWER_SOURCE_PORT].length());
//    Q_ASSERT(map[CMRI_PORT].length());
//    Q_ASSERT(map[FUNCTION_TEST_PORT].length());
//    Q_ASSERT(map[PHASE_CALIBRATION_PORT].length());
//    Q_ASSERT(map[NEUTRAL_CALIBRATION_PORT].length());
//    Q_ASSERT(map[STARTING_CURRENT_PORT].length());
//    Q_ASSERT(map[ERRORTEST_LOWCURRENT_PORT].length());
//    Q_ASSERT(map[ERRORTEST_HIGHCURRENT_PORT].length());
//    Q_ASSERT(map[NIC_SYNC_PORT].length());
//#endif
}

void DetectSerialPorts::detectPorts()
{
    auto ports = QSerialPortInfo::availablePorts();
    QSettings settings;
    QStringList foundPorts;

    for(int i=0; i<ports.count(); i++){
        QSerialPortInfo port = ports.at(i);
        qDebug() << tag << port.portName() << port.systemLocation() << port.vendorIdentifier() << port.serialNumber() << port.productIdentifier();

        if(port.serialNumber().trimmed().length() == 0){
            powerSourcePort = port;
            ports.removeAt(i--);
        }
    }

//    qDebug() << tag << "PowerSource Port" << powerSourcePort.portName();
//    settings.setValue("powerSourcePort", powerSourcePort.systemLocation());
//    PowerSource ps(powerSourcePort.systemLocation());

//    ps.disablePowerOutput();
//    MeterCommunication::delay(5000);

//    ps.setVoltage(240, 0, 0);
//    ps.setCurrent(1, 0, 0);

//    ps.enablePowerOutput();
//    MeterCommunication::delay(2000);

    for(int i=0; i<ports.count(); i++){
        bool isAuthenticated = authenticate(ports.at(i).systemLocation());
        qDebug() << tag << "authenticate CL" << ports.at(i).systemLocation() << isAuthenticated;
        if(isAuthenticated){
            VIInfo info;
            getVI(ports.at(i).systemLocation(), info);

            if(info.neutralCurrent > 0){
                qDebug() << tag << "Neutral Calibration" << ports.at(i).systemLocation();
                settings.setValue("neutralCalibrationPort", ports.at(i).systemLocation());
                foundPorts << QString("neutralCalibrationPort %1").arg(ports.at(i).systemLocation());
                ports.removeAt(i--);
            }
            else if(info.phaseCurrent > 0){
                qDebug() << tag << "Phase Calibration" << ports.at(i).systemLocation();
                settings.setValue("phaseCalibrationPort", ports.at(i).systemLocation());
                foundPorts << QString("phaseCalibrationPort %1").arg(ports.at(i).systemLocation());
                ports.removeAt(i--);
            }

//            qDebug() << tag << "Voltage" << info.voltage;
//            qDebug() << tag << "phase_current" << info.phaseCurrent;
//            qDebug() << tag << "neutral_current" << info.neutralCurrent;
        }
    }

    return;

//    ps.disablePowerOutput();

//    ps.setVoltage(0, 240, 0);
//    ps.setCurrent(0, 1, 0);

//    ps.enablePowerOutput();

//    MeterCommunication::delay(5000);

//    for(int i=0; i<ports.count(); i++){
//        bool isAuthenticated = authenticate(ports.at(i).systemLocation());
//        qDebug() << tag << "authenticate LC" << ports.at(i).systemLocation() << isAuthenticated;
//        if(isAuthenticated){
//            qDebug() << tag << "errorTestLowCurrent port " << ports.at(i).systemLocation();
//            settings.setValue("errorTestLowCurrentPort", ports.at(i).systemLocation());
//            foundPorts << QString("errorTestLowCurrentPort %1").arg(ports.at(i).systemLocation());
//            ports.removeAt(i--);
//            break;
//        }
//    }

//    ps.disablePowerOutput();

//    ps.setVoltage(0, 0, 240);
//    ps.setCurrent(0, 0, 1);

//    ps.enablePowerOutput();

//    MeterCommunication::delay(5000);

//    for(int i=0; i<ports.count(); i++){
//        bool isAuthenticated = authenticate(ports.at(i).systemLocation());
//        qDebug() << tag << "authenticate HC" << ports.at(i).systemLocation() << isAuthenticated;
//        if(isAuthenticated){
//            qDebug() << tag << "errorTestHighCurrent port " << ports.at(i).systemLocation();
//            settings.setValue("errorTestHighCurrentPort", ports.at(i).systemLocation());
//            foundPorts << QString("errorTestHighCurrentPort %1").arg(ports.at(i).systemLocation());
//            break;
//        }
//    }

//    qDebug() << tag << foundPorts;
//    ps.disablePowerOutput();
    //    settings.sync();
}

bool DetectSerialPorts::authenticate(QString port)
{
    MeterCommunication comm(port, tag, this, true);
    bool result = comm.authenticate();
    return result;
}

bool DetectSerialPorts::getVI(QString port, VIInfo &info)
{
    QByteArray streamData, crcData;
    QDataStream stream (&streamData, QIODevice::WriteOnly);

    stream << (quint8) 0x05;//data type
    stream << (quint8) 0x01;//read
    stream << (quint8) 0x05;//memory
    stream << (quint32) 0x01;//address;
    stream << (quint8) 0x0C;//size of dummy data
    for(int i=0;i<12;i++)
        stream << (quint8)0x00;

    streamData = MeterCommunication::createPacket(streamData, 3);

    QSerialPort serial(port);

    if(!serial.open(QIODevice::ReadWrite))
        return false;

    serial.write(streamData);

//    qDebug() << tag << "T1" <<
    serial.waitForBytesWritten(1000);
//    qDebug() << tag << "T2" <<
    serial.waitForReadyRead(10000);

    delay(5000);

    streamData = serial.readAll();
    qDebug() << tag << "s1" << streamData.size() << streamData.toHex();

    QDataStream streamIn (&streamData, QIODevice::ReadOnly);
    streamIn.setFloatingPointPrecision(QDataStream::SinglePrecision);
    streamIn.setByteOrder(QDataStream::LittleEndian);
    streamIn.skipRawData(6);
    streamIn >> info.voltage;
    streamIn >> info.phaseCurrent;
    streamIn >> info.neutralCurrent;

    return true;
}

void DetectSerialPorts::delay(int ms)
{
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(ms);

    // Process events and wait for the timer to timeout
    while (timer.isActive()) {
        QCoreApplication::processEvents();
    }
}
