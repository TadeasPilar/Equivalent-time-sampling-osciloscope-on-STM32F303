#ifndef INFOBAR_H
#define INFOBAR_H

#include <QObject>
#include <QLayout>
#include <QtWidgets>
#include "nucleoscope.h"

/*
 * Class managing info bar and its updates.
 *
 *
 *
 *
 */



class InfoBarItem
{
public:
    InfoBarItem();
    InfoBarItem(QString label);
    void setColor(QColor color);
    void setValue(QString value);
    void setValue(double value, QString unit);
    QWidget *getWidget();
    QString unit = "";



private:
    QVBoxLayout *layout;
    QWidget *box;
    QLabel *labelWidget;
    QLabel *valueWidget;

};





class InfoBar : public QObject
{
   Q_OBJECT

public:
    InfoBar(NucleoScope *scope);
    NucleoScope *scope;
    InfoBarItem *port;
    InfoBarItem *signalFrequency;
    InfoBarItem *samplingFrequency;
    InfoBarItem *Vpp;
    InfoBarItem *equivalentSamplingFrequency;
    InfoBarItem *sampling;
    QHBoxLayout *layout;
    QElapsedTimer timer;
    QTimer *updateTimer;
    InfoBarItem *updateRate;




public slots:
    void update();
    void updateSerialState();








};









#endif // INFOBAR_H
