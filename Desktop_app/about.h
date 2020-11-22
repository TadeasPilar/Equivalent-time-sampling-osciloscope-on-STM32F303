#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QtWidgets>
#include <QVBoxLayout>

/*
 *Dialog displaying the about page
 *
 */


class About : public QDialog
{
    Q_OBJECT

public:
    About();
    QVBoxLayout *layout;
};

#endif // ABOUT_H
