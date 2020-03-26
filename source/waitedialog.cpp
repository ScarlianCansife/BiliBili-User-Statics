#include "waitedialog.h"
#include "ui_waitedialog.h"

WaiteDialog::WaiteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaiteDialog)
{
    ui->setupUi(this);
    QPixmap waitePixmap(":images/waite");
    auto fixedWaitePixmap = waitePixmap.scaled(ui->pixLabel->width(),ui->pixLabel->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->pixLabel->setPixmap(fixedWaitePixmap);

    setWindowFlags(Qt::WindowTitleHint);
    setWindowFlag(Qt::WindowStaysOnTopHint);
}

WaiteDialog::~WaiteDialog()
{
    delete ui;
}

void WaiteDialog::setText(const QString &text)
{
    ui->proccessLabel->setText(text);
    update();
}
