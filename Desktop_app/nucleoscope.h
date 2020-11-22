#ifndef NUCLEOSCOPE_H
#define NUCLEOSCOPE_H

#include <QtGlobal>
#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include "waveform.h"
#include <QList>

#define BASE_FREQUENCY 72000000

/*
 * Holds setting of osciloscope. Parse settings from string.
 */


class ScopeSettings{

public:
    ScopeSettings();
    ScopeSettings(QString settingsString);
    qint16 samplingDivider = 511;
    int generatorDivider = 63;
    qint16 compVoltage = 2048;
    qint16 compHysteresis = 1;
    bool channel1On = 1;
    bool channel2On = 0;
    bool channel3On = 0;
    bool channel4On = 0;
    bool channel4Vref = 0;
    qint16 memDepth = 500;
    qint8 bits = 8;
    qint32 frequency;
    int vrefCal;

    bool parseSettings(QString settings);
    bool equals(ScopeSettings settings);
    QString toString();
    QString toHumanString();
    int chanelCount();


};

/*
 * Manages connection and comunication with osciloscope. Holds waveforms.
 * Also serves as connection between most other clases to exchange information.
 */



class NucleoScope : public QObject
{
    Q_OBJECT

public:
    NucleoScope(QObject *mainWindow);
    void ConnectToBoard(QSerialPort *port);
    void SendData();

    void autosetProcedure();
    int autosetState = 0;
    double autosetMin = 1000;
    double autosetMax = -1000;



public slots:
    void readSerial();
    void writeSettings();
    void cancelAutoset();

Q_SIGNALS:
    void dataReady();
    void autosetMeasureDone();

public:
    double ch1Data[48000];
    double ch2Data[48000];
    double ch3Data[48000];
    double ch4Data[48000];
    bool ch4Vref;

    WaveForm *ch1WaveForm;
    WaveForm *ch2WaveForm;
    WaveForm *ch3WaveForm;
    WaveForm *ch4WaveForm;
    ScopeSettings *settings;
    ScopeSettings *appSettings;


    double generatorFrequency;
    double equivalentSamplingPeriod = 0;
    bool displayTime = false;
    bool stopped = false;
    QProgressDialog *autosetInfo;
    const qint32 baseFrequency = BASE_FREQUENCY;
    QSerialPort *serialPort;


private:
    QElapsedTimer timer;
    QObject *pearent;

    QByteArray serialData;
    QString settingsString;
    char dataArray[48000];
    int parsingStage = 0;

    int dataPointer = 0;
    qint32 dataSize;
    bool connected;
    int oldMemDepth = 500;
    int oldSamplingDivider;



    void createWaveForms();
    void parse8bitData(QStringList *settings, QByteArray *dataArray);




};








#endif // NUCLEOSCOPE_H
