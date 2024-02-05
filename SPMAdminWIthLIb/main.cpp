#include "widget.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QSettings>
#include <QFile>
#include <QDir>
#include "plccontroller.h"
#include "meterinfo.h"
#include "wizplcliveview.h"

#include "detectserialports.h"

//AB0KH9LB -- opeto5
//AB0KTZ7A -- opto 6
//AB0KH9LB -- nicsync
//#define RUN_WIDGET

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
    a.setApplicationName("SPMAdmin");

    QString logPath = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0);
    QDir().mkpath(logPath);
    logPath += "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    logFile.setFileName(logPath + ".log");
    logFile.open(QIODevice::WriteOnly | QIODevice::Text);

    qDebug() << logPath;

    QSettings settings;

    DetectSerialPorts detect;
    detect.firstTimeDetect();

#ifdef RUN_WIDGET
//    WizPLCLiveView w;
    Widget w;
    w.show();
#else
    qmlRegisterType<PLCController>("SPMControls", 1, 0, "PLCController");
    qmlRegisterType<MeterInfo>("SPMControls", 1, 0, "MeterInfo");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &a, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
#endif
    return a.exec();
    logFile.close();
}
