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
    /*******************************************************************************************
    *
    *Prototipo  de funciones encargadas de mostrar en display el valor del ADC[]
    *
    *******************************************************************************************/
    void updateLCD1(const QString);
    void updateLCD2(const QString );
    void updateLCD3(const QString );

    /*******************************************************************************************
    *
    *Prototipo de funciones encargadas de seleccionar el display del ADC que se desea leer datos
    *
    *******************************************************************************************/
    void on_pushButton_ADC0_clicked();
    void on_pushButton_ADC1_clicked();
    void on_pushButton_ADC2_clicked();


    /*******************************************************************************************
    *
    *Prototipo de funciones encargadas de enviar 1/0 logico a cada salida
    *
    *   on_pushButton_OALL_clicked() = encargada de enviar 1/0 a todas las senales
    *
    *******************************************************************************************/
    void on_pushButton_O1_clicked();
    void on_pushButton_O2_clicked();
    void on_pushButton_O3_clicked();
    void on_pushButton_O4_clicked();
    void on_pushButton_O5_clicked();
    void on_pushButton_O6_clicked();
    void on_pushButton_O7_clicked();
    void on_pushButton_O8_clicked();
    void on_pushButton_OutputALL_clicked();
    /*******************************************************************************************
    *
    *Prototipo  de funciones encargadas de mostrar en display el estado logico de las ENTRADAS
    *
    *******************************************************************************************/
    void updateInput01(const QString);
    void updateInput02(const QString);
    void updateInput03(const QString);
    void updateInput04(const QString);
    void updateInput05(const QString);
    void updateInput06(const QString);
    void updateInput07(const QString);
    void updateInput08(const QString);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

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
