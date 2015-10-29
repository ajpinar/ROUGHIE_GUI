#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_ROTARY_HIGH_LIMIT_BUTTON_clicked();

    void on_ROTARY_LOW_LIMIT_BUTTON_clicked();

    void on_LINEAR_BACK_LIMIT_BUTTON_clicked();

    void on_LINEAR_FRONT_LIMIT_BUTTON_clicked();

    void on_ALLOWED_WORK_TIME_BUTTON_clicked();

    void on_RISE_TIME_BUTTON_clicked();

    void on_DESCENT_TIME_BUTTON_clicked();

    void on_NUMBER_OF_GLIDES_BUTTON_clicked();

    void on_GLIDE_BOTTOM_DEPTH_BUTTON_clicked();

    void on_GLIDE_TOP_DEPTH_BUTTON_clicked();

    void on_LINEAR_RATE_BUTTON_clicked();

    void on_ROTATION_RATE_BUTTON_clicked();

    void on_TANK_MID_BUTTON_clicked();

    void on_LINEAR_MID_BUTTON_clicked();

    void on_ROTATION_MID_BUTTON_clicked();

    void on_LINEAR_PID_KP_BUTTON_clicked();

    void on_LINEAR_PID_KI_BUTTON_clicked();

    void on_LINEAR_PID_KD_BUTTON_clicked();

    void on_NOSE_UP_TARGET_ANGLE_BUTTON_clicked();

    void on_NOSE_DOWN_TARGET_ANGLE_BUTTON_clicked();

    void on_START_GLIDE_CYCLE_BUTTON_clicked();

    void on_STOP_GLIDE_CYCLE_BUTTON_clicked();

    void on_RESET_BUTTON_clicked();

    void on_CURRENT_PARAMETERS_BUTTON_clicked();

    void on_CURRENT_POSITIONS_BUTTON_clicked();

    void on_DATALOGGER_START_BUTTON_clicked();

    void on_DATALOGGER_STOP_BUTTON_clicked();

    void on_PRESSURE_CONTROL_CHECK_clicked();

    void on_BANG_BANG_RADIO_clicked();

    void on_PID_RADIO_clicked();

    void sendToArduino(QString);

    void on_ROTARY_HIGH_LIMIT_VALUE_returnPressed();

    void readData();

    void on_terminalSend_clicked();

    void on_terminal_returnPressed();

    void terminalSend();

    void plot_pressure_data();

    void on_PLOT_PRESSURE_DATA_clicked();

private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    static const quint16 arduino_mega_vendor_id = 9025;
    static const quint16 arduino_mega_product_id = 66;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
    bool doLinPID;
};

#endif // DIALOG_H
