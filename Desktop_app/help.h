#ifndef HELP_H
#define HELP_H


#include <QDialog>
#include <QtWidgets>
#include <QVBoxLayout>

/*
 * Dialog displaying the help page
 *
 */

class Help : public QDialog
{
    Q_OBJECT

public:
    Help();
    QVBoxLayout *layout;
    QString readFile(QString file);

};

#endif // HELP_H
