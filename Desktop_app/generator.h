#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QGridLayout>
#include <QLayout>
#include <QtWidgets>
#include <QSlider>
#include <QLineEdit>
#include <QLabel>
#include <QList>
#include "nucleoscope.h"

#define TIMER8_FREQUENCY 144000000

/*
 * Class managing user input of generator frequency
 *
 *
 */


class Generator : public QObject
{
    Q_OBJECT
public:
    Generator(NucleoScope *scope);
    QSlider *GeneratorSlider;
    QLineEdit *GeneratorEdit;
    QVBoxLayout *layout;
    qint32 div;
    bool active;

private:
    QList<float> *frequencies;
    NucleoScope *scope;




public slots:
    void editingFinished();
    void valueChanged();










signals:

};

#endif // GENERATOR_H
