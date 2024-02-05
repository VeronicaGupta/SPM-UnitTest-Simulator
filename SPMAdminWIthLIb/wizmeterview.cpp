#include "wizmeterview.h"
#include "ui_wizmeterview.h"

WizMeterView::WizMeterView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WizMeterView)
{
    ui->setupUi(this);
    setStyleSheet("background-color: blue;");
    setAutoFillBackground(true);
}

WizMeterView::~WizMeterView()
{
    delete ui;
}

void WizMeterView::setHeaderInfo(QString headerInfo1, QString headerInfo2)
{
    ui->lblHeader1->setText(headerInfo1);
    ui->lblHeader2->setText(headerInfo2);
}

void WizMeterView::setMeterInfo(QString meterInfo1, QString meterInfo2)
{
    ui->frame->setStyleSheet(QString("background-color: %1;").arg(getColor(meterInfo1.at(0))));
    ui->lblMeterInfo1->setText(meterInfo1);
    ui->lblMeterInfo2->setText(meterInfo2);
}

QString WizMeterView::getColor(QChar status)
{
    QString outColor;
    if(status == "-")
        outColor = "skyblue";
    else if(status == "P")
        outColor = "green";
    else
        outColor = "red";

    return outColor;
}

