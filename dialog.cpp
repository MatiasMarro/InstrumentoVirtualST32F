#include "dialog.h"
#include "ui_dialog.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QWidget>
#include <QString>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //ui->ADC0->display("--------");
    ui->ADC1->display("--------");
    ui->ADC2->display("--------");


    stm32103C8T6 = new  QSerialPort;
    stm32103C8T6 = new  QSerialPort;
    stm32_port_name = "";
    stm32_disponible = false;
    serialBuffer = "";

    /*
    qDebug()<< "Numero de puertos disponibles:"<<QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            qDebug()<<"Tiene vendorID: "<< serialPortInfo.hasVendorIdentifier();
            if(serialPortInfo.hasVendorIdentifier())
            {
                qDebug()<< "Vendor ID: "<< serialPortInfo.vendorIdentifier();
            }

            qDebug()<<"Tiene productID: "<< serialPortInfo.hasProductIdentifier();
            if(serialPortInfo.hasProductIdentifier())
            {
                qDebug()<< "pruduct ID: "<< serialPortInfo.productIdentifier();
            }
        }
     qDebug()<< "===========================================================================";

     */
     foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
         {
             if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier())
             {
                 if(serialPortInfo.productIdentifier() == stm32_bluepill_product_id && serialPortInfo.vendorIdentifier() == stm32_bluepill_vendor_id )
                 {
                     qDebug()<< "Se encontro el puerto del stm32";
                     stm32_port_name = serialPortInfo.portName();
                     stm32_disponible = true;
                 }
             }
         }

     if(stm32_disponible)
         {
             stm32103C8T6->setPortName(stm32_port_name);
             stm32103C8T6->open(QSerialPort::ReadWrite);
             stm32103C8T6->setBaudRate(QSerialPort::Baud115200);
             stm32103C8T6->setDataBits(QSerialPort::Data8);
             stm32103C8T6->setFlowControl(QSerialPort::NoFlowControl);
             stm32103C8T6->setParity(QSerialPort::NoParity);
             stm32103C8T6->setStopBits(QSerialPort::OneStop);
             QObject::connect(stm32103C8T6, SIGNAL(readyRead()), this, SLOT(readSerial()));

         }else
             {
                 QMessageBox::warning(this,"ERROR DEL PUERTO", "Asegurese de conectar la bluepill con su UART");
             }

}

Dialog::~Dialog()
{
    if(stm32103C8T6->isOpen())
        {
            stm32103C8T6->close();
        }
        delete ui;
}

void Dialog::readSerial()
{
    // LECTURA DEL PUERTO
        QStringList bufferList = serialBuffer.split("|");
        if(bufferList.length() < 3)
        {
           serialData = stm32103C8T6->readAll();
          // QObject().thread()->
           serialBuffer += QString::fromStdString(serialData.toStdString());


        }else{
            qDebug() <<bufferList;
            for (int i=0; i < bufferList.length() ; i++){
              if(bufferList[i] != "" || bufferList[i] !=";" || bufferList[i] !="-" || bufferList[i] != "|" ){

                qDebug()<<bufferList[i];

                  if(bufferList[i].split(";").length() > 1)
                  {
                    if(bufferList[i].split(";")[0] != ""){
                      //  qDebug()<<"llamar display valor adc2 "+bufferList[i].split(";")[0];
                        updateLCD3(bufferList[i].split(";")[0]);
                    }else{
                    //  qDebug()<<"imprimir cero en lcdadc2";
                      updateLCD3("0000");
                    }
                  }else if(bufferList[i].split("-").length() > 1)
                  {
                    if(bufferList[i].split("-")[0] != ""){
                    //  qDebug()<<"llamar display valor adc1 "+bufferList[i].split("-")[0];
                      updateLCD2(bufferList[i].split("-")[0]);
                    }else{
                     // qDebug()<<"imprimir cero en lcdadc1";
                      updateLCD2("0000");
                    }
                  }else{
                    if(bufferList[i] != ""){
                      //  qDebug()<<"llamar display valor adc0 "+ bufferList[i];
                        updateLCD1(bufferList[i]);
                    }else{
                     // qDebug()<<"imprimir cero en lcdadc0";
                      updateLCD1("0000");
                    }
                  }
              }
            }
            serialBuffer.clear();

        }

}

void Dialog::updateLCD1(const QString adc_reading)
{
    qDebug()<<adc_reading;
    //ui->ADC0->display(adc_reading);
}

void Dialog::updateLCD2(const QString adc_reading)
{
    ui->ADC1->display(adc_reading);
}

void Dialog::updateLCD3(const QString adc_reading)
{
    ui->ADC2->display(adc_reading);
}


void Dialog::on_pushButton_2_clicked()
{
    if(stm32103C8T6->isWritable())
    {
        stm32103C8T6->write("FA");
    }else{
         QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
    }
    qDebug()<<"A";
}

