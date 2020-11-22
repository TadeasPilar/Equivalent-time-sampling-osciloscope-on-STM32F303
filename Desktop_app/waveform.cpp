#include "waveform.h"

//Create new waveform with set label
WaveForm::WaveForm(QString label)
{
    this->label = label;
    series = new QLineSeries();
    series->setName(label);
    series->setPointsVisible(true);
}





//Create new waveform from data
WaveForm::WaveForm(QString label, uint8_t dataArray[], qint32 length, int resolution){
    series = new QLineSeries();
    this->label = label;
    this->dataLength = length;
    this->resolution = resolution;
    update(dataArray, 1, false);




}

//Update existing waveform using data. Calculate voltages from binary data
void WaveForm::update(uint8_t dataArray[],  double eqivalentSamplingPeriod, bool displayTime){
    this->eqivalentSamplingPeriod = eqivalentSamplingPeriod;
    this->displayTime = displayTime;
    if(eqivalentSamplingPeriod>1){
        timeDivider = 1;
    }else if(eqivalentSamplingPeriod>0.001){
        timeDivider = 1000;
    }else if(eqivalentSamplingPeriod>0.000001){
        timeDivider = 1000000;
    }else if(eqivalentSamplingPeriod>0.000000001){
        timeDivider = 1000000000;
    }

    min = 10000;
    max = -10000;
    if(resolution == 8){
        if(eqivalentSamplingPeriod<0){
            for(qint32 i = 0;i<dataLength;i++){
                data[i] = (((double)dataArray[i])/256)*Vcc;
                if(data[i]<min){min = data[i];};
                if(data[i]>max){max = data[i];};
            }
        }else{
            for(qint32 i = 0;i<dataLength;i++){
                data[dataLength-i] = (((double)dataArray[i])/256)*Vcc;
                if(data[i]<min){min = data[i];};
                if(data[i]>max){max = data[i];};
            }
        }
    }

}

//For debugging
QString WaveForm::toString(){
    QString str = "";
    for(qint32 i = 0;i<dataLength;i++){
        str.append(QString::number(data[i]) + ",");
    }
    return str;
}


QLineSeries* WaveForm::getSeries(){
     series->setName(label);
    return this->series;
}

//Trigger on riding edge on level
void WaveForm::trigger(double level){

    triggerPoint = 0;
    for(int i = 20;i<dataLength;i++){
        if(data[i]>level && data[i-1]<level){
            triggerPoint = i;
            break;
        }
    }
    dataLength = dataLength - triggerPoint;

    qDebug() << "Trigger point is: " << QString::number(triggerPoint);
    qDebug() << "Triggered data length is: " << QString::number(dataLength);
    trigger(triggerPoint);

}
//Bacause abs() on Windows only supports int
double absolute(double val){
    if(val<0){
        return -val;
    }else{
        return val;
    }
}

//Strigger on triggerIndex. i.e. cut of first triggerIndex points from data.
void WaveForm::trigger(int triggerIndex){
    timeVector.clear();
    triggerPoint = triggerIndex;
    double time;
    qDebug() << "Triggering " << label << " on index " << triggerIndex;
    if(displayStyle>1){
        series = new QSplineSeries();
    }else{
        series = new QLineSeries();
    }
    series->setName(label);
    if(displayStyle==0 || displayStyle==3){
        series->setPointsVisible(true);
    }else{
        series->setPointsVisible(false);
    }
    if(displayTime){
        for(int i = 0;i<dataLength;i++){
            time = i*absolute(eqivalentSamplingPeriod)*timeDivider;
            series->append(time, data[i+triggerIndex]);
            timeVector.append(time);
        }
    }else{
        for(int i = 0;i<dataLength;i++){
            time = i;
            series->append(time, data[i+triggerIndex]);
            timeVector.append(time);
        }
    }
    emit WaveForm::triggered();

}





