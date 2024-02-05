#ifndef SPMSIMULATOR_H
#define SPMSIMULATOR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QQueue>
#include <QDebug>

class PMessageLogger: public QMessageLogger
{
    Q_DISABLE_COPY(PMessageLogger)
public:
    Q_DECL_CONSTEXPR PMessageLogger(const char *file, int line, const char *function)
        : QMessageLogger(file, line, function, "default") {}


    QDebug debug() const;
    QDebug debug(CategoryFunction catFunc) const;

////    Q_DECL_CONSTEXPR PMessageLogger(const char *file, int line, const char *function, const char *category)
////        : context(file, line, function, category) {}

////    void debug(const char *msg, ...) const Q_ATTRIBUTE_FORMAT_PRINTF(2, 3);

//    void debug(const char *msg, ...) const ;

//    typedef const QLoggingCategory &(*CategoryFunction)();

//    QDebug debug(CategoryFunction catFunc) const;


//    QMessageLogContext context;
};

class SPMSimulator : public QObject
{
    Q_OBJECT
public:
    explicit SPMSimulator(QObject *parent = nullptr);

public slots:
    void newConnection() ;
    void readyRead();

    void sendStartTest();

    void initTestQueue();

signals:

private:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
    QQueue<int> runTestQueue;
    QByteArray lastStatus;

};

#endif // SPMSIMULATOR_H
