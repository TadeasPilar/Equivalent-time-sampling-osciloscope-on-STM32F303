#include "scopewindow.h"
#include "ui_scopewindow.h"
ScopeWindow::ScopeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScopeWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Sampling Scope"));
    nucleoScope = new NucleoScope(this);
    CreateMenus();
    CreateGrid();
    CreateGraph();
    CreateCursors();
    CreateMath();
    CreateGenerator();
    CreateInfo();
    CreateSettings();


    QIcon *ico = new QIcon("icon_orig.png");
    this->setWindowIcon(*ico);






}

void ScopeWindow::CreateGraph(){
    display = new ScopeDisplay(nucleoScope);
    MainGrid->addLayout(display->getDisplay(), 1, 0);

}

void ScopeWindow::CreateCursors(){
    cursors = new Cursors(nucleoScope);
    CursorBox->setLayout(cursors->grid);
    connect(cursors, SIGNAL(cursorsSettingsChanged(int, int, int, int)), display,SLOT(cursorsSettingsChanged(int, int, int, int)));

}

void ScopeWindow::CreateMath(){
    mathmatics = new MathMatics(nucleoScope);
    MathBox->setLayout(mathmatics->layout);
    connect(mathmatics->op1, SIGNAL(seriesReady(QLineSeries*, QColor)), display, SLOT(drawSeries(QLineSeries*, QColor)));
    connect(mathmatics->op2, SIGNAL(seriesReady(QLineSeries*, QColor)), display, SLOT(drawSeries(QLineSeries*, QColor)));
    connect(mathmatics->op3, SIGNAL(seriesReady(QLineSeries*, QColor)), display, SLOT(drawSeries(QLineSeries*, QColor)));


}

void ScopeWindow::CreateGenerator(){
    generator = new Generator(nucleoScope);
    GeneratorBox->setLayout(generator->layout);



}

void ScopeWindow::CreateInfo(){
    //TODO: Draw info bar
    /*Create info area*/
    InfoBox = new QGroupBox("Info");
    MainGrid->addWidget(InfoBox, 0, 0, 1, 2);
    MainGrid->setRowMinimumHeight(0, 50);
    MainGrid->setRowStretch(0, 0);
    info = new InfoBar(nucleoScope);
    InfoBox->setLayout(info->layout);
    InfoBox->setContentsMargins(0,17,0,0);



}

void ScopeWindow::CreateSettings(){

    settings = new Settings(nucleoScope);
    SettingsBox->setLayout(settings->grid);
    connect(display, SIGNAL(samplingChanged(int)), settings,SLOT(equivalentTimeBoxChanged(int)));
    connect(nucleoScope, &NucleoScope::autosetMeasureDone, display, &ScopeDisplay::autoScale);
    connect(display, &ScopeDisplay::memDepthChanged, settings, &Settings::memDepthChanged);


}

void ScopeWindow::CreateGrid(){

    QWidget *mainWidget = new QWidget();
    MainGrid = new QGridLayout(mainWidget);
    this->setCentralWidget(mainWidget);
    mainWidget->setLayout(MainGrid);

    /* Create scope screen area*/
    ScopeBox = new QGroupBox("Scope");
    MainGrid->addWidget(ScopeBox, 1,0);
    MainGrid->setRowStretch(1, 100);
    MainGrid->setColumnStretch(0, 100);



    /*Create tools layout and box*/
    QVBoxLayout *ToolsLayout = new QVBoxLayout();
    CursorBox = new QGroupBox("Cursors");
    MathBox = new QGroupBox("Math");
    GeneratorBox = new QGroupBox("Generator");
    SettingsBox = new QGroupBox("Settings");
    ToolsLayout->addWidget(SettingsBox);
    ToolsLayout->addWidget(CursorBox);
    ToolsLayout->addWidget(MathBox);
    ToolsLayout->addWidget(GeneratorBox);

    MainGrid->addLayout(ToolsLayout, 1,1);
    MainGrid->setColumnStretch(1, 0);
    MainGrid->setColumnMinimumWidth(1, 300);
    MainGrid->setRowMinimumHeight(1, 500);

    //TODO: Start/Stop button and channels




}


void ScopeWindow::CreateMenus(){
    //TODO: Add meaningful menus

    QMenu *fileMenu = menuBar()->addMenu("Connect");   //create and add menu

    //connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);  //connect item press function to function
    QAction *connectAct = new QAction("Connect", this);
    fileMenu->addAction(connectAct);
    connect(connectAct, &QAction::triggered, this, &ScopeWindow::getSerialPort);
    QAction *disconnectAct = new QAction("Disconnect", this);
    fileMenu->addAction(disconnectAct);
    connect(disconnectAct, &QAction::triggered, this, &ScopeWindow::disconnect);

    QMenu *saveMenu = menuBar()->addMenu("Save");
    QAction *saveCSV = new QAction("Save as CSV", this);
    saveMenu->addAction(saveCSV);
    connect(saveCSV, &QAction::triggered, this, &ScopeWindow::exportCSV);

    QAction *savePNG = new QAction("Save as PNG", this);
    saveMenu->addAction(savePNG);
    connect(savePNG, &QAction::triggered, this, &ScopeWindow::exportPNG);

    QMenu *helpMenu = menuBar()->addMenu("Help");
    QAction *help = new QAction("Help", this);
    helpMenu->addAction(help);
    connect(help, &QAction::triggered, this, &ScopeWindow::openHelpPage);

    QAction *about = new QAction("About", this);
    helpMenu->addAction(about);
    connect(about, &QAction::triggered, this, &ScopeWindow::openAboutPage);


}


void ScopeWindow::exportCSV(){
    if(!nucleoScope->stopped){
        settings->startStopButtonPressed();
    }
    QString path = getPath();

    if(path.length()>0){
        if(!path.contains(".csv", Qt::CaseInsensitive)){
            path = path + ".csv";
        }
        QLocale locale = QLocale::system();
        //locale = QLocale::C;
        QString separator = ";";
         QString str = "";
        if(nucleoScope->settings->channel1On){
            str = str + "CH1" + separator;
            for(int i = 0;i < nucleoScope->ch1WaveForm->dataLength;i++){
                str = str + locale.toString(nucleoScope->ch1WaveForm->data[i]) + separator;
            }
            str = str + '\n';
        }
        if(nucleoScope->settings->channel2On){
            str = str + "CH2,";
            for(int i = 0;i < nucleoScope->ch2WaveForm->dataLength;i++){
                str = str + locale.toString(nucleoScope->ch2WaveForm->data[i]) + separator;
            }
            str = str + '\n';
        }
        if(nucleoScope->settings->channel3On){
            str = str + "CH3,";
            for(int i = 0;i < nucleoScope->ch3WaveForm->dataLength;i++){
                str = str + locale.toString(nucleoScope->ch3WaveForm->data[i]) + separator;
            }
            str = str + '\n';
        }
        if(nucleoScope->settings->channel4On){
            str = str + "CH4,";
            for(int i = 0;i < nucleoScope->ch4WaveForm->dataLength;i++){
                str = str + locale.toString(nucleoScope->ch4WaveForm->data[i]) + separator;
            }
            str = str + '\n';
        }

        QFile file(path);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream stream(&file);
            stream << str;
            file.close();
            qDebug() << "Save csv";
        }
    }
}

void ScopeWindow::exportPNG(){
    if(!nucleoScope->stopped){
        settings->startStopButtonPressed();
    }
    QString path = getPath();

    if(path.length()>0){
        if(!path.contains(".png", Qt::CaseInsensitive)){
            path = path + ".png";
        }
        auto active_window = this;
        QPixmap pixmap(active_window->size());
        active_window->render(&pixmap);
        qDebug() << "Saving screenshot " << pixmap.save(path, 0, 100);
    }

}

QString ScopeWindow::getPath(){

    QString fileName = QFileDialog::getSaveFileName(this,tr("Save PNG"), "", tr("All Files (*)"), Q_NULLPTR, QFileDialog::DontConfirmOverwrite);

    qDebug() << "Selected " << fileName;
    int action = QMessageBox::Yes;
    if(QFile::exists(fileName)){
        QMessageBox msg;
        msg.setText("Overwrite existing file?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Yes);
        action = msg.exec();
    }

    if(action == QMessageBox::Yes){
        return fileName;
    }else if(action == QMessageBox::No){
        return getPath();
    }

    return "";
}

void ScopeWindow::openAboutPage(){
    new About();
}

void ScopeWindow::openHelpPage(){
    new Help();
}



void ScopeWindow::disconnect(){
    if(serialPort->isOpen()){
        serialPort->close();
    }


}

//Open serial selector window and connect to the selected port
void ScopeWindow::getSerialPort(){
    SerialSelector *selector = new SerialSelector();
    connect(selector, &SerialSelector::serialReady, this, &ScopeWindow::setSerialPortCallback);
    selector->getSerial(serialPort);

}

//I don't remember what this does, but it is probably important.
void ScopeWindow::setSerialPortCallback(){
    qDebug() << "Setting serial port \n";
    QObject *sender = QObject::sender();
    SerialSelector* selector = reinterpret_cast<SerialSelector*>(sender);
    serialPort = selector->port;

    qDebug() << "Selected port is: " << serialPort->portName() << "\n";
    nucleoScope->ConnectToBoard(serialPort);



}

//Delete this upon window close
ScopeWindow::~ScopeWindow()
{
    delete ui;
}





