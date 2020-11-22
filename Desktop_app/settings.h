#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "nucleoscope.h"
#include <QtWidgets>
#include <QLayout>
#include <QMessageBox>

/*
 * Manages settings box
 */


class Settings : public QObject
{
   Q_OBJECT

public:
    Settings(NucleoScope *scope);
    NucleoScope *scope;
    QGridLayout *grid;
    QPushButton *ch1Button;
    QPushButton *ch2Button;
    QPushButton *ch3Button;
    QPushButton *ch4Button;
    QLineEdit *memDepth;
    QPushButton *autoset;
    QPushButton *autoscale;
    QPushButton *startStop;
    QPushButton *single;
    QComboBox *equivalentTimeBox;
    QComboBox *displayStyleBox;

public slots:
    void ch1ButtonPressed();
    void ch2ButtonPressed();
    void ch3ButtonPressed();
    void ch4ButtonPressed();
    void memDepthEdited();
    void memDepthChanged();
    void autoSetButtonPressed();
    void autoScaleButtonPressed();
    void startStopButtonPressed();
    void equivalentTimeBoxChanged(int index);
    void displayStyleBoxChanged(int index);
    void singleButtonPressed();


private:
    QString bgCol(QColor col);



};

#endif // SETTINGS_H
