#include "authenticateautotest.h"
#include <QTimer>
AuthenticateAutoTest::AuthenticateAutoTest(QObject *parent)
    : QObject{parent}
{
    comm = new MeterCommunication("/dev/ttyUSB3", "AAT", this, true);
}

void AuthenticateAutoTest::startTest()
{
    comm->authenticate();
    QTimer::singleShot(100, this, SLOT(startTest()));
}
