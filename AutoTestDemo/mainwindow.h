#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "functionautotest.h"
#include "nicsyncautotest.h"
#include "powerstablisationautotest.h"
#include <QDebug>
#include <QComboBox>
#include <QDir>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;

signals:
    void unitTestComplete(QString testUpdate);

private:
    QString tag;
//    MeterCommunication* meterCommunication;

private slots:
    void on_functionTestBtn_clicked();
    void on_nicsyncTestBtn_clicked();

    void on_powerStablisationTestBtn_clicked();

    void on_refreshBtn_clicked();

public slots:
    void unitTestUpdateDisplay(QString testUpdate);

};
#endif // MAINWINDOW_H
