#ifndef MATH_H
#define MATH_H

#include <QObject>
#include <QtWidgets>
#include <nucleoscope.h>
#include <waveform.h>

/*
 * Class managing each math operation, user input of math operation settings and calculation of math waveforms.
 *
 */




class MathOperation : public QObject
{
    Q_OBJECT

public:
    MathOperation(NucleoScope *scope,int identifier);
    int mathAChannel = 0;
    int mathBChannel = 0;
    int mathOperation = 0;
    int id;
    bool AReady = false;
    bool BReady = false;
    QHBoxLayout *layout;
    NucleoScope *scope;
    WaveForm *waveA;
    WaveForm *waveB;
    void recalculateMath();
    QLineSeries *mathSeries;


private:
    QComboBox *mathOperationSelector;
    QComboBox *channelASelector;
    QComboBox *channelBSelector;


public slots:
    void operationChanged(int);
    void channellAChanged(int);
    void channellBChanged(int);
    void channellAReady();
    void channellBReady();

signals:
    void seriesReady(QLineSeries*, QColor col);





};


/*
 * Class managing display of math operations
 *
 *
 */

class MathMatics : public QObject
{
    Q_OBJECT
public:
    MathMatics(NucleoScope *scope);
    NucleoScope *scope;
    QVBoxLayout *layout;
    MathOperation *op1;
    MathOperation *op2;
    MathOperation *op3;


};






#endif // MATH_H
