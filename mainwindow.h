#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "manometer.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_PBstart_clicked();
    void updateDat();

    void on_PBstop_clicked();    
    void serialOpen();
    void serialOpen1();
    void readData();
    void readData1();
    void on_horizontalSlider_valueChanged(int value);

    void on_fanSpeedE_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow      *ui;
    QSerialPort         *serial, *serial1;
    QTimer              *updateData, *updateUi;
    ManoMeter           *tempMeter1, *tempMeter2;
    ManoMeter           *humiMeter1, *humiMeter2;
    ManoMeter           *flowMeter1;

};

#endif // MAINWINDOW_H
