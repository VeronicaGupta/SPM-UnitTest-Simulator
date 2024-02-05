#ifndef CMRICONTROL_H
#define CMRICONTROL_H

#include <QObject>
#include <QSerialPort>

class CMRIControl : public QObject
{
    Q_OBJECT
public:
    explicit CMRIControl(QString portName, QObject *parent = nullptr);

public slots:
    void cmriReset();

    void readData();

signals:

private:
    QSerialPort* serial;
    QString tag;
};

#endif // CMRICONTROL_H
