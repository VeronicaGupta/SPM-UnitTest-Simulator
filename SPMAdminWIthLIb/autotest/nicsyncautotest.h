#ifndef NICSYNCAUTOTEST_H
#define NICSYNCAUTOTEST_H

#include <QObject>
#include <QDateTime>
#include "nicsynctest.h"

class NicSyncAutoTest : public QObject
{
    Q_OBJECT
public:
    explicit NicSyncAutoTest(QString portName, QObject *parent = nullptr);

public slots:
    void starTest();
    void nicSyncTestResult(NicSyncTestResult* result);


signals:

private:
    NicSyncTest* nicSync;
    QDateTime time;
    int passCounter;
    int failCounter;

};

#endif // NICSYNCAUTOTEST_H
