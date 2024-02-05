#ifndef WIZMETERVIEW_H
#define WIZMETERVIEW_H

#include <QWidget>

namespace Ui {
class WizMeterView;
}

class WizMeterView : public QWidget
{
    Q_OBJECT

public:
    explicit WizMeterView(QWidget *parent = nullptr);
    ~WizMeterView();

    void setHeaderInfo(QString headerInfo1, QString headerInfo2 = QString());
    void setMeterInfo(QString meterInfo1, QString meterInfo2 = QString());

    QString getColor(QChar status);

private:
    Ui::WizMeterView *ui;
};

#endif // WIZMETERVIEW_H
