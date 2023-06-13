#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //Connector
    connect(ui->OK_pushButton, SIGNAL(clicked()), this, SLOT(onOK_pushButton_clicked()));

}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getGraphQuantity()
{
    return graphQuantity;
}

QString Dialog::getchartType()
{
    return chartType;
}

QString Dialog::getDataSource()
{
    return dataSource;
}

void Dialog::onOK_pushButton_clicked()
{
    chartType = ui->chart_Type_comboBox->currentText();
    dataSource = ui->data_Source_comboBox->currentText();
    graphQuantity = ui->quantity_comboBox->currentText();
    accept();
}

