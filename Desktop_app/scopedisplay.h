#ifndef SCOPEDISPLAY_H
#define SCOPEDISPLAY_H

#include <QGridLayout>
#include <QtWidgets>
#include <QtCharts>
#include <QVector>
#include "waveform.h"
#include <QSlider>
#include <QLineEdit>
#include <QLabel>
#include "nucleoscope.h"
#include <float.h>

#define EDIT_WIDTH 35
#define VOFFSETMAX 5

/*
 * Displays osciloscope graph.
 * Displays graph controls.
 * Calculates viable sampling frequencies.
 */


class ScopeDisplay : public QObject
{
   Q_OBJECT
public:
    ScopeDisplay(NucleoScope* scope);
    QGridLayout* getDisplay();



private:
    NucleoScope *scope;
    QGridLayout *DisplayGrid;
    QChartView *chartView;
    QChart *displayChart;
    QSlider *VZoomSlider;
    QSlider *HZoomSlider;
    QSlider *VOffsetSlider;
    QSlider *HOffsetSlider;

    QLineEdit *VZoomEdit;
    QLineEdit *HZoomEdit;
    QLineEdit *VOffsetEdit;
    QLineEdit *HOffsetEdit;

    QHBoxLayout *selectorLayout;
    QListWidget *dividerList;
    QList<qint32> possibleDividers;
    QList<double> possibleEqPeriod;
    QList<double> possiblePeriodLengths;
    QLineEdit *divEdit;

    QLineSeries *cursor1;
    QLineSeries *cursor2;
    QLineSeries *math;
    QList<QColor> colors;
    int cursorsType = 0;
    int cursorChannel = 1;
    int cursor1Position = 0;
    int cursor2Position = 0;
    QGroupBox *cursorsInfoBox;
    QVBoxLayout *cursorsInfoLayout;
    QLabel *line1;
    QLabel *line2;
    QLabel *line3;
    QLabel *line4;
    QLabel *line5;
    QLabel *line6;



    qint32 VZoom = 33;
    double VOffset = 0;
    qint32 HZoom = 50;
    qint32 HOffset = 0;

    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    QString xAxisUnit = "Samples";
    QString xAxisFrequencyUnit = "Hz";

    bool trigger = true;
    double triggerLevel = 1.5;



    double lastFrequency;
    QStringList computeDividers(int freq);

    QVector<WaveForm> waveList;
    int parseNumber(QString value, int max);
    QStringList histogramFilter(QStringList list);
    void updateAxis();
    void removeCurors();
    void cursorsPlacer();


private slots:
    void VOffsetEditingFinished();
    void VOffsetValueChanged();
    void HOffsetEditingFinished();
    void HOffsetValueChanged();
    void VZoomEditingFinished();
    void VZoomValueChanged();
    void HZoomEditingFinished();
    void HZoomValueChanged();
    void redraw();
    void samplingSelected();
    void divEdited();
    void updateCursors();


public slots:
    void cursorsSettingsChanged(int type, int channel, int cur1, int cur2);
    void autoScale();
    void drawSeries(QLineSeries *series, QColor col);

signals:
    void samplingChanged(int index);
    void memDepthChanged();











};

#endif // SCOPEDISPLAY_H
