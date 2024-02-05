#ifndef VIRTUALPORT_H
#define VIRTUALPORT_H

#include <QObject>
#include <QSocketNotifier>
#include <QDebug>

#include <QCoreApplication>
#include <QSocketNotifier>
#include <QFile>
#include <QDebug>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <pty.h>
#include "sys/socket.h"


class PTYReader : public QObject
{
    Q_OBJECT

public:
    PTYReader(QObject *parent = nullptr) : QObject(parent)
    {
        // Open the master side of the pseudo-terminal
        master = posix_openpt(O_RDWR | O_NOCTTY);
        grantpt(master);
        unlockpt(master);

        // Redirect the master side to a socket
        int socketPair[2];
        if (socketpair(AF_LOCAL, SOCK_STREAM, 0, socketPair) == -1) {
            qCritical("Failed to create socket pair.");
            QCoreApplication::exit(EXIT_FAILURE);
        }
        dup2(socketPair[0], master);
        close(socketPair[0]);

        // Create a QSocketNotifier to monitor the socket
        socketNotifier = new QSocketNotifier(socketPair[1], QSocketNotifier::Read, this);
        connect(socketNotifier, &QSocketNotifier::activated, this, &PTYReader::readData);
    }

public slots:
    void readData()
    {
        char buffer[256];
        ssize_t bytesRead = read(master, buffer, sizeof(buffer));

        if (bytesRead > 0) {
            qDebug() << "Received:" << QByteArray(buffer, bytesRead);
        } else if (bytesRead == 0) {
            qDebug() << "Connection closed.";
            QCoreApplication::exit(EXIT_SUCCESS);
        } else {
            qCritical() << "Error reading from pseudo-terminal.";
            QCoreApplication::exit(EXIT_FAILURE);
        }
    }

private:
    int master;
    QSocketNotifier *socketNotifier;
};

class VirtualPort : public QObject
{
    Q_OBJECT
public:
    explicit VirtualPort(int masterFd, QObject *parent = nullptr);
    ~VirtualPort();

public slots:
    void readData();

signals:

private:
    int masterFd;
    QSocketNotifier *notifier;

};

class VirtualPortReader : public QObject
{
    Q_OBJECT

public:
    VirtualPortReader(QObject *parent = nullptr) : QObject(parent)
    {
        // Open the master side of the pseudo-terminal
        if (openpty(&master, &slave, NULL, NULL, NULL) == -1) {
            qCritical("Failed to open pseudo-terminal.");
//            qApp->exit(EXIT_FAILURE);
        }

        // Create a QSocketNotifier to monitor the master side for data
        notifier = new QSocketNotifier(master, QSocketNotifier::Read, this);
        connect(notifier, &QSocketNotifier::activated, this, &VirtualPortReader::readData);

        qDebug() << "Virtual serial port:" << ttyname(slave);
    }

public slots:
    void readData()
    {
        char buffer[256];
        ssize_t bytesRead;

        // Read data from the master side
        bytesRead = read(master, buffer, sizeof(buffer));

        if (bytesRead > 0) {
            qDebug() << "Received:" << QByteArray(buffer, bytesRead);
        } else if (bytesRead == 0) {
            qCritical() << "Connection closed.";
            qApp->exit(EXIT_SUCCESS);
        } else {
            qCritical() << "Error reading from pseudo-terminal.";
            qApp->exit(EXIT_FAILURE);
        }
    }

private:
    int master;
    int slave;
    QSocketNotifier *notifier;
};

//class PseudoTerminalReader : public QObject
//{
//    Q_OBJECT

//public:
////    PseudoTerminalReader(int masterFd, QObject *parent = nullptr) : QObject(parent), masterFd(masterFd)
////    {
////        // Create a QSocketNotifier to monitor the master side of the pseudo-terminal for readability
////        notifier = new QSocketNotifier(masterFd, QSocketNotifier::Read, this);
////        connect(notifier, &QSocketNotifier::activated, this, &PseudoTerminalReader::readData);
////    }

////    ~PseudoTerminalReader()
////    {
////        // Close the master side of the pseudo-terminal when the object is destroyed
////        close(masterFd);
////    }

//public slots:

//private:
//    int masterFd;
//    QSocketNotifier *notifier;
//};


#endif // VIRTUALPORT_H
