#ifndef POWERSOURCESIMULATOR_H
#define POWERSOURCESIMULATOR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class PowerSourceSimulator : public QTcpServer
{
    Q_OBJECT
public:
    explicit PowerSourceSimulator(int portNumber, QObject *parent = nullptr);


protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int psCounter;

private slots:
    void readClientData();

    void clientDisconnected();
    void sendData(QTcpSocket *clientSocket, QByteArray data);

};

#endif // POWERSOURCESIMULATOR_H
