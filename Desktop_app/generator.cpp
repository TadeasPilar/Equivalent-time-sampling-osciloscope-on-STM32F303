#include "generator.h"



Generator::Generator(NucleoScope *scope){
    this->scope = scope;
    layout = new QVBoxLayout();
    GeneratorEdit = new QLineEdit();
    GeneratorSlider = new QSlider(Qt::Horizontal);
    //Slider contains frequency divider
    GeneratorSlider->setRange(-1, 65533);
    GeneratorSlider->setValue(65536-scope->settings->generatorDivider-1);
    valueChanged();
    layout->addWidget(GeneratorSlider);
    layout->addWidget(GeneratorEdit);


    connect(GeneratorSlider, &QAbstractSlider::valueChanged, this, &Generator::valueChanged);
    connect(GeneratorSlider, &QAbstractSlider::sliderReleased, this, &Generator::editingFinished);
    connect(GeneratorEdit, &QLineEdit::editingFinished, this, &Generator::editingFinished);



}



void Generator::valueChanged(){     //Value of slider changed callback
    //Inverting the slider to make small frequencies on left
    div = 65536-GeneratorSlider->value();
    if(div<3){
        div=3;
    }

    if(div>65536){                      //If divider greater than 65536, turn the generator off
        active = false;
        div=0;
        GeneratorEdit->setText("Off");
        scope->generatorFrequency = 0;
        scope->appSettings->generatorDivider = 0;
    }else{                              //Else calculate frequency from divider and display to user.
        active = true;
        float freq = TIMER8_FREQUENCY/(float)div;
        QString str;
        if(freq>1000000){
            str = QString::number(freq/1000000, 'f', 6) + "MHz";
        }else{
            str = QString::number(freq/1000, 'f', 6) + "kHz";
        }
        GeneratorEdit->setText(str);

        freq = TIMER8_FREQUENCY/(double)div;
        scope->generatorFrequency = freq;
        scope->appSettings->generatorDivider = div-1;   //Divider 0 in procesor means division by 1.
    }

}



void Generator::editingFinished(){  //Value in edit field changed callback
    QString str = GeneratorEdit->text();
    double freq = 0;
    QLocale c(QLocale::C);

    int multiplier = 1;

    if(str.contains('M', Qt::CaseInsensitive)){
        multiplier = 1000000;
    }
    if(str.contains('k', Qt::CaseInsensitive)){
        multiplier = 1000;
    }
    if(str.contains('o', Qt::CaseInsensitive)){
        multiplier = 0;
    }


    str = str.remove(QChar('k'), Qt::CaseInsensitive);
    str = str.remove(QChar('m'), Qt::CaseInsensitive);
    str = str.remove(QChar('h'), Qt::CaseInsensitive);
    str = str.remove(QChar('z'), Qt::CaseInsensitive);

    freq = c.toDouble(str);  //Use decimal point from OS language
    freq = freq*multiplier;

    if(freq == 0){          //If frequency is zero, turn off generator
        active = false;
        GeneratorEdit->setText("Off");
        scope->appSettings->generatorDivider = 0;
        scope->generatorFrequency = 0;
    }else{                  //Find closest divider based on user set frequency and apply it
        active = true;

        div = qRound(TIMER8_FREQUENCY/freq);
        if(div<2){div = 2;};
        if(div>65536){div = 65536;};

        freq = TIMER8_FREQUENCY/(double)div;



        if(freq>1000000){
            str = QString::number(freq/1000000, 'f', 6) + "MHz";
        }else{
            str = QString::number(freq/1000, 'f', 6) + "kHz";
        }


        scope->generatorFrequency = freq;
        GeneratorSlider->setValue(65536-div);
        GeneratorEdit->setText(str);


        scope->appSettings->generatorDivider = div-1;   //Divider 0 in procesor means division by 1.
        scope->writeSettings();
        qDebug() << "Generator frequency is: " << QString::number(freq);
    }

}



































