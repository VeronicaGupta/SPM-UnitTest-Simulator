#ifndef LASERCOMMUNICATION_H
#define LASERCOMMUNICATION_H

#include <QObject>
#include <QVariantList>
#include <QTcpSocket>
#include <QTimer>

#include "meterinfo.h"
#include "pvstatemachine.h"

class LaserCommunication : public QObject
{
    Q_OBJECT
public:
    explicit LaserCommunication(QObject *parent = nullptr);

public slots:
    void connectLaser();
    bool startMeterEngraving(QString loaNumber, QString loaNumber2, QString mfgDate, QString serialNumber);
    bool startEngraving(QStringList parameterList);

private slots:
    void connectToHost();
    void printState();
    void readyRead();
    void startMarking();

signals:
    void laserEngraveResult(LaserEngraveResult* testResult);

signals:


private:
    QTcpSocket* socket;
    PVStateMachine* stateMachine;
    QState stConnecting;
    QState stTimeOut;
    QState stConnectError;
    QState stConnected;
    QState stDisconnected;

    QTimer tmrTimeOut;
    QString tag;

    LaserEngraveResult engraveResult;
};

#endif // LASERCOMMUNICATION_H
