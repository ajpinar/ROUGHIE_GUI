#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;

    doLinPID = false;

    /*
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }
    */

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_mega_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_mega_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    if(arduino_is_available){
        // open and configure the port
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
    }else{
        // give error message if not available
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
    }
    connect(arduino, SIGNAL(readyRead()), this, SLOT(readData()));
}

Dialog::~Dialog()
{
    if(arduino->isOpen()){
        arduino->close();
    }
    delete ui;
}

void Dialog::plot_pressure_data(){
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->plot->xAxis->setRange(-1, 1);
    ui->plot->yAxis->setRange(0, 1);
    ui->plot->replot();
}

void Dialog::on_ROTARY_HIGH_LIMIT_BUTTON_clicked()
{
    QString message = "update -rothighlimit " + ui->ROTARY_HIGH_LIMIT_VALUE->text();
    Dialog::sendToArduino(message);

}

void Dialog::on_ROTARY_LOW_LIMIT_BUTTON_clicked()
{
    QString message = "update -rotlowlimit " + ui->ROTARY_LOW_LIMIT_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_LINEAR_BACK_LIMIT_BUTTON_clicked()
{
    QString message = "update -linbacklimit " + ui->LINEAR_BACK_LIMIT_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_LINEAR_FRONT_LIMIT_BUTTON_clicked()
{
    QString message = "update -linfrontlimit " + ui->LINEAR_FRONT_LIMIT_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_ALLOWED_WORK_TIME_BUTTON_clicked()
{
    QString message = "update -allowedWorkTime " + ui->ALLOWED_WORK_TIME_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_RISE_TIME_BUTTON_clicked()
{
    QString message = "update -risetime " + ui->RISE_TIME_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_DESCENT_TIME_BUTTON_clicked()
{
    QString message = "update -destime " + ui->DESCENT_TIME_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_NUMBER_OF_GLIDES_BUTTON_clicked()
{
    QString message = "update -number_of_glides " + ui->NUMBER_OF_GLIDES_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_GLIDE_BOTTOM_DEPTH_BUTTON_clicked()
{
    QString message = "update -glidebottom " + ui->GLIDE_BOTTOM_DEPTH_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_GLIDE_TOP_DEPTH_BUTTON_clicked()
{
    QString message = "update -glidetop " + ui->GLIDE_TOP_DEPTH_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_LINEAR_RATE_BUTTON_clicked()
{
    QString message = "update -linrate " + ui->LINEAR_RATE_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_ROTATION_RATE_BUTTON_clicked()
{
    QString message = "update -rotrate " + ui->ROTATION_RATE_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_TANK_MID_BUTTON_clicked()
{
    QString message = "update -tankmid " + ui->TANK_MID_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_LINEAR_MID_BUTTON_clicked()
{
    QString message = "update -linmid " + ui->LINEAR_MID_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_ROTATION_MID_BUTTON_clicked()
{
    QString message = "update -rotmid " + ui->ROTATION_MID_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_LINEAR_PID_KP_BUTTON_clicked()
{
    QString message = "update -linkp " + ui->LINEAR_PID_KP_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_LINEAR_PID_KI_BUTTON_clicked()
{
    QString message = "update -linki " + ui->LINEAR_PID_KI_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_LINEAR_PID_KD_BUTTON_clicked()
{
    QString message = "update -linkd " + ui->LINEAR_PID_KD_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_NOSE_UP_TARGET_ANGLE_BUTTON_clicked()
{
    QString message = "update -linNoseUpTarget " + ui->NOSE_UP_TARGET_ANGLE_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_NOSE_DOWN_TARGET_ANGLE_BUTTON_clicked()
{
    QString message = "update -linNoseDownTarget " + ui->NOSE_DOWN_TARGET_ANGLE_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_START_GLIDE_CYCLE_BUTTON_clicked()
{
    QString message = "start";
    Dialog::sendToArduino(message);
}

void Dialog::on_STOP_GLIDE_CYCLE_BUTTON_clicked()
{
    QString message = "stop";
    Dialog::sendToArduino(message);
}

void Dialog::on_RESET_BUTTON_clicked()
{
    QString message = "reset";
    Dialog::sendToArduino(message);
}

void Dialog::on_CURRENT_PARAMETERS_BUTTON_clicked()
{

    QString message = "params";
    Dialog::sendToArduino(message);

}

void Dialog::on_CURRENT_POSITIONS_BUTTON_clicked()
{
    QString message = "currentpos";
    Dialog::sendToArduino(message);
}

void Dialog::on_DATALOGGER_START_BUTTON_clicked()
{
    QString message = "sdstart";
    Dialog::sendToArduino(message);
}

void Dialog::on_DATALOGGER_STOP_BUTTON_clicked()
{
    QString message = "sdstop";
    Dialog::sendToArduino(message);
}

void Dialog::on_PRESSURE_CONTROL_CHECK_clicked()
{
    QString message = "pressurecontrol";
    Dialog::sendToArduino(message);
}

void Dialog::on_BANG_BANG_RADIO_clicked()
{
    if(doLinPID){
        QString message = "linear";
        Dialog::sendToArduino(message);
    }
    doLinPID = false;
}

void Dialog::on_PID_RADIO_clicked()
{
    if(!doLinPID){
        QString message = "linear";
        Dialog::sendToArduino(message);
    }
    doLinPID = true;
}

void Dialog::sendToArduino(QString command)
{
    if(arduino->isWritable()){
        arduino->write(command.toStdString().c_str());
        qDebug() << command.toStdString().c_str();
    }else{
        qDebug() << "Couldn't write to serial!";
    }
}

void Dialog::readData()
{
    data.append(arduino->readAll());
    ui->serialmonitor->setPlainText(data);
    //qDebug() << data;
}

void Dialog::on_ROTARY_HIGH_LIMIT_VALUE_returnPressed()
{
    QString message = "update -rothighlimit " + ui->ROTARY_HIGH_LIMIT_VALUE->text();
    Dialog::sendToArduino(message);
}

void Dialog::on_terminalSend_clicked()
{
    Dialog::terminalSend();
}

void Dialog::on_terminal_returnPressed()
{
    Dialog::terminalSend();
}

void Dialog::terminalSend()// scroll not working
{
    Dialog::sendToArduino(ui->terminal->text());
    ui->terminal->setText("");
    QScrollBar *vScrollBar = ui->serialmonitor->verticalScrollBar();
    vScrollBar->triggerAction(QScrollBar::SliderToMaximum);
}

void Dialog::on_PLOT_PRESSURE_DATA_clicked()
{
    Dialog::plot_pressure_data();
}
