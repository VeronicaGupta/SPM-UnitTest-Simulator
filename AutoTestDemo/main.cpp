#include "mainwindow.h"

#include <QApplication>
#include <QStandardPaths>
#include <QSettings>
#include <QFile>
#include <QDir>

QFile logFile;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s \n", localMsg.constData());
        logFile.write("Debug");
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s \n", localMsg.constData());
        logFile.write("Info: ");
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s \n", localMsg.constData());
        logFile.write("Warning: ");
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s \n", localMsg.constData());
        logFile.write("Critical: ");
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s \n", localMsg.constData());
        logFile.write("Fatal: ");
        logFile.write(localMsg);
        logFile.write("\r\n");
        logFile.flush();
        abort();
    }

    logFile.write(localMsg);
    logFile.write("\r\n");
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput); // Install the handler

    QApplication a(argc, argv);

    a.setOrganizationName("Kimbal");
    a.setApplicationName("AutoTest");

    QString logPath = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
    QDir().mkpath(logPath);
    logPath += "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    logFile.setFileName(logPath + ".log");
    logFile.open(QIODevice::WriteOnly | QIODevice::Text);

    qDebug() << "LOGPATH" << logPath;

    QSettings settings;

    MainWindow w;
    w.show();
    return a.exec();
    logFile.close();
}
