#ifndef CURSORS_H
#define CURSORS_H

#include <QObject>
#include <QGridLayout>
#include <QtWidgets>
#include "nucleoscope.h"


/*
 *  Class managing user input of curosrs settings
 *
 *
 */


class Cursors : public QObject
{
    Q_OBJECT
public:
    Cursors(NucleoScope *scope);
    NucleoScope *scope;
    QGridLayout *grid;
    QComboBox *cursorType;
    QComboBox *cursorSource;
    QLineEdit *cursor1Edit;
    QLineEdit *cursor2Edit;
    QSlider *cursor1Slider;
    QSlider *cursor2Slider;

    int cursor1Value;
    int cursor2Value;
    int cursortype=0;
    int channel=0;




public slots:
    void cursorTypeChanged(int);
    void cursorSourceChanged(int);
    void cursor1ValueChanged();
    void cursor2ValueChanged();
    //void cursor1EditingFinished();
    //void cursor2EditingFinished();

signals:
    void cursorsSettingsChanged(int type, int channel, int cur1, int cur2);


};

#endif // CURSORS_H
