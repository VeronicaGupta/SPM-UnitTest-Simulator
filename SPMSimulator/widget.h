#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include "spmsimulator.h"
#include "powersourcesimulator.h"
#include "metersimulator.h"
#include <QProcess>
#include "ui_widget.h"
#include "assigntest.h"
#include "virtualport.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    SPMSimulator* spmSimulator;
    PowerSourceSimulator* powerSourceSimulator;
    PowerSourceSimulator* cmriSimulator;
    QList<MeterSimulator*> meterSimulators;
};
#endif // WIDGET_H
