#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

//    int master, slave;
//    char *slaveName;

//    // Open the master side of the pseudo-terminal
//    if (openpty(&master, &slave, NULL, NULL, NULL) == -1)
//    {
//        perror("openpty");
//        return EXIT_FAILURE;
//    }

//    // Get the name of the slave side
//    if ((slaveName = ttyname(slave)) == NULL)
//    {
//        perror("ttyname");
//        close(master);
//        close(slave);
//        return EXIT_FAILURE;
//    }

//    qDebug() << "Virtual serial port:" << slaveName;

    // Create a QCoreApplication event loop
//    PseudoTerminalReader reader(master);

    VirtualPortReader port;

    spmSimulator = new SPMSimulator(this);
    powerSourceSimulator = new PowerSourceSimulator(6007, this);
    cmriSimulator = new PowerSourceSimulator(6008, this);

    for(int i=0; i<7; i++){
        auto meterSimulator = new MeterSimulator(6000 + i, i, this);
        meterSimulators.append(meterSimulator);
    }

    sleep(2);

    qDebug() << "\nPort generation started..";

    QProcess process;
    process.setWorkingDirectory("/home/parnika/kimbal/SPMSimulator");
    process.start("sudo", QStringList() << "./create_ports.sh"); // MODIFY // sudo visudo // your_user ALL=(ALL) NOPASSWD: /path/to/create_ports.sh


    if (!process.waitForStarted()) {
        qDebug() << "Error starting process:" << process.errorString();
    }

    if (!process.waitForFinished()) {
        qDebug() << "Error waiting for process to finish:" << process.errorString();
    }

    QByteArray output = process.readAllStandardOutput();
    QByteArray errorOutput = process.readAllStandardError();
    qDebug() << "Standard Output:" << QString::fromUtf8(output);
    qDebug() << "Standard Error:" << QString::fromUtf8(errorOutput);
    qDebug() << "\nPORTS GENERATED..\n";


//#ifdef SIMULATOR_MODE
//    settings.setValue("powerSourcePort", "/dev/ttyUSB107");6007
//    settings.setValue("cmriSourcePort", "/dev/ttyUSB108");6008
//    settings.setValue("functionTestPort", "/dev/ttyUSB100"); 6000
//    settings.setValue("phaseCalibrationPort", "/dev/ttyUSB101");6001
//    settings.setValue("neutralCalibrationPort", "/dev/ttyUSB102");6002
//    settings.setValue("errorTestStartingCurrentPort", "/dev/ttyUSB103");6003
//    settings.setValue("errorTestLowCurrentPort", "/dev/ttyUSB104");6004
//    settings.setValue("errorTestHighCurrentPort", "/dev/ttyUSB105");6005
//    settings.setValue("networkSycnPort", "/dev/ttyUSB106");6006

}

Widget::~Widget()
{
    delete ui;
}

