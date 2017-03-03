#include "okfailmessagewindow.h"
#include "ui_okfailmessagewindow.h"

OkFailMessageWindow::OkFailMessageWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OkFailMessageWindow)
{
    ui->setupUi(this);
}

OkFailMessageWindow::~OkFailMessageWindow()
{
    delete ui;
}

void OkFailMessageWindow::on_okButton_2_clicked()
{
    this->close();
}
