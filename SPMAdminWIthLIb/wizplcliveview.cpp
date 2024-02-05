#include "wizplcliveview.h"
#include "ui_wizplcliveview.h"



WizPLCLiveView::WizPLCLiveView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WizPLCLiveView)
{
    ui->setupUi(this);
    meterView[0] = ui->meterView0;
    meterView[1] = ui->meterView1;
    meterView[2] = ui->meterView2;
    meterView[3] = ui->meterView3;
    meterView[4] = ui->meterView4;
    meterView[5] = ui->meterView5;
    meterView[6] = ui->meterView6;
    meterView[7] = ui->meterView7;
    meterView[8] = ui->meterView8;
    meterView[9] = ui->meterView9;
    meterView[10] = ui->meterView10;
    meterView[11] = ui->meterView11;

    for(int i=0; i< 12; i++)
        meterView[i]->setHeaderInfo(QString("S-%1 (%2)").arg(i).arg(QChar('A' + i)));

    plcController = new PLCController(this);
    connect(plcController, SIGNAL(commandReceived(QString)), this, SLOT(commandReceived(QString)));
}

WizPLCLiveView::~WizPLCLiveView()
{
    delete ui;
}

void WizPLCLiveView::commandReceived(QString command)
{
    if(command.startsWith("Start_test")){

    }
    else if(command.startsWith("Part_Result_update")){
        QString partStatus = command.split(" ", QString::SkipEmptyParts).last();

        if(partStatus.length() != 12)
            return;


        for(int i=0; i<partStatus.length(); i++){
            QChar c = partStatus.at(i);
//            auto color = getColor(c);
            int index;
            if(c != "-")
                index = plcController->meterStageQueue.at(11-i)->meterTmpSerialNumber;
            meterView[i]->setMeterInfo(c, QString("%1").arg(index));
        }
    }
}

QColor WizPLCLiveView::getColor(QChar status)
{
    return QColor();
}

void WizPLCLiveView::on_btnStart_clicked()
{
    plcController->startProduction("AB", "LO", "ll", "A", 1, 1000);
//    commandReceived("Part_Result_update P--APPPPPPPP");
}


void WizPLCLiveView::on_btnTest1_clicked()
{
    static int currentMeterCount ;
    MeterInfo* meter = new MeterInfo(currentMeterCount++);
    meter->meterTmpSerialNumber = currentMeterCount;
    plcController->meterStageQueue.enqueue(meter);
    qDebug() << plcController->meterStageQueue.count() << plcController->meterStageQueue.length();
}

void WizPLCLiveView::on_btnTest2_clicked()
{
    plcController->meterStageQueue.dequeue();
    qDebug() << plcController->meterStageQueue.count() << plcController->meterStageQueue.length();
}
