#include "virtualport.h"

extern "C" {
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
}
#include <QDebug>
#include <QFile>
#include <QDebug>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

VirtualPort::VirtualPort(int masterFd, QObject *parent) : QObject(parent)
{

}

VirtualPort::~VirtualPort()
{
    // Close the master side of the pseudo-terminal when the object is destroyed
    close(masterFd);
}

void VirtualPort::readData()
{
    // Read data from the master side of the pseudo-terminal
    char buffer[256];
    ssize_t bytesRead = read(masterFd, buffer, sizeof(buffer));
    if (bytesRead > 0)
    {
        qDebug() << "Received:" << QByteArray(buffer, bytesRead);
    }
}
