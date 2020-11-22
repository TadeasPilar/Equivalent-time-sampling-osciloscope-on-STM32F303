#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QObject>
#include <QtCharts>

/*
 * Stores waveform data
 * Creates QLineSeries from data
 * Does triggering
 */


class WaveForm : public QObject
{
    Q_OBJECT
public:
    WaveForm();
    WaveForm(QString label);
    WaveForm(QString label, uint8_t dataArray[], qint32 length, int resolution);
    void setZoom( qint32 displayStart, qint32 displayWidth, qint32 displayHeight, qint32 displayOffset);
    QtCharts::QLineSeries* getSeries();
    int getResolution();
    qint32 getDataLength();
    QString toString();
    void update(uint8_t dataArray[], double eqivalentSamplingPeriod, bool displayTime);
    void update(uint8_t dataArray[]);
    void trigger(double level);
    void trigger(int triggerIndex);
    void computeTimestamps(int trigger);


    QString label;
    WaveForm dataToWaveForm(QString dataString);
    QList<double> timeVector;
    QtCharts::QLineSeries *series;
    qint32 displayStart;
    qint32 displayWidth;
    qint32 displayHeight;
    qint32 displayOffset;
    double data[12000];
    double timeStamps[12000];
    int resolution;
    qint32 dataLength = 128;
    double Vcc = 3.3;
    qint32 frequency = 0;
    double min = 100;
    double max = -100;
    int triggerPoint = 0;
    double eqivalentSamplingPeriod;
    int timeDivider = 1;
    bool displayTime = false;
    int displayStyle = 0;


Q_SIGNALS:
    void triggered();






};

#endif // WAVEFORM_H
