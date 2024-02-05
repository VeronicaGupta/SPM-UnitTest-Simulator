#ifndef PVFAILTRANSITION_H
#define PVFAILTRANSITION_H

#include <QObject>
#include <QSignalTransition>
#include <QDateTime>
#include <QTimer>

class PVFailTransition : public QSignalTransition
{
    Q_OBJECT
public:
    explicit PVFailTransition(const QObject *object, const char* signal, int timeoutDuration, int retryCount, QObject *parent = nullptr);

public slots:
    void startTimer();
    void stopTimer();
    void timeout();

protected:
    bool eventTest(QEvent *e);

signals:
    void retry();
    void fail();

private:
    QTimer* timer;
    int passCounter;
    int retryCount;
    QDateTime lastDateTime;
};

#endif // PVFAILTRANSITION_H
