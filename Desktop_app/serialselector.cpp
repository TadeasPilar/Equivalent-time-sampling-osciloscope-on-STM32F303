#include "serialselector.h"

SerialSelector::SerialSelector()
{
    layout = new QVBoxLayout();
    this->setLayout(layout);


}



void SerialSelector::getSerial(QSerialPort *oldPort){

    if(oldPort != nullptr){
        if(oldPort->isOpen()){ //If serial port already opened, close it before opening new one.
            oldPort->close();
        }
    }

    connectButton = new QPushButton("Connect");
    layout->addWidget(connectButton);
    connect(connectButton, &QPushButton::clicked, this, &SerialSelector::buttonPressed);

    portsList = new QListWidget(this);
    portsList->setMinimumSize(300, 300);
    layout->addWidget(portsList);


    //Print debug info about all serial ports
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << " Manufacturer: " << serialPortInfo.manufacturer() << "\n";
        qDebug() << " Port name: " << serialPortInfo.portName() << "\n";
        qDebug() << " Product identifier: " << serialPortInfo.productIdentifier() << "\n";
        qDebug() << " Serial number: " << serialPortInfo.serialNumber() << "\n";
        qDebug() << " Vendor identifier: " << serialPortInfo.vendorIdentifier() << "\n";

        //Create list of available serial ports
        QString text;
        text.append(serialPortInfo.manufacturer());
        text.append(" at " + serialPortInfo.portName());
        new QListWidgetItem(text, portsList);
    }


    exec(); //Show window. Prevent main window usage until this is closed.

}

//Tries to open selected serial port. Creates error message if it failes.
void SerialSelector::buttonPressed(){
    if(portsList->selectedItems().size()>0){
        QStringList portInfo = portsList->currentItem()->text().split(" ");
        QString portName = portInfo[2];
        qDebug() << portName << "\n";
        port = new QSerialPort(portName);
        port->setBaudRate(QSerialPort::Baud115200);
        port->setDataBits(QSerialPort::Data8);
        port->setFlowControl(QSerialPort::NoFlowControl);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        port->open(QSerialPort::ReadWrite);
        if(port->error() == QSerialPort::NoError){
            connectButton->setText("Connected");
            QColor col = QColor(Qt::green);
            QString qss = QString("background-color: %1").arg(col.name());
            connectButton->setStyleSheet(qss);

            QPushButton *close = new QPushButton("Close");
            layout->addWidget(close);
            connect(close, &QPushButton::clicked, this, &QWidget::close);

            emit SerialSelector::serialReady();
        } else {
            QString errorMsg;
            errorMsg.append("Failed to open selected port\n");
            errorMsg.append("Error: " + portErrorTranslator(port->error()));
            QMessageBox::critical(this, "Error", errorMsg);
        }
    }

}

//Translets serial port error from int to human readable string.
QString SerialSelector::portErrorTranslator(int error){
    switch(error){
    case 0:
        return "NoError";
    case 1:
        return "DeviceNotFoundError";
    case 2:
        return "PermissionError";
    case 3:
        return "OpenError";
    case 4:
        return "ParityError";
    case 5:
        return "FramingError";
    case 6:
        return "BreakConditionError";
    case 7:
        return "WriteError";
    case 8:
        return "ReadError";
    case 9:
        return "ResourceError";
    case 10:
        return "UnsupportedOperationError";
    case 11:
        return "UnknownError";
    case 12:
        return "TimeoutError";
    case 13:
        return "NotOpenError";
    }
    return "Invalid error identifier";
}

