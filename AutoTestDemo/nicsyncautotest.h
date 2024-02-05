#ifndef NICSYNCAUTOTEST_H
#define NICSYNCAUTOTEST_H

#include <QObject>
#include "nicsynctest.h"
#include "QDebug"

class MainWindow;

class NicsyncAutoTest : public QObject
{
    Q_OBJECT
public:
    explicit NicsyncAutoTest(QString portName, int N, QString serial, int customer, QObject *parent = nullptr);

public slots:
    void unitTest();

private slots:
    void nicSyncTestResult(NicSyncTestResult* result);

signals:
    void testComplete();
    void unitTestUpdate(QString testUpdate);


private:
    NicSyncTest* nicSyncTest;

    int passCounter;
    int failCounter;
    int testCounter;
    QString PORTNAME;
    qint32 MAXTESTS;
    QString TESTNAME;
    QString tag;

    QString Serial;
    EnCustomer Customer;

};

#endif // NICSYNCAUTOTEST_H
