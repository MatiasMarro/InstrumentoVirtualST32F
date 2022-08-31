#include "dialog.h"
#include "ui_dialog.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QWidget>
#include <QString>
#include <QMessageBox>

QString adcTOread= "ADC0|ADC1|ADC2";

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    //COMIENZO DE FUNCION PRINCIPAL

    //Seteamos los parametros por defecto al comienzo
    ui->setupUi(this);
    ui->ADC0->display("--------");
    ui->ADC1->display("--------");
    ui->ADC2->display("--------");
    ui->lcdINPUT1->display("-");
    ui->lcdINPUT2->display("-");
    ui->lcdINPUT3->display("-");
    ui->lcdINPUT4->display("-");
    ui->lcdINPUT5->display("-");
    ui->lcdINPUT6->display("-");
    ui->lcdINPUT7->display("-");
    ui->lcdINPUT8->display("-");
    stm32_port_name = "";
    stm32_disponible = false;
    serialBuffer = "";

    //Creamos el objeto stm32 que representa nuestra bluepill
    stm32103C8T6 = new  QSerialPort;
    stm32103C8T6 = new  QSerialPort;


    /* COMPROBACION DE PUERTOS HABILITADOS Y HABILITACION DE PUERTO EN CASO DE EXISTIR BLUEPILL */
    /* Tomamos los ID de la bluepill para despues guardarlos en constantes (ver dialog.h -> private:)
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

     //Miramos los puertos habilitados para encontrar nuestra bluepill con los datos ID que guardamos en variables privadas (ver dialog.h->private:)
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

     //Si se encuentra en los puertos nuestra blue pill se configura y se llama a la funcion readSerial cada vez que exista una senal en el puerto
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

//Destructor del objeto, comprueba si esta abierto el puerto de la bluepill y lo cierra
Dialog::~Dialog()
{
    if(stm32103C8T6->isOpen())
        {
            stm32103C8T6->close();
        }
        delete ui;
}

//Funcion principal para lectura de datos por el puerto
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

                qDebug()<<bufferList[i] << "lista";

             //COMPROBACION PARA DATOS DE ADC[]
                  if(bufferList[i].split(";").length() > 1)
                  {
                    if(bufferList[i].split(";")[0] != ""){
                    //    qDebug()<<"llamar display valor adc2 "+bufferList[i].split(";")[0];
                        updateLCD3(bufferList[i].split(";")[0]);
                    }else{
                   //   qDebug()<<"imprimir cero en lcdadc2";
                      updateLCD3("0000");
                    }
                  }else if(bufferList[i].split("-").length() > 1)
                  {
                    if(bufferList[i].split("-")[0] != ""){
                   //   qDebug()<<"llamar display valor adc1 "+bufferList[i].split("-")[0];
                      updateLCD2(bufferList[i].split("-")[0]);
                    }else{
                    //  qDebug()<<"imprimir cero en lcdadc1";
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

              if(bufferList[i] == "OFF1")
                  updateInput01("0");
              if(bufferList[i] == "ON1")
                  updateInput01("1");

              if(bufferList[i] == "OFF2")
                  updateInput02("0");
              if(bufferList[i] == "ON2")
                  updateInput02("1");

              if(bufferList[i] == "OFF3")
                  updateInput03("0");
              if(bufferList[i] == "ON3")
                  updateInput03("1");

              if(bufferList[i] == "OFF4")
                  updateInput04("0");
              if(bufferList[i] == "ON4")
                  updateInput04("1");

              if(bufferList[i] == "OFF5")
                  updateInput05("0");
              if(bufferList[i] == "ON5")
                  updateInput05("1");

              if(bufferList[i] == "OFF6")
                  updateInput06("0");
              if(bufferList[i] == "ON6")
                  updateInput06("1");

              if(bufferList[i] == "OFF7")
                  updateInput07("0");
              if(bufferList[i] == "ON7")
                  updateInput07("1");

              if(bufferList[i] == "OFF8")
                  updateInput08("0");
              if(bufferList[i] == "ON8")
                  updateInput08("1");


              //COMPROBACION PARA ESTADO DE LAS ENTRADAS
//              if(bufferList[i].split("OFF").length() > 1 )
//              {
//                  if(bufferList[i].split("OFF")[1] == "1")
//                      updateInput01("0");
//                  if(bufferList[i].split("OFF")[1] == "2")
//                      updateInput02("0");
//                  if(bufferList[i].split("OFF")[1] == "3")
//                      updateInput03("0");
//                  if(bufferList[i].split("OFF")[1] == "4")
//                      updateInput04("0");
//                  if(bufferList[i].split("OFF")[1] == "5")
//                      updateInput05("0");
//                  if(bufferList[i].split("OFF")[1] == "6")
//                      updateInput06("0");
//                  if(bufferList[i].split("OFF")[1] == "7")
//                      updateInput07("0");
//                  if(bufferList[i].split("OFF")[1] == "8")
//                      updateInput08("0");
//              }
//          qDebug()<<"El pin "<<bufferList[i].split("OFF")[1]<<" esta desconectado";

//              if(bufferList[i].split("ON").length() > 1)
//              {
//                  if(bufferList[i].split("ON")[1] == "1")
//                       updateInput01("1");
//                  if(bufferList[i].split("ON")[1] == "2")
//                       updateInput02("1");
//                  if(bufferList[i].split("ON")[1] == "3")
//                       updateInput03("1");
//                  if(bufferList[i].split("ON")[1] == "4")
//                       updateInput04("1");
//                  if(bufferList[i].split("ON")[1] == "5")
//                       updateInput05("1");
//                  if(bufferList[i].split("ON")[1] == "6")
//                       updateInput06("1");
//                  if(bufferList[i].split("ON")[1] == "7")
//                       updateInput07("1");
//                  if(bufferList[i].split("ON")[1] == "8")
//                       updateInput08("1");

//             }
//          qDebug()<<"El pin "<<bufferList[i].split("ON")[1]<<" esta CONECTADO";



            }
            serialBuffer.clear();

        }

}
/*******************************************************************************************
*
*Funciones encargadas de enviar los datos del ADC al display
*
*******************************************************************************************/
void Dialog::updateLCD1(const QString adc_reading)
{

    ui->ADC0->display(adc_reading);
}

void Dialog::updateLCD2(const QString adc_reading)
{
    ui->ADC1->display(adc_reading);
}

void Dialog::updateLCD3(const QString adc_reading)
{
    ui->ADC2->display(adc_reading);
}

/*******************************************************************************************
*
*Funciones encargadas de mostrar la lectura del ADC clickeado
*
*******************************************************************************************/
void Dialog::on_pushButton_ADC0_clicked()
{
     qDebug()<<"ADC0";
}


void Dialog::on_pushButton_ADC1_clicked()
{
    qDebug()<<"ADC1";
}


void Dialog::on_pushButton_ADC2_clicked()
{
    qDebug()<<"ADC2";
}

/*******************************************************************************************
*
*Funciones encargadas de enviar 1/0 logico cuando se da click
*
*
*******************************************************************************************/
void Dialog::on_pushButton_O1_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("A");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"PIN 1";
}


void Dialog::on_pushButton_O2_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("B");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"PIN 2";
}


void Dialog::on_pushButton_O3_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("C");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"PIN 3";
}


void Dialog::on_pushButton_O4_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("D");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"PIN 4";
}


void Dialog::on_pushButton_O5_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("E");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"PIN 5";
}


void Dialog::on_pushButton_O6_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("F");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"PIN 6";
}


void Dialog::on_pushButton_O7_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("G");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"PIN 7";
}


void Dialog::on_pushButton_O8_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("H");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"PIN 8";
}


void Dialog::on_pushButton_OutputALL_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("ABCDEFGH");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"ALL PIN";
}


/*******************************************************************************************
*
*Funciones encargadas de actualizar el display con la salida logica correspondiente
*
*
*******************************************************************************************/
void Dialog::updateInput01(const QString logicState)
{
    ui->lcdINPUT1->display(logicState);
}
void Dialog::updateInput02(const QString logicState)
{
    ui->lcdINPUT2->display(logicState);
}
void Dialog::updateInput03(const QString logicState)
{
    ui->lcdINPUT3->display(logicState);
}
void Dialog::updateInput04(const QString logicState)
{
    ui->lcdINPUT4->display(logicState);
}
void Dialog::updateInput05(const QString logicState)
{
    ui->lcdINPUT5->display(logicState);
}
void Dialog::updateInput06(const QString logicState)
{
    ui->lcdINPUT6->display(logicState);
}
void Dialog::updateInput07(const QString logicState)
{
    ui->lcdINPUT7->display(logicState);
}
void Dialog::updateInput08(const QString logicState)
{
    ui->lcdINPUT8->display(logicState);
}


/*******************************************************************************************
*
*
*Funciones encargadas de enviar peticion de estado de las entradas
*
*
*******************************************************************************************/

void Dialog::on_pushButton_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("a");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"COMPROBAR PIN 1";
}


void Dialog::on_pushButton_2_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("b");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"COMPROBAR PIN 2";
}


void Dialog::on_pushButton_3_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("c");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"COMPROBAR PIN 3";
}


void Dialog::on_pushButton_4_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("d");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"COMPROBAR PIN 4";
}


void Dialog::on_pushButton_5_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("e");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"COMPROBAR PIN 5";
}


void Dialog::on_pushButton_6_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("f");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"COMPROBAR PIN 6";
}


void Dialog::on_pushButton_7_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("g");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"COMPROBAR PIN 7";
}


void Dialog::on_pushButton_8_clicked()
{
    if(stm32103C8T6->isWritable())
      {
          stm32103C8T6->write("h");
      }else{
           QMessageBox::warning(this,"NO SE PUEDE ENVIAR DATOS", "Compruebe el estado de su puerto");
      }
      qDebug()<<"COMPROBAR PIN 8";
}

