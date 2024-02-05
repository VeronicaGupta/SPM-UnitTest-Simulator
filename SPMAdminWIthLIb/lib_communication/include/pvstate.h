#ifndef PVSTATE_H
#define PVSTATE_H

#include <QObject>
#include <QState>
#include <QTimer>

#include "pvfailtransition.h"

class PVState : public QState
{
    Q_OBJECT
public:
    explicit PVState(QState *parent = nullptr);

    void addFailTransition(const QObject *sender, const char *signal, QAbstractState *target, int timeoutDuration = 3000, int retry = 7);

public slots:
    void stateEntered();
    void stateExited();

signals:

private:
    PVFailTransition *t1;

};

#endif // PVSTATE_H
