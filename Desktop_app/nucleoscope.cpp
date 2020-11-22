#include "nucleoscope.h"

NucleoScope::NucleoScope(QObject *mainWindow)
{
    this->pearent = mainWindow;
    ch1WaveForm = new WaveForm("CH1");
    ch2WaveForm = new WaveForm("CH2");
    ch3WaveForm = new WaveForm("CH3");
    ch4WaveForm = new WaveForm("CH4");
    serialPort = new QSerialPort();
    settings = new ScopeSettings();
    appSettings = new ScopeSettings();
    autosetInfo = new QProgressDialog();
    autosetInfo->close();






}

void NucleoScope::autosetProcedure(){ //If autoset is in progress, this function gets called instead of displaying the data.

    switch(autosetState){
    case 1:                 //Step one: Measure 12 000 points at 1.532 MHz
        oldMemDepth = settings->memDepth;
        oldSamplingDivider = settings->samplingDivider;
        autosetInfo = new QProgressDialog();    //Tell user about the autoset progress using progress bar.
        autosetInfo->setRange(0, 100);
        autosetInfo->setValue(10);
        autosetInfo->setLabelText("Autoset in progress");
        connect(autosetInfo, &QProgressDialog::canceled, this, &NucleoScope::cancelAutoset);
        autosetInfo->open();
        appSettings->samplingDivider = 47-1;
        appSettings->memDepth = 12000;
        autosetState = 2;
        break;
    case 2:             //Step two: Measure 12 000 points at 72.216 kHz. Find min and max values.
        autosetMin = ch1WaveForm->min;
        autosetMax = ch1WaveForm->max;
        appSettings->samplingDivider = 997-1;
        autosetState = 3;
        autosetInfo->setValue(30);
        break;
    case 3:            //Step three: Measure 12 000 points at 14.49 kHz. Find min and max values.
        if(ch1WaveForm->min<autosetMin){
            autosetMin = ch1WaveForm->min;
        }
        if(ch1WaveForm->max>autosetMax){
            autosetMax = ch1WaveForm->max;
        }
        appSettings->samplingDivider = 4969-1;
        autosetInfo->setValue(60);
        autosetState = 4;
        break;
    case 4:{            //Step four: Find min and max values. Set comparator treshod voltage to average of min and max.
        if(ch1WaveForm->min<autosetMin){
            autosetMin = ch1WaveForm->min;
        }
        if(ch1WaveForm->max>autosetMax){
            autosetMax = ch1WaveForm->max;
        }
        autosetInfo->setValue(80);
        double midVoltage = (autosetMax+autosetMin)/2;
        appSettings->compVoltage = (int)(midVoltage/0.000805664);}
    case 5:
        appSettings->memDepth = 500;
        autosetState = 0;
        emit autosetMeasureDone();  //Autoset procedure will continue in ScopeDisplay

        break;

    }


    writeSettings();    //Apply settings and start new measurment

}

void NucleoScope::cancelAutoset(){  //If user decides to cancel autoset procedure, set osciloscope to its previous state.
    if(autosetState!=0){
        autosetState = 0;
        appSettings->memDepth = oldMemDepth;
        appSettings->samplingDivider = oldSamplingDivider;
    }
    autosetInfo->close();
}



void NucleoScope::ConnectToBoard(QSerialPort *port){  //Connect serial port data ready to data read function.
    serialPort = port;
    connected = true;
    connect(port, &QSerialPort::readyRead, this, &NucleoScope::readSerial);
    writeSettings();    //Start first measurment
}


void NucleoScope::readSerial(){

    while(serialPort->bytesAvailable()>0){      //Read all data currently available in serial buffer
        QByteArray arr = serialPort->read(1);   //Read one byte from buffer
        char ch = (char)arr[0];
        switch(parsingStage){
        case 0:                                 //Stage 0: Ignore data until begining of message is found
            if(ch == ':'){
                timer.start();
                parsingStage = 1;
                settingsString = "";
            }
            break;
        case 1:                                 //Stage 1: Coppy data into settings string until end of setting string is found. Then parse settings.
            if(ch == ':'){
                parsingStage = 2;
                qDebug() << "Settings string is: " << settingsString << "\n";
                settings->parseSettings(settingsString);
                if(settings->bits == 8){        //Calculate data size based on memory depth and ADC bits parsed from setting string
                    dataSize = settings->memDepth*settings->chanelCount();
                }else{
                    dataSize = settings->memDepth*settings->chanelCount()*2;
                }
                dataPointer = 0;
                for(int i =0;i<48000;i++){
                    dataArray[i] = 0;
                }


                qDebug() << "Data size is: " << QString::number(dataSize) << "\n";
                qDebug().noquote() << settings->toHumanString() + "\n";
            } else {
                settingsString.append(ch);
            }
            break;
        case 2:                                //Stage 2: Coppy data size of data into data array. Check if there is terminator hracter(!) at the end of data.
            if(dataPointer<dataSize){
                dataArray[dataPointer] = ch;
                dataPointer++;
            }else{
                if(ch == '!'){
                    parsingStage = 0;
                    qDebug() << "Read " << QString::number(dataPointer) << " bytes of data.";
                    //stopped = false;
                    qDebug() << "Time spent recieving: " << timer.elapsed() << "ms";
                    createWaveForms();
                }else{
                    qDebug() << ch << " is not terminator character\n";
                }
            }
            break;
        }
    }
}


void NucleoScope::createWaveForms(){        //Parse data from data array into its proper waveforms.
    int chc = settings->chanelCount();
    if(chc>0){
        qDebug() << "Equivalent sampling period is: " + QString::number(equivalentSamplingPeriod);
        timer.start();
        int counter = 0;
        int size = dataSize/chc;

        if(settings->channel1On){
            uint8_t data[size];
            int p = 0;
            for(int j = counter*size;j<counter*size+size;j++){
                data[p] = (uint8_t)dataArray[j];
                p++;
            }
            counter++;
            ch1WaveForm->dataLength = settings->memDepth;
            ch1WaveForm->resolution = settings->bits;
            ch1WaveForm->frequency = settings->frequency;
            ch1WaveForm->update(data, equivalentSamplingPeriod, displayTime);

        }

        if(settings->channel2On){
            uint8_t data[size];
            int p = 0;
            for(int j = counter*size;j<counter*size+size;j++){
                data[p] = (uint8_t)dataArray[j];
                p++;
            }
            counter++;
            ch2WaveForm->dataLength = settings->memDepth;
            ch2WaveForm->resolution = settings->bits;

            ch2WaveForm->update(data, equivalentSamplingPeriod, displayTime);
        }

        if(settings->channel3On){
            uint8_t data[size];
            int p = 0;
            for(int j = counter*size;j<counter*size+size;j++){
                data[p] = (uint8_t)dataArray[j];
                p++;
            }
            counter++;
            ch3WaveForm->dataLength = settings->memDepth;
            ch3WaveForm->resolution = settings->bits;
            ch3WaveForm->update(data, equivalentSamplingPeriod, displayTime);
        }

        if(settings->channel4On){
            uint8_t data[size];
            int p = 0;
            for(int j = counter*size;j<counter*size+size;j++){
                data[p] = (uint8_t)dataArray[j];
                p++;
            }
            counter++;
            ch4WaveForm->dataLength = settings->memDepth;
            ch4WaveForm->resolution = settings->bits;
            ch4WaveForm->update(data, equivalentSamplingPeriod, displayTime);
        }

        qDebug() << "Time spent creating waveforms: " << timer.elapsed() << "ms";
        if(autosetState == 0){  //If autoset not in progress, draw waveforms. Else continue autoset.
            emit NucleoScope::dataReady();
        }else{
            autosetProcedure();
        }
    }
}





void NucleoScope::writeSettings(){  //Write settings set by user to microcontroller.
    if(connected){
        QString message = ':' + appSettings->toString() + ':' + "##############################"; //There must be always at least 50 characters in message to microcontroller. Padded by #.
        qDebug() << "Writing app settings" + message;
        serialPort->write(message.toUtf8());
    }

}










ScopeSettings::ScopeSettings(QString settingsString){
    this->parseSettings(settingsString);

}

ScopeSettings::ScopeSettings(){

}



bool ScopeSettings::parseSettings(QString settings){
    QStringList settingsList = settings.split('|');
    if(settingsList.length()>12){
        samplingDivider = settingsList[0].toInt();
        generatorDivider = settingsList[1].toInt();
        compVoltage = settingsList[2].toInt();
        compHysteresis = settingsList[3].toInt();
        channel1On = settingsList[4].toInt();
        channel2On = settingsList[5].toInt();
        channel3On = settingsList[6].toInt();
        channel4On = settingsList[7].toInt();
        memDepth = settingsList[8].toInt();
        bits = settingsList[9].toInt();
        channel4Vref = settingsList[10].toInt();
        frequency = settingsList[11].toInt();
        vrefCal = settingsList[12].toInt();

    }else{
        qWarning() << "Failed to parse settings\n";
        return false;
    }
    //TODO: Check if parsed numbers make sense.
    return true;
}

QString ScopeSettings::toHumanString(){
    return "Sampling divider: " + QString::number(samplingDivider) + "\nGenerator divider: " + QString::number(generatorDivider) +
            "\nComparator voltage: " + QString::number(compVoltage) + "\nComparator hysteresis: " + QString::number(compHysteresis) +
            "\nChanneles: " + QString::number(channel1On) + '|' + QString::number(channel2On) + '|' + QString::number(channel3On) +
            '|' + QString::number(channel4On) + "\nMemory depth: " + QString::number(memDepth) + "\nBit depth: " + QString::number(bits) +
            "\nVRef on channel 4: " + QString::number(channel4Vref) + "\nSignal frequency: " + QString::number(frequency) +
            "\nVRefCal: " + QString::number(vrefCal);
}


QString ScopeSettings::toString(){
    return  QString::number(samplingDivider) + "|" + QString::number(generatorDivider) +
            "|" + QString::number(compVoltage) + "|" + QString::number(compHysteresis) +
            "|" + QString::number(channel1On) + '|' + QString::number(channel2On) + '|' + QString::number(channel3On) +
            '|' + QString::number(channel4On) + "|" + QString::number(memDepth) + "|" + QString::number(bits) +
            "|" + QString::number(channel4Vref);
}


int ScopeSettings::chanelCount(){
    int n = 0;
    if(channel1On){n++;};
    if(channel2On){n++;};
    if(channel3On){n++;};
    if(channel4On){n++;};
    return n;
}















