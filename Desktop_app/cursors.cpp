#include "cursors.h"

Cursors::Cursors(NucleoScope *scope)
{
    //Create cursor type selector
    this->scope = scope;
    grid = new QGridLayout();
    cursorType = new QComboBox();
    cursorType->addItem("Off");
    cursorType->addItem("X");
    cursorType->addItem("Y");
    cursorType->addItem("Tracking");
    cursorType->setFixedWidth(100);
    connect(cursorType, SIGNAL(currentIndexChanged(int)), this, SLOT(cursorTypeChanged(int)));
    grid->addWidget(cursorType, 0, 1, 1, 1);

    //Create cursor channel selector
    cursorType = new QComboBox();
    cursorType->addItem("CH1");
    cursorType->addItem("CH2");
    cursorType->addItem("CH3");
    cursorType->addItem("CH4");
    cursorType->addItem("Math 1");
    cursorType->addItem("Math 2");
    cursorType->addItem("Math 3");
    cursorType->setCurrentIndex(0);
    cursorType->setFixedWidth(65);
    connect(cursorType, SIGNAL(currentIndexChanged(int)), this, SLOT(cursorSourceChanged(int)));
    grid->addWidget(cursorType, 0, 2, 1, 1);
    grid->addWidget(new QLabel("Cursors"), 0, 0);

    //Create cursor A slider
    cursor1Slider = new QSlider(Qt::Horizontal);
    cursor1Slider->setRange(1, 500);
    cursor1Slider->setPageStep(1);
    connect(cursor1Slider, &QSlider::valueChanged, this, &Cursors::cursor1ValueChanged);
    grid->addWidget(cursor1Slider, 1, 0, 1, 3);

    //Create cursor B slider
    cursor2Slider = new QSlider(Qt::Horizontal);
    cursor2Slider->setRange(1, 500);
    cursor2Slider->setPageStep(1);
    connect(cursor2Slider, &QSlider::valueChanged, this, &Cursors::cursor2ValueChanged);
    grid->addWidget(cursor2Slider, 2, 0, 1, 3);

}




void Cursors::cursorTypeChanged(int index){
    cursortype = index;
    emit cursorsSettingsChanged(cursortype, channel, cursor1Slider->value(), cursor2Slider->value());
}

void Cursors::cursorSourceChanged(int index){
    channel = index;
    emit cursorsSettingsChanged(cursortype, channel, cursor1Slider->value(), cursor2Slider->value());
}


void Cursors::cursor1ValueChanged(){
    emit cursorsSettingsChanged(cursortype, channel, cursor1Slider->value(), cursor2Slider->value());
}
void Cursors::cursor2ValueChanged(){
    emit cursorsSettingsChanged(cursortype, channel, cursor1Slider->value(), cursor2Slider->value());
}















