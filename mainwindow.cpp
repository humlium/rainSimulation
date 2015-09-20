#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QtSql/QSql>

#include "manometer.h"

int i=0;
QString str,str11, temp1, temp2, humi1, humi2, flow1, pv03, pv05,pv07, pv10, pv25, pv50;
QStringList list, list1,list11;
QFile file("out.csv");
QTextStream out(&file);
QByteArray serialData,serialData1, command="F";



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    serial1 = new QSerialPort(this);
    
////// set timer ////////////////
    updateData = new QTimer(this);
    updateData->setInterval(2000);
///// end set timer ///////////////

///// set gauge /////////////////////
    tempMeter1 = new ManoMeter(this);
    tempMeter2 = new ManoMeter(this);
    humiMeter1 = new ManoMeter(this);
    humiMeter2 = new ManoMeter(this);
    flowMeter1 = new ManoMeter(this);
    tempMeter1->setMaximum(100);
    tempMeter1->setNominal(60);
    tempMeter1->setCritical(80);
    tempMeter2->setMaximum(100);
    tempMeter2->setNominal(60);
    tempMeter2->setCritical(80);
    humiMeter1->setMaximum(100);
    humiMeter1->setNominal(60);
    humiMeter1->setCritical(80);
    humiMeter2->setMaximum(100);
    humiMeter2->setNominal(60);
    humiMeter2->setCritical(80);
    flowMeter1->setMaximum(2);
    flowMeter1->setNominal(1.0);
    flowMeter1->setCritical(1.5);
    ui->templayout1->addWidget(tempMeter1);
    ui->templayout2->addWidget(tempMeter2);
    ui->flowlayout->addWidget(flowMeter1);
    ui->humilayout1->addWidget(humiMeter1);
    ui->humilayout2->addWidget(humiMeter2);
    tempMeter1->setSuffix("\u00B0c");
    tempMeter2->setSuffix("\u00B0c");
    humiMeter1->setSuffix(" %");
    humiMeter2->setSuffix(" %");
    flowMeter1->setSuffix(" m/sec");


//// end set gauge ///////////////////////////

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
           return;
    ui->fanSpeedE->setText("0");
    serialOpen();

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(serial1, SIGNAL(readyRead()), this, SLOT(readData1()));
    connect(updateData, SIGNAL(timeout()), this, SLOT(updateDat()));

    ui->label_6->setStyleSheet("background-color: blue;");
    ui->label_6->setText("<font color='white'>Gauge</font>");
    ui->label_8->setStyleSheet("background-color: blue;");
    ui->label_8->setText("<font color='white'>Airborne Particle</font>");
    ui->label_9->setStyleSheet("background-color: blue;");
    ui->label_9->setText("<font color='white'>Machine Status</font>");
    ui->label_16->setStyleSheet("background-color: blue;");
    ui->label_16->setText("<font color='white'>Receive Data Monitor</font>");
    ui->plainTextEdit->appendPlainText("temp1,temp2,humi1,humi2,flow,p03,p05,p07,p10,p2,p50");

}

MainWindow::~MainWindow()
{
    updateData->stop();
    serial->close();
    delete ui;
}

void MainWindow::on_PBstart_clicked()
{
    ui->PBstart->setDisabled(true);
    updateData->start();
    ui->labelStatus->setText("Run");
    //int val = ui->horizontalSlider->value() + 1;
    //if(val<=100) ui->fanSpeedE->setText(QString::number(val));
}

void MainWindow::on_PBstop_clicked()
{
    updateData->stop();
    ui->labelStatus->setText("Stop");
    ui->PBstart->setEnabled(true);
}

void MainWindow::readData()
{
    QByteArray tmp = serial->readAll();
    if(tmp.endsWith('\n')){
        serialData.append(tmp);

            str = serialData;
            list = str.split(",");
            if(list.length() >= 5){
            /*ui->plainTextEdit->appendPlainText(QString::number(list[0].toFloat(), 'f', 2)+","+QString::number(list[1].toFloat(),'f',2)
                    +","+QString::number(list[2].toFloat(), 'f', 2)+","+QString::number(list[3].toFloat(),'f',2)
                    +","+QString::number(list[4].toFloat(), 'f', 2)
                    +","+QString::number(list[5].toFloat(),'f',2)
                    +","+QString::number(list[6].toFloat(), 'f', 2)+","+QString::number(list[7].toFloat(),'f',2)
                    +","+QString::number(list[8].toFloat(), 'f', 2)+","+QString::number(list[9].toFloat(),'f',2)
                    +","+ui->p03->text()+","+ui->p05->text()+","+ui->p07->text()
                    +","+ui->p10->text()+","+ui->p25->text()+","+ui->p50->text());*/
                tempMeter1->setValue(list[0].toFloat());
                tempMeter2->setValue(list[1].toFloat());
                humiMeter1->setValue(list[2].toFloat());
                humiMeter2->setValue(list[3].toFloat());
                flowMeter1->setValue(list[4].toFloat());
                str11 = serialData;
                serialData.clear();
            }

    }else serialData.append(tmp);
}

void MainWindow::readData1()
{
    QByteArray tmp = serial1->readAll();
    QString tmp1,tmp3;
    QStringList tmp2;
    if(tmp.endsWith('\x0A')){
        serialData1.append(tmp);
        tmp3 = serialData1.left(serialData1.length()-1);
        list1 = tmp3.split('\x0A');
        //ui->label_2->setText(QString::number(list1.length()));
        //ui->plainTextEdit->appendPlainText(tmp3);
        if(list1.length() >= 20) {
            //if(serialData1.at(0)=='*')
            //{
                if(ui->labelStatus->text()!="Run") ui->labelStatus->setText("Run");
                tmp1 = list1[10];
                tmp2 = tmp1.split(" ");
                //if(tmp2[1]=="0.3"){
                ui->p03->setText(tmp2[tmp2.length()-1]);

                tmp1 = list1[11];
                tmp2 = tmp1.split(" ");
                ui->p05->setText(tmp2[tmp2.length()-1]);

                tmp1 = list1[12];
                tmp2 = tmp1.split(" ");
                ui->p07->setText(tmp2[tmp2.length()-1]);

                tmp1 = list1[13];
                tmp2 = tmp1.split(" ");
                ui->p10->setText(tmp2[tmp2.length()-1]);

                tmp1 = list1[14];
                tmp2 = tmp1.split(" ");
                ui->p25->setText(tmp2[tmp2.length()-1]);

                tmp1 = list1[15];
                tmp2 = tmp1.split(" ");
                ui->p50->setText(tmp2[tmp2.length()-1]);

                list11 = str11.split(",");

                if(list11.length() >= 5){
                    ui->plainTextEdit->appendPlainText(QString::number(list11[0].toFloat(), 'f', 2)+","+QString::number(list11[1].toFloat(),'f',2)
                            +","+QString::number(list11[2].toFloat(), 'f', 2)+","+QString::number(list11[3].toFloat(),'f',2)
                            +","+QString::number(list11[4].toFloat(), 'f', 2)
                            +","+ui->p03->text()+","+ui->p05->text()+","+ui->p07->text()
                            +","+ui->p10->text()+","+ui->p25->text()+","+ui->p50->text()
                            +","+list1[4]);

                }
               // } else ui->labelStatus->setText("Data Wrong");
            //}else ui->labelStatus->setText("Data Wrong");
            serialData1.clear();
        }
    }else serialData1.append(tmp);
}

void MainWindow::updateDat()
{
    if ((command != "I")&&(command != "H")) command.append(ui->horizontalSlider->value());
    serial->write(command);
    //ui->labelStatus->setText(command);
    command = "F";
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->fanSpeedE->setText(QString("%1").arg(value));
}

void MainWindow::on_fanSpeedE_textChanged(const QString &arg1)
{
    ui->horizontalSlider->setValue(arg1.toInt());
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("save file"), "/media","csv file(*.csv)");
    QFile f( filePath );
    f.open( QIODevice::WriteOnly );
    f.write(ui->plainTextEdit->toPlainText().toUtf8());
    f.close();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(updateData->isActive()) command = "I";
    else serial->write("I");
}

void MainWindow::serialOpen()
{
    serial->setPortName("ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if(serial->open(QIODevice::ReadWrite)){
        ui->labelStatus->setText("port on");
        serialOpen1();
    }
    else {
    serial->close();
    ui->labelStatus->setText("port off");
    }
}

void MainWindow::serialOpen1()
{
    serial1->setPortName("ttyUSB1");
    serial1->setBaudRate(QSerialPort::Baud9600);
    serial1->setDataBits(QSerialPort::Data8);
    serial1->setParity(QSerialPort::NoParity);
    serial1->setStopBits(QSerialPort::OneStop);
    serial1->setFlowControl(QSerialPort::NoFlowControl);
    if(serial1->open(QIODevice::ReadWrite)) ui->labelStatus->setText("Ready");
    else {
    serial->close();
    ui->labelStatus->setText("port off");
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText("temp1,temp2,humi1,humi2,flow,p03,p05,p07,p10,p2,p50");
}

void MainWindow::on_pushButton_6_clicked()
{
    int val = ui->horizontalSlider->value() + 1;
    if(val<=100) ui->fanSpeedE->setText(QString::number(val));
}

void MainWindow::on_pushButton_5_clicked()
{
    int val = ui->horizontalSlider->value() - 1;
    if(val >= 0) ui->fanSpeedE->setText(QString::number(val));
}

void MainWindow::on_pushButton_7_clicked()
{
    if(updateData->isActive()) command = "H";
    else serial->write("H");
}
