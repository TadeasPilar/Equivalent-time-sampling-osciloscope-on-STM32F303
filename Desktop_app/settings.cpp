#include "settings.h"


Settings::Settings(NucleoScope *scope)
{

    QColor col = QColor(Qt::darkGray);
    QString qss = QString("background-color: %1").arg(col.name());


    this->scope = scope;
    grid = new QGridLayout();

    //Create channel 1 button
    ch1Button = new QPushButton("CH1");
    ch1Button->setFixedWidth(50);
    QColor blu = QColor(32, 159, 233);
    QString BLU = QString("background-color: %1").arg(blu.name());
    ch1Button->setStyleSheet(BLU);
    grid->addWidget(ch1Button, 0, 0);
    connect(ch1Button, &QPushButton::pressed, this, &Settings::ch1ButtonPressed);

    //Create channel 2 button
    ch2Button = new QPushButton("CH2");
    ch2Button->setFixedWidth(50);
    ch2Button->setStyleSheet(qss);
    grid->addWidget(ch2Button, 0, 1);
    connect(ch2Button, &QPushButton::pressed, this, &Settings::ch2ButtonPressed);

    //Create channel 3 button
    ch3Button = new QPushButton("CH3");
    ch3Button->setFixedWidth(50);
    ch3Button->setStyleSheet(qss);
    grid->addWidget(ch3Button, 0, 2);
    connect(ch3Button, &QPushButton::pressed, this, &Settings::ch3ButtonPressed);

    //Create channel 4 button
    ch4Button = new QPushButton("CH4");
    ch4Button->setFixedWidth(50);
    ch4Button->setStyleSheet(qss);
    grid->addWidget(ch4Button, 0, 3);
    connect(ch4Button, &QPushButton::pressed, this, &Settings::ch4ButtonPressed);

    //Create memory depth edit
    grid->addWidget(new QLabel("Memory depth"), 1, 0, 1, 2);
    memDepth = new QLineEdit("500 pts");
    grid->addWidget(memDepth, 1, 2, 1, 2);
    connect(memDepth, &QLineEdit::editingFinished, this, &Settings::memDepthEdited);

    //Create autoset button
    autoset = new QPushButton("AutoSet");
    grid->addWidget(autoset, 2, 0, 1, 2);
    connect(autoset, &QPushButton::pressed, this, &Settings::autoSetButtonPressed);

    //Create autoscale button
    autoscale = new QPushButton("AutoScale");
    grid->addWidget(autoscale, 3, 0, 1, 2);
    connect(autoscale, &QPushButton::pressed, this, &Settings::autoScaleButtonPressed);

    //Create start/stop button
    startStop = new QPushButton("Start/Stop");
    grid->addWidget(startStop, 2, 2, 1, 2);
    connect(startStop, &QPushButton::pressed, this, &Settings::startStopButtonPressed);
    startStop->setStyleSheet(QString("background-color: %1").arg(QColor(Qt::green).name()));
    startStop->setText("Stop");

    //Create single button
    single = new QPushButton("Single");
    grid->addWidget(single, 3, 2, 1, 2);
    connect(single, &QPushButton::pressed, this, &Settings::singleButtonPressed);

    //Create
    equivalentTimeBox = new QComboBox();
    equivalentTimeBox->addItem("Samples");
    equivalentTimeBox->addItem("Equivalent time");
    connect(equivalentTimeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(equivalentTimeBoxChanged(int)));
    grid->addWidget(new QLabel("Time base"), 4, 0, 1, 2);
    grid->addWidget(equivalentTimeBox, 4, 2, 1, 2);

    displayStyleBox = new QComboBox();
    displayStyleBox->addItem("Line+Dots");
    displayStyleBox->addItem("Line");
    displayStyleBox->addItem("Spline");
    displayStyleBox->addItem("Spline+Dots");
    connect(displayStyleBox, SIGNAL(currentIndexChanged(int)), this, SLOT(displayStyleBoxChanged(int)));
    grid->addWidget(new QLabel("Display style"), 5, 0, 1, 2);
    grid->addWidget(displayStyleBox, 5, 2, 1, 2);

}



void Settings::autoSetButtonPressed(){
    scope->autosetState = 1;
    if(scope->stopped){
        scope->stopped=false;
        startStop->setStyleSheet(bgCol(QColor(Qt::green)));
        startStop->setText("Stop");
        scope->writeSettings();
    }


}

void Settings::autoScaleButtonPressed(){
    scope->autosetState = 5;
    if(scope->stopped){
        scope->stopped=false;
        startStop->setStyleSheet(bgCol(QColor(Qt::green)));
        startStop->setText("Stop");
        scope->writeSettings();
    }


}


void Settings::displayStyleBoxChanged(int index){
    scope->ch1WaveForm->displayStyle = index;
    scope->ch2WaveForm->displayStyle = index;
    scope->ch3WaveForm->displayStyle = index;
    scope->ch4WaveForm->displayStyle = index;
}


void Settings::equivalentTimeBoxChanged(int index){
    if(scope->equivalentSamplingPeriod>0){
        if(index){
            equivalentTimeBox->setCurrentIndex(1);
            scope->displayTime=true;
        }else{
            equivalentTimeBox->setCurrentIndex(0);
            scope->displayTime=false;
        }
    }else{
        equivalentTimeBox->setCurrentIndex(0);
        QMessageBox::critical(nullptr, "Error", "Select sampling frequency from list before enabling Equivalent time");
    }

}


void Settings::startStopButtonPressed(){
    if(scope->stopped){
        scope->stopped=false;
        startStop->setStyleSheet(bgCol(QColor(Qt::green)));
        startStop->setText("Stop");
        scope->writeSettings();

    }else{
        scope->stopped = true;
        startStop->setStyleSheet(bgCol(QColor(Qt::red)));
        startStop->setText("Start");
    }
}


void Settings::singleButtonPressed(){
    scope->stopped = true;
    startStop->setStyleSheet(bgCol(QColor(Qt::red)));
    startStop->setText("Start");
    scope->writeSettings();



}



void Settings::ch1ButtonPressed(){
    QMessageBox::warning(nullptr, "CH1 can't be turned off", "Most of this program funcions are based on CH1 and as such can't be turned off.");
}




void Settings::ch2ButtonPressed(){
    if(!scope->settings->channel2On){
        ch2Button->setStyleSheet(bgCol(QColor(153, 202, 83)));
        scope->appSettings->channel2On = true;
        //scope->writeSettings();
    }else{
        ch2Button->setStyleSheet(bgCol(QColor(Qt::darkGray)));
        scope->appSettings->channel2On = false;
        //scope->writeSettings();
    }
}
void Settings::ch3ButtonPressed(){
    if(!scope->settings->channel3On){
        ch3Button->setStyleSheet(bgCol(QColor(246, 166, 37)));
        scope->appSettings->channel3On = true;
        //scope->writeSettings();
    }else{
        ch3Button->setStyleSheet(bgCol(QColor(Qt::darkGray)));
        scope->appSettings->channel3On = false;
        //scope->writeSettings();
    }
}
void Settings::ch4ButtonPressed(){
    if(!scope->settings->channel4On && !scope->settings->channel4Vref){
        ch4Button->setStyleSheet(bgCol(QColor(109, 95, 213)));
        scope->appSettings->channel4On = true;
        //scope->writeSettings();
    }else if(scope->settings->channel4On && !scope->settings->channel4Vref){
        QMessageBox::information(nullptr, "", "Please make sure, that no signal is connected to CH4 input before using Vref.");
        scope->appSettings->channel4Vref = true;
        ch4Button->setText("Vref");
    }else{
        ch4Button->setText("CH4");
        ch4Button->setStyleSheet(bgCol(QColor(Qt::darkGray)));
        scope->appSettings->channel4On = false;
        scope->appSettings->channel4Vref = false;
        //scope->writeSettings();
    }
}

QString Settings::bgCol(QColor col){
    return QString("background-color: %1").arg(col.name());
}

void Settings::memDepthEdited(){
    int multiplier = 1;
    int depth;
    QString str = memDepth->text();
    if(str.contains('k', Qt::CaseInsensitive)){
        multiplier = 1000;
    }

    str = str.remove(QChar('k'), Qt::CaseInsensitive);
    str = str.remove(QChar('p'), Qt::CaseInsensitive);
    str = str.remove(QChar('t'), Qt::CaseInsensitive);
    str = str.remove(QChar('s'), Qt::CaseInsensitive);

    depth = str.toInt();
    depth = depth*multiplier;
    if(depth > 12000){
        depth = 12000;
    }

    if(depth<1){
        depth = 1;
    }

    if(depth>1000){
        str = QString::number(depth/1000) + "kpts";
    }else{
        str = QString::number(depth) + "pts";
    }
    memDepth->setText(str);
    scope->appSettings->memDepth = depth;

}

void Settings::memDepthChanged(){


    int depth;
    QString str;

    depth = scope->appSettings->memDepth;
    if(depth > 12000){
        depth = 12000;
    }

    if(depth<1){
        depth = 1;
    }

    if(depth>1000){
        str = QString::number(depth/1000) + "kpts";
    }else{
        str = QString::number(depth) + "pts";
    }
    memDepth->setText(str);
}













