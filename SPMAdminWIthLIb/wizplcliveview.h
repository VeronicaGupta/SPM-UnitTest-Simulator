#ifndef WIZPLCLIVEVIEW_H
#define WIZPLCLIVEVIEW_H

#include <QWidget>
#include "wizmeterview.h"
#include "plccontroller.h"

namespace Ui {
class WizPLCLiveView;
}

class WizPLCLiveView : public QWidget
{
    Q_OBJECT

public:
    explicit WizPLCLiveView(QWidget *parent = nullptr);
    ~WizPLCLiveView();

private slots:
    void commandReceived(QString command);
    QColor getColor(QChar status);

    void on_btnStart_clicked();

    void on_btnTest1_clicked();

    void on_btnTest2_clicked();

private:
    Ui::WizPLCLiveView *ui;

    WizMeterView* meterView[12];
    PLCController* plcController;
};

#endif // WIZPLCLIVEVIEW_H
