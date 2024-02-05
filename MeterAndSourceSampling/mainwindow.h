#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <powersource.h>
#include <functiontest.h>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QTimer>
#include <QTimerEvent>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startBtn_clicked();
    void on_refreshBtn_clicked();
    void on_stopBtn_clicked();

    void get_meter_values(FunctionTestResult* result);
    void get_source_valuesV(QString voltage);
    void get_source_valuesI(QString current);

protected:
    void timerEvent(QTimerEvent *event) override;


public slots:
    void unitTestUpdateDisplay(QString testUpdate);

    void updatePlot()
    {
        // Update variables (replace with your logic)
        meterVoltage += 0.1;
        sourceVoltage[0] = qrand() % 100 + 1;  // Random value between 1 and 100
        meterCurrent += 0.1;
        sourceCurrent[0] = qrand() % 50 + 1;  // Random value between 1 and 50

        // Plot the variables
        plotPoints();
    }

private:
    Ui::MainWindow *ui;

    PowerSource* powerSource;
    FunctionTest* functionTest;

    QString updateString;

    int timerId;
    double sum;
    QGraphicsScene *scene;
    QTimer *timer;
    float meterVoltage;
    float sourceVoltage[3];
    float meterCurrent;
    float sourceCurrent[3];

    void plotAxes()
    {
        // X-axis
        QGraphicsLineItem *xAxis = new QGraphicsLineItem(0, 0, 500, 0);
        scene->addItem(xAxis);

        // Y-axis
        QGraphicsLineItem *yAxis = new QGraphicsLineItem(0, 0, 0, -300);
        scene->addItem(yAxis);

        // X-axis label
        QGraphicsTextItem *xLabel = new QGraphicsTextItem("Time");
        xLabel->setPos(500, 10);
        scene->addItem(xLabel);

        // Y-axis label
        QGraphicsTextItem *yLabel = new QGraphicsTextItem("Values");
        yLabel->setPos(-30, -300);
        scene->addItem(yLabel);
    }

    void plotPoints()
    {
        // Clear the previous items in the scene
//        scene->clear();

        // Plot axes and labels
        plotAxes();

        // Plot meterVoltage and sourceVoltage[0]
        plotVariable(meterVoltage, 0, 0, Qt::blue, "Meter Voltage");
        plotVariable(sourceVoltage[0], 0, 1, Qt::red, "Source Voltage");

        // Plot meterCurrent and sourceCurrent[0]
        plotVariable(meterCurrent, 1, 0, Qt::green, "Meter Current");
        plotVariable(sourceCurrent[0], 1, 1, Qt::black, "Source Power");

        // Plot points for each variable
        plotPoint(meterVoltage, 0, 0, Qt::blue);
        plotPoint(sourceVoltage[0], 0, 1, Qt::red);
        plotPoint(meterCurrent, 1, 0, Qt::green);
        plotPoint(sourceCurrent[0], 1, 1, Qt::black);
    }

    void plotVariable(float value, int row, int column, QColor color, const QString& label)
    {
        QGraphicsLineItem *lineItem = new QGraphicsLineItem(0, 0, 0, -value);
        lineItem->setPen(QPen(color));
        lineItem->setPos(column * 200, -row * 150);  // Adjust position based on row and column
        scene->addItem(lineItem);

        QGraphicsTextItem *textItem = new QGraphicsTextItem(label);
        textItem->setFont(QFont("Arial", 10, QFont::Bold));
        textItem->setDefaultTextColor(color);
        textItem->setPos(column * 200, -row * 150 - 20);  // Adjust position above the line
        scene->addItem(textItem);
    }

    void plotPoint(float value, int row, int column, QColor color)
   {
       // Plot each point as a small circle
       QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem(0, 0, 5, 5);
       pointItem->setBrush(color);
       pointItem->setPos(column * 200, -row * 150 - value);  // Adjust position based on row, column, and value
       scene->addItem(pointItem);

       // Display the value next to the point
       QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::number(value));
       textItem->setDefaultTextColor(color);
       textItem->setPos(column * 200 + 10, -row * 150 - value - 10);  // Adjust position for text
       scene->addItem(textItem);
   }

};
#endif // MAINWINDOW_H
