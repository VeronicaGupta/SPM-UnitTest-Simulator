#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//     Map the enum values to their string representations
    static const QMap<EnCustomer, QString> customerMap = {
        {CUST_APDCL, "APDCL"},
        {CUST_INTELLI, "Intelli"},
        {CUST_JAMMU, "Jammu"},
        {CUST_KASHMIR, "Kashmir"},
        {CUST_PANASONIC, "Panasonic"},
        {CUST_APARVA, "Aparva"},
        {CUST_DEMO, "Demo"},
        {CUST_OTHER, "Other"},
        {CUST_NOT_FOUND, "Not Found"},
    };

    // Add enum values to the combo box
    ui->nicsyncTestCustomer->addItems(customerMap.values());

    // Set the default index
    ui->nicsyncTestCustomer->setCurrentIndex(6);

    on_refreshBtn_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::unitTestUpdateDisplay(QString testUpdate)
{
    ui->DialogLog->setText(ui->DialogLog->toPlainText() + "\n" + QDateTime::currentDateTime().toString() + " :: " +testUpdate + "\n");
}

void MainWindow::on_functionTestBtn_clicked()
{
    FunctionAutoTest* ftTest = new FunctionAutoTest(ui->serialPort->currentText(),
                                                    ui->functiontestCycle->text().toInt(), this);
    ftTest->unitTest();
}

void MainWindow::on_nicsyncTestBtn_clicked()
{
    NicsyncAutoTest* nsTest = new NicsyncAutoTest(ui->serialPort->currentText(),
                                                  ui->nicsynctestCycle->text().toInt(),
                                                  ui->nicsyncTestSerial->text(),
                                                  ui->nicsyncTestCustomer->currentIndex(),this);
    nsTest->unitTest();
}


void MainWindow::on_powerStablisationTestBtn_clicked()
{
    QStringList voltages= {ui->phase1Voltage->text(), ui->phase2Voltage->text(), ui->phase3Voltage->text()};
    QStringList currents= {ui->phase1Current->text(), ui->phase2Current->text(), ui->phase3Current->text()};

    PowerStablisationAutoTest* psTest = new PowerStablisationAutoTest(ui->psSerialPort->currentText(),
                                                                      ui->powerStablisationCycle->text().toInt(),
                                                                      ui->cmriSerialPort->currentText(),
                                                                      voltages,
                                                                      currents, this);
    psTest->unitTest();
}

void MainWindow::on_refreshBtn_clicked()
{
    QDir lsDev("/dev/");
    lsDev.setNameFilters(QStringList() << "tty*");
    QStringList ports = lsDev.entryList(QDir::System);

    QStringList meterPortNames={};
    QStringList scannerPortNames={};

    for (const QString port : ports) {
        if (port.startsWith("ttyUSB")){
                meterPortNames.append(port.mid(0,10));
        }
    }

    ui->serialPort->clear();
    ui->serialPort->addItems(meterPortNames);
    ui->serialPort->setCurrentIndex(0);

    ui->psSerialPort->clear();
    ui->psSerialPort->addItems(meterPortNames);
    ui->psSerialPort->setCurrentIndex(0);

    ui->cmriSerialPort->clear();
    ui->cmriSerialPort->addItems(meterPortNames);
    ui->cmriSerialPort->setCurrentIndex(0);

    // when simulator is running
    if (meterPortNames.contains("ttyUSB100")){
        ui->serialPort->setCurrentIndex(meterPortNames.indexOf("ttyUSB100"));
    }
    if (meterPortNames.contains("ttyUSB107")){
        ui->psSerialPort->setCurrentIndex(meterPortNames.indexOf("ttyUSB107"));
    }
    if (meterPortNames.contains("ttyUSB108")){
        ui->cmriSerialPort->setCurrentIndex(meterPortNames.indexOf("ttyUSB108"));
    }

    if (meterPortNames.isEmpty()) {
        qDebug() << "Error: USB port not found.";
        unitTestUpdateDisplay("Error - No USB found!");
    }
}
