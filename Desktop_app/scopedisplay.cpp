#include "scopedisplay.h"


ScopeDisplay::ScopeDisplay(NucleoScope* scope)
{
    this->scope = scope;

    //Create grid and add chart(graph)
    DisplayGrid = new QGridLayout();
    DisplayGrid->setColumnStretch(1, 1);
    DisplayGrid->setRowStretch(1,1);
    displayChart = new QChart();
    displayChart->setMargins(QMargins(0, 0, 0, 0));
    chartView = new QChartView(displayChart);    
    DisplayGrid->addWidget(chartView, 0, 0, 3, 3);
    connect(this->scope, &NucleoScope::dataReady, this, &ScopeDisplay::redraw); //Connect data ready to redrawing function

    //Create graph axis with some default range
    axisX = new QValueAxis;
    axisY = new QValueAxis;
    axisX->setRange(0,1);
    axisY->setRange(-0.5,4);
    axisX->setTitleText("Samples");
    axisY->setTitleText("Amplitude(V)");
    displayChart->addAxis(axisX, Qt::AlignBottom);
    displayChart->addAxis(axisY, Qt::AlignLeft);

    //Add waveforms to chart and attach axis to it
    displayChart->addSeries(scope->ch1WaveForm->getSeries());
    scope->ch1WaveForm->getSeries()->attachAxis(axisX);
    scope->ch1WaveForm->getSeries()->attachAxis(axisY);

    displayChart->addSeries(scope->ch2WaveForm->getSeries());
    scope->ch2WaveForm->getSeries()->attachAxis(axisX);
    scope->ch2WaveForm->getSeries()->attachAxis(axisY);

    displayChart->addSeries(scope->ch3WaveForm->getSeries());
    scope->ch3WaveForm->getSeries()->attachAxis(axisX);
    scope->ch3WaveForm->getSeries()->attachAxis(axisY);

    displayChart->addSeries(scope->ch4WaveForm->getSeries());
    scope->ch4WaveForm->getSeries()->attachAxis(axisX);
    scope->ch4WaveForm->getSeries()->attachAxis(axisY);

    //Create cursors stuff
    cursor1 = new QLineSeries();
    cursor2 = new QLineSeries();
    cursorsInfoBox = new QGroupBox("Cursors info");
    cursorsInfoBox->setFixedWidth(180);
    cursorsInfoLayout = new QVBoxLayout();
    cursorsInfoBox->setLayout(cursorsInfoLayout);
    line1 = new QLabel();
    line2 = new QLabel();
    line3 = new QLabel();
    line4 = new QLabel();
    line5 = new QLabel();
    line6 = new QLabel();
    cursorsInfoLayout->addWidget(line1);
    cursorsInfoLayout->addWidget(line2);
    cursorsInfoLayout->addWidget(line3);
    cursorsInfoLayout->addWidget(line4);
    cursorsInfoLayout->addWidget(line5);
    cursorsInfoLayout->addWidget(line6);


    //Horizontal zoom controlls
    HZoomSlider = new QSlider(Qt::Horizontal);
    HZoomSlider->setMaximum(100);
    HZoomSlider->setMinimum(0);
    DisplayGrid->addWidget(HZoomSlider, 3,1);
    HZoomEdit = new QLineEdit("50");
    HZoomSlider->setValue(50);
    HZoomEdit->setMaximumWidth(EDIT_WIDTH);
    DisplayGrid->addWidget(HZoomEdit, 3, 2);
    DisplayGrid->addWidget(new QLabel("Horizontal\nexpansion"), 3, 0);
    connect(HZoomSlider, &QAbstractSlider::valueChanged, this, &ScopeDisplay::HZoomValueChanged);
    connect(HZoomEdit, &QLineEdit::editingFinished, this, &ScopeDisplay::HZoomEditingFinished);

    //Vertical zoom controlls
    VZoomSlider = new QSlider(Qt::Vertical);
    VZoomSlider->setRange(1, 200);
    DisplayGrid->addWidget(VZoomSlider, 1, 3, Qt::AlignHCenter);
    VZoomEdit = new QLineEdit("3.3 V");
    VZoomSlider->setValue(33);
    VZoomEdit->setMaximumWidth(EDIT_WIDTH);
    DisplayGrid->addWidget(VZoomEdit, 2, 3, Qt::AlignHCenter);
    DisplayGrid->addWidget(new QLabel("Vertical\nexpansion"), 0, 3, Qt::AlignHCenter);
    connect(VZoomSlider, &QAbstractSlider::valueChanged, this, &ScopeDisplay::VZoomValueChanged);
    connect(VZoomEdit, &QLineEdit::editingFinished, this, &ScopeDisplay::VZoomEditingFinished);

    //Horizontal offset controlls
    HOffsetSlider = new QSlider(Qt::Horizontal);
    DisplayGrid->addWidget(HOffsetSlider, 4, 1);
    HOffsetEdit = new QLineEdit("0");
    HOffsetEdit->setMaximumWidth(EDIT_WIDTH);
    DisplayGrid->addWidget(HOffsetEdit, 4, 2);
    DisplayGrid->addWidget(new QLabel("Horizontal\noffset"), 4, 0);
    connect(HOffsetSlider, &QAbstractSlider::valueChanged, this, &ScopeDisplay::HOffsetValueChanged);
    connect(HOffsetEdit, &QLineEdit::editingFinished, this, &ScopeDisplay::HOffsetEditingFinished);

    //Vertical offset controlls
    VOffsetSlider = new QSlider(Qt::Vertical);
    DisplayGrid->addWidget(VOffsetSlider, 1, 4, Qt::AlignHCenter);
    VOffsetEdit = new QLineEdit("0 V");
    VOffsetEdit->setMaximumWidth(EDIT_WIDTH);
    VOffsetSlider->setRange(-150, 150);
    VOffsetSlider->setValue(0);
    DisplayGrid->addWidget(VOffsetEdit, 2, 4, Qt::AlignHCenter);
    DisplayGrid->addWidget(new QLabel("Vertical\noffset"), 0, 4, Qt::AlignHCenter);
    connect(VOffsetSlider, &QAbstractSlider::valueChanged, this, &ScopeDisplay::VOffsetValueChanged);
    connect(VOffsetEdit, &QLineEdit::editingFinished, this, &ScopeDisplay::VOffsetEditingFinished);

    //Dividers selector
    selectorLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    selectorLayout->addLayout(vLayout);


    DisplayGrid->addLayout(selectorLayout, 5, 0, 1, 5);
    dividerList = new QListWidget();
    vLayout->addWidget(new QLabel("Sampling:\n\n\n\n\n\nDivider:"));
    selectorLayout->addWidget(dividerList);
    connect(dividerList, &QListWidget::itemSelectionChanged, this, &ScopeDisplay::samplingSelected);


    divEdit = new QLineEdit();
    divEdit->setFixedWidth(100);
    vLayout->addWidget(divEdit, Qt::AlignTop);
    connect(divEdit, &QLineEdit::editingFinished, this, &ScopeDisplay::divEdited);



}

int lengths[] = {2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 200, 350, 500, 1000};
#define HISTOGRAM_LENGHT = 15;
int histogramIndex(double len){
    int closestLengthIndex = 0;
    int closestLenghtDistance = 32000;
    for(int i = 0;i<15;i++){
        if(std::abs(len-lengths[i])<closestLenghtDistance){
            closestLenghtDistance=std::abs(len-lengths[i]);
            closestLengthIndex = i;
        }

    }
    return closestLengthIndex;
}


#define MIN_SAMPLING_DIVIDER 10
#define MAX_SAMPLING_JUMP 100
#define LIST_LENGTH 50
#define MIN_SAMPLES_PER_PERIOD 2
#define MAX_SAMPLES_PER_PERIOD 10000

//Compute dividers based on signal frequency, filer and add them to list

QStringList ScopeDisplay::computeDividers(int freq){
    dividerList->clear();
    possibleDividers.clear();
    possibleEqPeriod.clear();
    possiblePeriodLengths.clear();
    QStringList list;
    int n = 1;
    double Fs;
    double s;
    int histogram[15];
    for(int i = 0;i<15;i++){histogram[i]=0;};

    if(freq>100000){

        for(;n<MAX_SAMPLING_JUMP;n++){
            for(qint32 div = MIN_SAMPLING_DIVIDER;div<65536;div+=1){
                Fs=scope->baseFrequency/div;
                s=Fs/(Fs*n-freq);
                if(std::abs(s)>MIN_SAMPLES_PER_PERIOD && abs(s)<MAX_SAMPLES_PER_PERIOD){

                    double T = 1/(double)freq;
                    double Ts = 1/Fs;
                    double equivalentSamplingPeriod = -Ts+T*(double)n;
                    double samplingFrequency = (double)scope->baseFrequency/(double)div;



                    if(histogram[histogramIndex(s)]>0){
                    }else{
                        histogram[histogramIndex(s)]++;
                        possibleDividers.append(div);
                        possibleEqPeriod.append(equivalentSamplingPeriod);
                        possiblePeriodLengths.append(s);
                        list<<"Divider:" + QString::number(div) + " | Sampling frequency:" + QString::number(samplingFrequency/1000000, 'f', 3) + "MHz | Equivalent sampling frequency: " + QString::number(0.000001/equivalentSamplingPeriod, 'f', 3) + "MHz | Samples per period:" + QString::number(s,'f', 1) + " | Periodes skipped: " + QString::number(n-1);
                    }
                }
            }

        }
    }else{
        for(qint32 div = MIN_SAMPLING_DIVIDER;div<65536;div+=1){
            Fs=scope->baseFrequency/div;
            s=Fs/freq;
            if(std::abs(s)>MIN_SAMPLES_PER_PERIOD && abs(s)<MAX_SAMPLES_PER_PERIOD){


                if(histogram[histogramIndex(s)]>0){
                }else{
                    histogram[histogramIndex(s)]++;
                    possibleDividers.append(div);
                    possibleEqPeriod.append(1/(double)div);
                    possiblePeriodLengths.append(s);
                    list<<"Divider:" + QString::number(div) + " | Samples per period:" + QString::number(s,'f', 1);
                }
            }
        }

    }
    dividerList->addItems(list);
    return list;
}

//Draw aditional series into chart with color col. Currently only used for math.
void ScopeDisplay::drawSeries(QLineSeries *series, QColor col){
    if(!colors.contains(col)){
        colors.append(col);
        displayChart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
        series->setColor(col);
        qDebug()<<"Drawing aditional series " << series->name();
    }

}

//Automaticly select divider and scale axis.
void ScopeDisplay::autoScale(){
    scope->autosetInfo->setValue(90);
    computeDividers(scope->settings->frequency);
    double bestSpp = 0;
    int bestIndex = 0;
    for(int i = 0;i<possiblePeriodLengths.length();i++){
        double len = possiblePeriodLengths[i];
        if(len>bestSpp && len<100){
            bestIndex = i;
            bestSpp = len;
        }
    }
    dividerList->setCurrentRow(bestIndex);
    scope->appSettings->memDepth = bestSpp*10;
    emit memDepthChanged();
    HZoomSlider->setValue(bestSpp*2);
    scope->autosetInfo->setValue(100);
    scope->autosetInfo->close();
}


//If user manually edits divider, parse it form string. Change X axis to samples since time can not be computed from divider only.
void ScopeDisplay::divEdited(){
    scope->displayTime = false;
    emit samplingChanged((int)scope->displayTime);
    scope->equivalentSamplingPeriod = 0;
    dividerList->clearSelection();
    int div = divEdit->text().toInt();
    if(div<9){
        div = 9;
    }
    if(div>65536){
        div = 65536;
    }
    scope->appSettings->samplingDivider = div-1;
    divEdit->setText(QString::number(div));
    scope->displayTime = false;

}


//
void ScopeDisplay::redraw(){
    QElapsedTimer timer;
    timer.start();
    double samplesPerPeriod;
    if(scope->equivalentSamplingPeriod>0){
        samplesPerPeriod = (1/(double)scope->settings->frequency)/scope->equivalentSamplingPeriod;
    }else{
        samplesPerPeriod = 0;
    }
    //Sets limits of horizontal sliders based on data length and adds margin for triggering.
    HOffsetSlider->setMaximum(scope->ch1WaveForm->dataLength-std::abs(samplesPerPeriod*2)-HZoom);
    HZoomSlider->setMaximum(scope->ch1WaveForm->dataLength-std::abs(samplesPerPeriod*2));
    HOffsetSlider->setMinimum(1);
    HZoomSlider->setMinimum(1);

    updateAxis();   //Updates axis based on currrent user selection.
    displayChart->removeAllSeries();    //All series have to be removed from chart before updating them. Otherwise the chart would update upon every point beeing added to series and that takes a long time.
    colors.clear();

    //Triggers channel one and add it chart.
    if(scope->settings->channel1On){ //This is redundant since channel 1 can not be turned of anymore.
        scope->ch1WaveForm->trigger(triggerLevel); //Trigger level is (max+min)/2
        displayChart->addSeries(scope->ch1WaveForm->getSeries());
        scope->ch1WaveForm->getSeries()->attachAxis(axisX);
        scope->ch1WaveForm->getSeries()->attachAxis(axisY);
        scope->ch1WaveForm->getSeries()->setColor(QColor(32, 159, 233));
    }

    //Choose unite of time to use for X axis
    if(scope->displayTime){

        switch(scope->ch1WaveForm->timeDivider){
        case 1:
            axisX->setTitleText("Time(s)");
            xAxisUnit = "s";
            xAxisFrequencyUnit = "mHz";
            break;
        case 1000:
            axisX->setTitleText("Time(ms)");
            xAxisUnit = "ms";
            xAxisFrequencyUnit = "Hz";
            break;
        case 1000000:
            axisX->setTitleText("Time(us)");
            xAxisUnit = "us";
            xAxisFrequencyUnit = "kHz";
            break;
        case 1000000000:
            axisX->setTitleText("Time(ns)");
            xAxisUnit = "ns";
            xAxisFrequencyUnit = "MHz";
            break;
        }
    }else{
        axisX->setTitleText("Samples");
        xAxisUnit = "";
    }

    //Add all other active channels to chart. Trigger them using trigger point from channel 1
    if(scope->settings->channel2On){
        scope->ch2WaveForm->trigger(scope->ch1WaveForm->triggerPoint);
        displayChart->addSeries(scope->ch2WaveForm->getSeries());
        scope->ch2WaveForm->getSeries()->attachAxis(axisX);
        scope->ch2WaveForm->getSeries()->attachAxis(axisY);
        scope->ch2WaveForm->getSeries()->setColor(QColor(153, 202, 83));
    }
    if(scope->settings->channel3On){
        scope->ch3WaveForm->trigger(scope->ch1WaveForm->triggerPoint);
        displayChart->addSeries(scope->ch3WaveForm->getSeries());
        scope->ch3WaveForm->getSeries()->attachAxis(axisX);
        scope->ch3WaveForm->getSeries()->attachAxis(axisY);
        scope->ch3WaveForm->getSeries()->setColor(QColor(246, 166, 37));
    }
    if(scope->settings->channel4On){
        scope->ch4WaveForm->trigger(scope->ch1WaveForm->triggerPoint);
        displayChart->addSeries(scope->ch4WaveForm->getSeries());
        scope->ch4WaveForm->getSeries()->attachAxis(axisX);
        scope->ch4WaveForm->getSeries()->attachAxis(axisY);
        scope->ch4WaveForm->getSeries()->setColor(QColor(109, 95, 213));
    }




    if(scope->generatorFrequency == 0){ //If generator is off, calculate dividers based on measured frequency.
        qDebug() << "Signal frequency changed. Calculating dividers";
        if(scope->settings->frequency!=lastFrequency){  //Only calculate new dividers if frequency changed.
            computeDividers(scope->settings->frequency);
            lastFrequency = scope->settings->frequency;
        }
    }else{  //Calculate dividers based on freqency of generator.

        qDebug() << "Generator frequency is " << scope->generatorFrequency;
        qDebug() << "Last frequency is " << lastFrequency;
        if(scope->generatorFrequency!=lastFrequency){   //Only calculate new dividers if frequency changed.
            qDebug() << "Generator frequency changed. Calculating dividers";
            computeDividers(scope->generatorFrequency);
            lastFrequency=scope->generatorFrequency;
        }
    }


    //If osciloscope is not stopped, request new measurment by sending settings
    if(!scope->stopped){
        scope->writeSettings();
    }

    updateCursors();    //Update cursors pased on user selection
    displayChart->update();
    qDebug() << "Time spent redrawing: " << timer.elapsed() << "ms";

}


void ScopeDisplay::updateAxis(){ //Updates axis based on currrent user selection.

    QLineSeries *series = scope->ch1WaveForm->getSeries();
    if(series->count()>HZoom+HOffset && HZoom+HOffset>0){
        qDebug() << series->count();
        double rangeMax = series->at(HZoom+HOffset).x();
        double rangeMin = series->at(HOffset).x();
        axisX->setRange(rangeMin, rangeMax);

        double YRange = (VZoom*0.1);
        double yRangeMin = VOffset;
        double yRangeMax = yRangeMin+YRange;
        axisY->setRange(yRangeMin, yRangeMax);
    }




}

void ScopeDisplay::removeCurors(){ //Pretty self explanatory

    if(displayChart->series().contains(cursor1)){
        displayChart->removeSeries(cursor1);
        displayChart->removeSeries(cursor2);
        delete cursor1;
        delete cursor2;
    }
}

void ScopeDisplay::cursorsPlacer(){ //Add sursors to chart
    cursor1 = new QLineSeries();
    displayChart->addSeries(cursor1);
    cursor1->setName("Cursor A");
    cursor1->attachAxis(axisX);
    cursor1->attachAxis(axisY);
    cursor1->setColor(QColor(255, 0, 0, 200));
    cursor2 = new QLineSeries();
    displayChart->addSeries(cursor2);
    cursor2->setName("Cursor B");
    cursor2->attachAxis(axisX);
    cursor2->attachAxis(axisY);
    cursor2->setColor(QColor(0, 0, 0, 200));


}

//Update cursors based on user selection
void ScopeDisplay::updateCursors(){
    //Set cursors invisible if they are off
    if(cursorsType){
        selectorLayout->addWidget(cursorsInfoBox);
        cursorsInfoBox->setVisible(true);
    }else{
        selectorLayout->removeWidget(cursorsInfoBox);
        cursorsInfoBox->setVisible(false);
    }
    QLineSeries *source;
    bool allowTracking = false;

    QList<QtCharts::QAbstractSeries*> ser = displayChart->series();
    QList<QString> names;
    for(int i = 0;i<ser.length();i++){
        names.append(ser.at(i)->name());
    }

    //Select source for cursors. Only really usefull for tracking cursors.
    switch(cursorChannel){
    case 0:
        if(scope->settings->channel1On){
            source = scope->ch1WaveForm->getSeries();
            allowTracking = true;
        }
        break;
    case 1:
        if(scope->settings->channel2On){
            source = scope->ch2WaveForm->getSeries();
            allowTracking = true;
        }
        break;
    case 2:
        if(scope->settings->channel3On){
            source = scope->ch3WaveForm->getSeries();
            allowTracking = true;
        }
        break;
    case 3:
        if(scope->settings->channel4On){
            source = scope->ch4WaveForm->getSeries();
            allowTracking = true;
        }
        break;
    case 4:
        if(names.contains("Math 1")){
            source = (QLineSeries*)ser.at(names.indexOf("Math 1"));
            allowTracking = true;
        }
        break;
    case 5:
        if(names.contains("Math 2")){
            source = (QLineSeries*)ser.at(names.indexOf("Math 2"));
            allowTracking = true;
        }
        break;
    case 6:
        if(names.contains("Math 3")){
            source = (QLineSeries*)ser.at(names.indexOf("Math 3"));
            allowTracking = true;
        }
        break;
    }

    //Calculate cursors position and information
    switch(cursorsType){
    case 1: {   //X axis(time) cursors
        cursorsPlacer();
        double xRange = axisX->max()-axisX->min();
        double cursor1Pos = axisX->min()+0.002*(double)cursor1Position*xRange;
        cursor1->append(cursor1Pos, axisY->min());
        cursor1->append(cursor1Pos, axisY->max());

        double cursor2Pos = axisX->min()+0.002*(double)cursor2Position*xRange;
        cursor2->append(cursor2Pos, axisY->min());
        cursor2->append(cursor2Pos, axisY->max());

        //Draw information about cursors position
        line1->setText("A:     " + QString::number(cursor1Pos, 'f', 3) + xAxisUnit);
        line2->setText("B:     " + QString::number(cursor2Pos, 'f', 3) + xAxisUnit);
        line3->setText("∆X:    " + QString::number(cursor1Pos-cursor2Pos, 'f', 3) + xAxisUnit);
        if(scope->displayTime){
            line4->setText("1/∆X:  " + QString::number(1000/(cursor1Pos-cursor2Pos))+xAxisFrequencyUnit);
        }else{
            line4->setText("");
        }
        line5->setText("");
        line6->setText("");
        break;
    }
    case 2: {   //Y axis (amplitude) cursor
        cursorsPlacer();
        double yRange = axisY->max()-axisY->min();
        double cursor1Pos = axisY->min()+0.002*(double)cursor1Position*yRange;
        cursor1->append(0, cursor1Pos);
        cursor1->append(axisX->max(), cursor1Pos);

        double cursor2Pos = axisY->min()+0.002*(double)cursor2Position*yRange;
        cursor2->append(0, cursor2Pos);
        cursor2->append(axisX->max(), cursor2Pos);

        //Draw information about cursors position
        line1->setText("A:  " + QString::number(cursor1Pos, 'f', 3) + "V");
        line2->setText("B:  " + QString::number(cursor2Pos, 'f', 3) + "V");
        line3->setText("∆Y: " + QString::number(cursor1Pos-cursor2Pos, 'f', 3) + "V");
        line4->setText("");
        line5->setText("");
        line6->setText("");
        break;
    }
    case 3: {   //Tracking cursors. User sets time, cursor tracks voltage.
        if(allowTracking){
            cursorsPlacer();
            double xRange = axisX->max()-axisX->min();
            double cursor1xPos = axisX->min()+0.002*(double)cursor1Position*xRange;

            double smallestDistance = 10000;
            int smallestDistanceIndex = 0;
            for(int i = 1; i < source->count();i++){
                double dis = std::abs(cursor1xPos-source->at(i).x());
                if(dis<smallestDistance){
                    smallestDistance = dis;
                    smallestDistanceIndex = i;
                }
            }
            cursor1xPos = source->at(smallestDistanceIndex).x();
            double cursor1yPos = source->at(smallestDistanceIndex).y();

            qDebug() << "Cursor1pos: " + QString::number(cursor1xPos);
            cursor1->append(cursor1xPos, axisY->min());
            cursor1->append(cursor1xPos, 2*axisY->max());
            cursor1->append(0,2*axisY->max());
            cursor1->append(0, cursor1yPos);
            cursor1->append(2*axisX->max(), cursor1yPos);

            double cursor2xPos = axisX->min()+0.002*(double)cursor2Position*xRange;

            smallestDistance = 10000;
            smallestDistanceIndex = 0;
            for(int i = 1; i < source->count();i++){
                double dis = std::abs(cursor2xPos-source->at(i).x());
                if(dis<smallestDistance){
                    smallestDistance = dis;
                    smallestDistanceIndex = i;
                }
            }
            cursor2xPos = source->at(smallestDistanceIndex).x();
            double cursor2yPos = source->at(smallestDistanceIndex).y();

            qDebug() << "Cursor2pos: " + QString::number(cursor2xPos);
            cursor2->append(cursor2xPos, axisY->min());
            cursor2->append(cursor2xPos, 2*axisY->max());
            cursor2->append(0,2*axisY->max());
            cursor2->append(0, cursor2yPos);
            cursor2->append(2*axisX->max(), cursor2yPos);

            //Draw information about cursors position
            line1->setText("Ax: " + QString::number(cursor1xPos, 'f', 3) + xAxisUnit);
            line2->setText("Bx: " + QString::number(cursor2xPos, 'f', 3) + xAxisUnit);
            line3->setText("∆X: " + QString::number(cursor1xPos-cursor2xPos, 'f', 3) + xAxisUnit);
            line4->setText("Ay: " + QString::number(cursor1yPos, 'f', 3) + "V");
            line5->setText("By: " + QString::number(cursor2yPos, 'f', 3) + "V");
            line6->setText("∆Y: " + QString::number(cursor1yPos-cursor2yPos, 'f', 3) + "V");

            break;
        }
    }
    }








}

void ScopeDisplay::cursorsSettingsChanged(int type, int channel, int cur1, int cur2){
    cursorsType = type;
    cursorChannel = channel;
    cursor1Position = cur1;
    cursor2Position = cur2;
    removeCurors();
    updateCursors();
}

void ScopeDisplay::samplingSelected(){

    int row = dividerList->currentRow();
    scope->equivalentSamplingPeriod = possibleEqPeriod[row];

    if(possibleDividers.length()>row){
        scope->appSettings->samplingDivider = possibleDividers[row]-1;

    }
    divEdit->setText(QString::number(possibleDividers[row]));
    scope->displayTime = true;
    emit samplingChanged((int)scope->displayTime);

    HZoomSlider->setValue(abs(possiblePeriodLengths[row])*2);
}



QGridLayout* ScopeDisplay::getDisplay(){
    return DisplayGrid;
}



void ScopeDisplay::VOffsetEditingFinished(){
    VOffsetEdit->setText(VOffsetEdit->text().remove('V'));
    VOffsetEdit->setText(VOffsetEdit->text().replace(',', '.'));
    VOffset = VOffsetEdit->text().toDouble();
    if(VOffset<-VOFFSETMAX){VOffset = -VOFFSETMAX;}
    else if (VOffset>VOFFSETMAX){VOffset = VOFFSETMAX;}
    VOffsetSlider->setValue((VOffset/VOFFSETMAX)*(-100));
    VOffsetEdit->setText(QString::number(VOffset, 'f', 1) + " V");
    updateAxis();
    removeCurors();
    updateCursors();
}

void ScopeDisplay::VOffsetValueChanged(){
    VOffset = VOFFSETMAX*0.01*(-1*VOffsetSlider->value());
    VOffsetEdit->setText(QString::number(VOffset, 'f', 1) + " V");
    updateAxis();
    removeCurors();
    updateCursors();
}

void ScopeDisplay::HOffsetEditingFinished(){
    HOffset = ScopeDisplay::parseNumber(HOffsetEdit->text(), HOffsetSlider->maximum());
    HOffsetSlider->setValue(HOffset);
    HOffsetEdit->setText(QString::number(HOffset));
    updateAxis();
    removeCurors();
    updateCursors();
}
void ScopeDisplay::HOffsetValueChanged(){
    HOffset = HOffsetSlider->value();
    HOffsetEdit->setText(QString::number(HOffset));
    updateAxis();
    removeCurors();
    updateCursors();
}
void ScopeDisplay::VZoomEditingFinished(){
    VZoomEdit->setText(VZoomEdit->text().remove('V'));
    VZoomEdit->setText(VZoomEdit->text().replace(',', '.'));
    VZoom = (qint32)(VZoomEdit->text().toDouble()*10+0.5);
    if(VZoom < 1){
        VZoom = 1;
    }
    if(VZoom > 200){
        VZoom = 200;
    }
    VZoomSlider->setValue(VZoom);
    VZoomEdit->setText(QString::number((double)VZoom/10, 'f', 1) + " V");
    updateAxis();
    removeCurors();
    updateCursors();
}
void ScopeDisplay::VZoomValueChanged(){
    VZoom = VZoomSlider->value();
    VZoomEdit->setText(QString::number((double)VZoom/10, 'f', 1) + " V");
    updateAxis();
    removeCurors();
    updateCursors();
}
void ScopeDisplay::HZoomEditingFinished(){
    HZoom = ScopeDisplay::parseNumber(HZoomEdit->text(), HZoomSlider->maximum());
    HZoomSlider->setValue(HZoom);
    HZoomEdit->setText(QString::number(HZoom));
    updateAxis();
    removeCurors();
    updateCursors();

}
void ScopeDisplay::HZoomValueChanged(){
    HZoom = HZoomSlider->value();
    HZoomEdit->setText(QString::number(HZoom));
    updateAxis();
    removeCurors();
    updateCursors();
}

//Parse number from string to value from 0 to max
int ScopeDisplay::parseNumber(QString value, int max){
    int number = value.toInt();
    if(number < 0){
        return 0;
    }
    if(number > max){
        return max;
    }
    return number;
}












