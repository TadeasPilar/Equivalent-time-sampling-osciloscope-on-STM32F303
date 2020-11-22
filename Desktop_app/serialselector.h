#ifndef SERIALSELECTOR_H
#define SERIALSELECTOR_H

#include <QObject>
#include <QDialog>
#include <QSlider>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QDebug>
#include <QListWidget>
#include <QStringList>
#include <QMessageBox>
#include <QThread>

#define STM_IDENTIFIER 1155

/*
 * Popup window for selecting and opening serial port
 */

class SerialSelector : public QDialog
{
    Q_OBJECT
public:
    SerialSelector();
    void getSerial(QSerialPort *oldPort);
    QSerialPort *port;


private:
    QVBoxLayout *layout;
    QString portName;
    QListWidget *portsList;
    QPushButton *connectButton;
    QString portErrorTranslator(int error);


private slots:
    void buttonPressed();

signals:
    void serialReady();


};

#endif // SERIALSELECTOR_H
