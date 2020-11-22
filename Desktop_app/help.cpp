#include "help.h"


Help::Help()
{



   QString helpCode = readFile(":/help/HELP.htm");  //Load help from HTML file




   QLabel *imageLabel = new QLabel(helpCode);       //Display HTML file as label inside scroll area
   imageLabel->setBackgroundRole(QPalette::Base);
   imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

   QScrollArea *scrollArea = new QScrollArea();
   scrollArea->setWidget(imageLabel);


   QHBoxLayout *layout = new QHBoxLayout(this);
   this->setLayout(layout);
   layout->addWidget(scrollArea);
   this->setFixedWidth(870);

   this->setWindowTitle("Help");

   this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


   show();      //Show help window but allow main window to continue operate
}



QString Help::readFile(QString file){       //Read file to string


    QFile txt(file);

    if(!txt.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << txt.error();
        return "";
    }

    QString text = "";
    QTextStream instream(&txt);
    text = instream.readAll();

    txt.close();
    return text;


}






