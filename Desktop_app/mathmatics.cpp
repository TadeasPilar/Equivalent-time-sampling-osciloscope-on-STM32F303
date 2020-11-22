#include "mathmatics.h"

MathMatics::MathMatics(NucleoScope *scope)
{
    this->scope = scope;
    layout = new QVBoxLayout();
    //Create 3 math operations
    op1 = new MathOperation(scope, 1);
    layout->addLayout(op1->layout);
    op2 = new MathOperation(scope, 2);
    layout->addLayout(op2->layout);
    op3 = new MathOperation(scope, 3);
    layout->addLayout(op3->layout);


}



MathOperation::MathOperation(NucleoScope *scope, int identifier){   //Class form math operation
    this->scope = scope;
    id = identifier;
    layout = new QHBoxLayout();

    //Create function selector
    layout->addWidget(new QLabel("Math " + QString::number(identifier)), Qt::AlignLeft);
    mathOperationSelector = new QComboBox();
    mathOperationSelector->addItem("Off");
    mathOperationSelector->addItem("FFT");
    mathOperationSelector->addItem("A+B");
    mathOperationSelector->addItem("A-B");
    mathOperationSelector->addItem("A*B");
    mathOperationSelector->setFixedWidth(60);
    layout->addWidget(mathOperationSelector, Qt::AlignLeft);
    connect(mathOperationSelector, SIGNAL(currentIndexChanged(int)), this,SLOT(operationChanged(int)));

    //Create channel A selector
    channelASelector = new QComboBox();
    channelASelector->addItem("A: CH1");
    channelASelector->addItem("A: CH2");
    channelASelector->addItem("A: CH3");
    channelASelector->addItem("A: CH4");
    layout->addWidget(channelASelector, Qt::AlignLeft);
    channelASelector->setVisible(false);
    connect(channelASelector, SIGNAL(currentIndexChanged(int)), this, SLOT(channellAChanged(int)));

    //Create channel B selector
    channelBSelector = new QComboBox();
    channelBSelector->addItem("B: CH1");
    channelBSelector->addItem("B: CH2");
    channelBSelector->addItem("B: CH3");
    channelBSelector->addItem("B: CH4");
    layout->addWidget(channelBSelector, Qt::AlignLeft);
    channelBSelector->setVisible(false);
    connect(channelBSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(channellBChanged(int)));


}

void MathOperation::recalculateMath(){
    qDebug() << "Recalculating math";

    int displayStyle = waveA->displayStyle; //Coppy display style of wave A
    if(displayStyle>2){
        mathSeries = new QSplineSeries();
    }else{
        mathSeries = new QLineSeries();
    }
    if(displayStyle==0 || displayStyle==4){
        mathSeries->setPointsVisible(true);
    }else{
        mathSeries->setPointsVisible(false);
    }

    mathSeries->setName("Math " + QString::number(id));
    QColor col;
    switch(id){         //Decide witch color to use
    case 1:
        col = QColor(Qt::magenta);
        break;
    case 2:
       col = QColor(Qt::darkGreen);
        break;
    case 3:
        col = QColor(Qt::darkBlue);
        break;
    }


    switch(mathOperation){      //Decide witch math operation to use
    case 0:
        break;
    case 1:
            QMessageBox::information(nullptr, "Error", "Not implemented yet");
            mathOperation = 0;
        break;
    case 2:{
        for(int i = 0;i<waveA->dataLength;i++){
            mathSeries->append(waveA->timeVector.at(i), waveA->data[i+waveA->triggerPoint]+waveB->data[i+waveA->triggerPoint]);
        }
        emit seriesReady(mathSeries, col);
        break;
    }
    case 3:
        for(int i = 0;i<waveA->dataLength;i++){
            mathSeries->append(waveA->timeVector.at(i), waveA->data[i+waveA->triggerPoint]-waveB->data[i+waveA->triggerPoint]);
        }
        emit seriesReady(mathSeries, col);

        break;
    case 4:
        for(int i = 0;i<waveA->dataLength;i++){
            mathSeries->append(waveA->timeVector.at(i), waveA->data[i+waveA->triggerPoint]*waveB->data[i+waveA->triggerPoint]);
        }
        emit seriesReady(mathSeries, col);
        break;
    }
    AReady = false;
    BReady = false;



}


void MathOperation::channellAReady(){
    qDebug() << "Source A ready";
    AReady = true;
    if(mathOperation == 1){     //If operation is FFT, there is no second channel
        recalculateMath();
    }else if(BReady){           //Checek if data from channel B is ready
        recalculateMath();
    }
}

void MathOperation::channellBReady(){
    qDebug() << "Source B ready";
    BReady = true;
    if(AReady){                 //Checek if data from channel A is ready
        recalculateMath();
    }
}

void MathOperation::channellAChanged(int index){        //Connect waveform to this math operation
    mathAChannel = index+1;
    disconnect(nullptr, nullptr, this, SLOT(channellAReady()));
    switch(index){
    case 0:
        waveA = scope->ch1WaveForm;
        break;
    case 1:
        waveA = scope->ch2WaveForm;
        break;
    case 2:
        waveA = scope->ch3WaveForm;
        break;
    case 3:
        waveA = scope->ch4WaveForm;
        break;
    }
    connect(waveA, &WaveForm::triggered, this, &MathOperation::channellAReady);
}

void MathOperation::channellBChanged(int index){        //Connect waveform to this math operation
    mathBChannel = index+1;
    disconnect(nullptr, nullptr, this, SLOT(channellBReady()));
    switch(index){
    case 0:
        waveB = scope->ch1WaveForm;
        break;
    case 1:
        waveB = scope->ch2WaveForm;
        break;
    case 2:
        waveB = scope->ch3WaveForm;
        break;
    case 3:
        waveB = scope->ch4WaveForm;
        break;
    }
    connect(waveB, &WaveForm::triggered, this, &MathOperation::channellBReady);
}

void MathOperation::operationChanged(int index){
    mathOperation = index;
    switch(index){
    case 0:                     //This math is turned off. Hide channel selectors.
        channelASelector->setVisible(false);
        channelBSelector->setVisible(false);
        disconnect(nullptr, nullptr, this, SLOT(channellAReady()));
        disconnect(nullptr, nullptr, this, SLOT(channellBReady()));
        break;
    case 1:                     //This math operation uses only  channel, hide second channel selector.
        channelASelector->setVisible(true);
        channellAChanged(channelASelector->currentIndex());
        channelBSelector->setVisible(false);
        disconnect(nullptr, nullptr, this, SLOT(channellBReady()));
        break;
    case 2:                     //All other operations use both channels
    case 3:
    case 4:
        channelASelector->setVisible(true);
        channelBSelector->setVisible(true);
        channellAChanged(channelASelector->currentIndex());
        channellBChanged(channelBSelector->currentIndex());
        break;
    }
}








