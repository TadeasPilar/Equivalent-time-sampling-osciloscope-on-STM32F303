#include "infobar.h"

InfoBar::InfoBar(NucleoScope *scope)
{
    this->scope = scope;

    //Create info bar items
    port = new InfoBarItem("Port");
    port->setValue("Disconnected");
    signalFrequency = new InfoBarItem("CH1 frequency");
    samplingFrequency = new InfoBarItem("Sampling frequency");
    Vpp = new InfoBarItem("CH1 Vpp");
    equivalentSamplingFrequency = new InfoBarItem("Equivalent sampling frequency");
    sampling = new InfoBarItem("Sampling");
    updateRate = new InfoBarItem("Update rate");

    //Add all those items to layout
    layout = new QHBoxLayout();
    layout->addWidget(port->getWidget());
    layout->addWidget(signalFrequency->getWidget());
    layout->addWidget(samplingFrequency->getWidget());
    layout->addWidget(Vpp->getWidget());
    layout->addWidget(equivalentSamplingFrequency->getWidget());
    layout->addWidget(sampling->getWidget());
    layout->addWidget(updateRate->getWidget());
    connect(scope, &NucleoScope::dataReady, this, &InfoBar::update);
    timer.start();

    //Timer to periodically(1s) check if serial port is still connected
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &InfoBar::updateSerialState);
    updateTimer->start(1000);

}


void InfoBar::updateSerialState(){  //Check if serial port is connected and update information
    updateTimer->start(1000);
    if(scope->serialPort->error()==0){
        port->setValue(scope->serialPort->portName());
    }else{
        port->setValue("Disconnected");
    }

}

void InfoBar::update(){     //Update all info bar items based on the last measurment

    signalFrequency->setValue(scope->settings->frequency, "Hz");
    double samplingFreq = scope->baseFrequency/(scope->settings->samplingDivider+1);
    samplingFrequency->setValue(samplingFreq, "Hz");
    Vpp->setValue(scope->ch1WaveForm->max-scope->ch1WaveForm->min, "V");

    //If sampling parameters are known
    if(scope->equivalentSamplingPeriod!=0){
        //If sampling is real time
        if(2*scope->settings->frequency<samplingFreq){
            equivalentSamplingFrequency->setValue(samplingFreq, "Hz");
            sampling->setValue("Real-time");

        }else{
            double eqFreq = 1/scope->equivalentSamplingPeriod;
            equivalentSamplingFrequency->setValue( eqFreq, "Hz");
            sampling->setValue("Equivalent-time");
        }
    }else{
        sampling->setValue("Unknown");
        equivalentSamplingFrequency->setValue("Unknown");
    }

    updateRate->setValue(1000/(double)timer.restart(), "Hz");


}









InfoBarItem::InfoBarItem(QString label){        //Constructor for info bar item
    box = new QGroupBox();
    layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    box->setLayout(layout);
    box->setContentsMargins(3,8,3,3);
    labelWidget = new QLabel(label);
    valueWidget = new QLabel();
    layout->addWidget(labelWidget, 1, Qt::AlignCenter);
    layout->addWidget(valueWidget, 1, Qt::AlignCenter);




}

void InfoBarItem::setValue(QString value){      //Set text value of info bar item
    valueWidget->setText(value);
}

void InfoBarItem::setValue(double value, QString unit){     //Set value from number and base unit
    QString str = "";
    if(abs(value)>1000000){
        str = "M" + unit;
        value = value/1000000;
    }else if( abs(value) > 1000){
        str = "k" + unit;
        value = value/1000;
    }else{
        str = unit;
    }
    valueWidget->setText(QString::number(value, 'f', 3) + str);
}



QWidget *InfoBarItem::getWidget(){
    return this->box;
}








