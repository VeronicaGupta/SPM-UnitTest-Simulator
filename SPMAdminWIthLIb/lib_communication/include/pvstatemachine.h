#ifndef PVSTATEMACHINE_H
#define PVSTATEMACHINE_H

#include <QStateMachine>
#include <QFinalState>

class PVStateMachine : public QStateMachine
{
    Q_OBJECT
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)
public:
    explicit PVStateMachine(QObject *parent = nullptr);

    void addState(QState *state, QString stateName = "");
    void addState(QFinalState *state);

public slots:
    void start();

    void setState(QString state);
    QString state();
    QString lastState();

    void removeAllStates();

signals:
    void stateChanged();

private:
    QString mState;
    QString mLastState;
};

#endif // PVSTATEMACHINE_H
