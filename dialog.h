#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void readSerial();
    void updateLCD1(const QString);
    void updateLCD2(const QString );
    void updateLCD3(const QString );


    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
    QSerialPort *stm32103C8T6;
    static const quint16 stm32_bluepill_vendor_id = 4292;
    static const quint16 stm32_bluepill_product_id = 60000;
    QString stm32_port_name;
    bool stm32_disponible;

    QString parsedData;
    double ADCvalue;
    QByteArray serialData;
    QString serialBuffer;

};
#endif // DIALOG_H
