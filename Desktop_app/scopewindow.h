#ifndef SCOPEWINDOW_H
#define SCOPEWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QtWidgets>
#include "scopedisplay.h"
#include "nucleoscope.h"
#include <QSerialPort>
#include "serialselector.h"
#include <QDebug>
#include "generator.h"
#include "infobar.h"
#include "settings.h"
#include "cursors.h"
#include "mathmatics.h"
#include "about.h"
#include "help.h"

/*
 * Main window of this aplication.
 * Initializes and holds all other clases.
 * Creates layout and fills it with all the GUI stuff
 * Also does data export
 */


QT_BEGIN_NAMESPACE
namespace Ui { class ScopeWindow; }
QT_END_NAMESPACE

class ScopeWindow : public QMainWindow
{
    Q_OBJECT

public:
    ScopeWindow(QWidget *parent = nullptr);
    ~ScopeWindow();
    ScopeDisplay *display;
    NucleoScope *nucleoScope;
    QSerialPort *serialPort = nullptr;
    Generator *generator;
    Cursors *cursors;
    MathMatics *mathmatics;



private:
    Ui::ScopeWindow *ui;
    QGridLayout *MainGrid;
    QGroupBox *ScopeBox;
    QGroupBox *InfoBox;
    QGroupBox *CursorBox;
    QGroupBox *MathBox;
    QGroupBox *GeneratorBox;
    QGroupBox *SettingsBox;


    QSlider *GeneratorSlider;
    QLineEdit *GeneratoEditEdit;
    qint32 generatorValue;
    InfoBar *info;
    Settings *settings;



    void CreateMenus();
    void CreateGraph();
    void CreateCursors();
    void CreateMath();
    void CreateGenerator();
    void CreateInfo();
    void CreateGrid();
    void CreateSettings();

    QString getPath();

private slots:
    void getSerialPort();
    void setSerialPortCallback();
    void disconnect();
    void openAboutPage();
    void openHelpPage();
    void exportCSV();
    void exportPNG();


};
#endif // SCOPEWINDOW_H
