#ifndef AUTHENTICATEAUTOTEST_H
#define AUTHENTICATEAUTOTEST_H

#include <QObject>
#include "metercommunication.h"

class AuthenticateAutoTest : public QObject
{
    Q_OBJECT
public:
    explicit AuthenticateAutoTest(QObject *parent = nullptr);

public slots:
    void startTest();

signals:

private:
    MeterCommunication* comm;

};

#endif // AUTHENTICATEAUTOTEST_H
