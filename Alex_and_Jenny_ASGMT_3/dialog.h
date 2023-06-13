#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

/*
Name: Jenny Itza & Alex Guerra
Date: 3/25/2022
Course: CMPS2212 - GUI Programming
Project Description: Adding Notepad new features
*/

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QString getGraphQuantity();    // function that gets the value of the data member graphQuantity
    QString getchartType();        // function that gets the value of the data member chartType
    QString getDataSource();       // function that gets the value of the data member dataSource
private slots:
    void onOK_pushButton_clicked();

private:
    Ui::Dialog *ui;
    QString graphQuantity;       //Saves user's graph quantity option
    QString chartType;           //Saves user's chart type option
    QString dataSource;         //Saves user's data source option

};

#endif // DIALOG_H
