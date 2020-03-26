#include "errorwidget.h"
#include "ui_errorwidget.h"

errorWidget::errorWidget(const QString& errorText_,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::errorWidget),
    errorText(errorText_)
{
    ui->setupUi(this);
    ui->textLabel->setText(errorText);
    ui->pixLabel->setPixmap(QPixmap(":images/error"));
}

errorWidget::~errorWidget()
{
    delete ui;
}
