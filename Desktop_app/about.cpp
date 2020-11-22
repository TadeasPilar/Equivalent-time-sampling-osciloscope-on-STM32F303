#include "about.h"

About::About()
{
    layout = new QVBoxLayout();
    this->setLayout(layout);
    QLabel *l1 = new QLabel("<b>Czech Technical University in Prague</b>");
    l1->setAlignment(Qt::AlignHCenter);
    layout->addWidget(l1,  Qt::AlignCenter);

    QLabel *l2 = new QLabel("<b>Department of Measurement</b>");
    l2->setAlignment(Qt::AlignHCenter);
    layout->addWidget(l2,  Qt::AlignCenter);

    QLabel *l3 = new QLabel("<b>Tadeáš Pilař</b>");
    l3->setAlignment(Qt::AlignHCenter);
    layout->addWidget(l3,  Qt::AlignCenter);

    QLabel *l4 = new QLabel("<b>2020</b>");
    l4->setAlignment(Qt::AlignHCenter);
    layout->addWidget(l4,  Qt::AlignCenter);


    QLabel *logo = new QLabel("<img src=':/ctu_logo/logo_cvut_en.jpg' width=\"500\" />");
    logo->setAlignment(Qt::AlignHCenter);
    logo->setFixedWidth(500);
    layout->addWidget(logo,  Qt::AlignCenter);

    layout->setSizeConstraint(QLayout::SetFixedSize);

    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    exec();
}
