#include "widget.h"

#include <QApplication>
#include "virtualport.h"


extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <QCoreApplication>
#include <QDebug>
#include <QSocketNotifier>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
}

#include <QCoreApplication>
#include <QDebug>
#include <QSocketNotifier>
#include "sys/socket.h"



//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    PTYReader ptyReader;

//    return a.exec();
//}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    auto data = QByteArray::fromHex("02030600000188a803");
//    QByteArray data1 = QByteArray::fromHex("02030600000189");
//    qDebug() << "sendingData" << data.toHex() << data1.toHex() << data.startsWith(data1);



//    int master, slave;
//     char *slave_name;

//     // Step 1: Open master PTY
//     if ((master = posix_openpt(O_RDWR)) == -1) {
//         qDebug() << "posix_openpt";
//         exit(EXIT_FAILURE);
//     }

//     // Step 2: Grant access to the slave PTY
//     if (grantpt(master) == -1) {
//         perror("grantpt");
//         close(master);
//         exit(EXIT_FAILURE);
//     }

//     // Step 3: Unlock the slave PTY
//     if (unlockpt(master) == -1) {
//         perror("unlockpt");
//         close(master);
//         exit(EXIT_FAILURE);
//     }

//     // Step 4: Get the name of the slave PTY
//     if ((slave_name = ptsname(master)) == NULL) {
//         perror("ptsname");
//         close(master);
//         exit(EXIT_FAILURE);
//     }

//     qDebug() << "Slave PTY: " << slave_name;
////     printf("Slave PTY: %s\n", slave_name);

//     // Now you can use master and slave as file descriptors for the master and slave PTYs.

//     // Example: Write data to master, and it will be readable from the slave.
//     char buffer[256];
//     ssize_t bytesRead;

////     QIODevice d()

//     while (1) {
//         bytesRead = read(master, buffer, sizeof(buffer));
//         if(bytesRead <=0)
//             continue;
//         qDebug() << "Received: " << bytesRead << buffer;
////         printf("Received: %.*s", (int)bytesRead, buffer);
//         write(master, "Hello, PTY!\n", 12);
//     }

//     qDebug() << "exited";
//     // Cleanup: Close file descriptors
//     close(master);



//    VirtualPort port(1);
    Widget w;
//    w.show();
    return a.exec();
}
