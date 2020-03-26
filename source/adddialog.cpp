#include "adddialog.h"
#include "ui_adddialog.h"

addDialog::addDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    ui->setupUi(this);
    ui->classComboBox->addItem(QStringLiteral("观众"));
    ui->classComboBox->addItem(QStringLiteral("UP主"));

    QRegExp regex("[0123456789]*");
    auto pattern = new QRegExpValidator(regex,this);
    ui->uidLineEdit->setValidator(pattern);

    setFont(QFont("Microsoft Yahei"));
}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::on_uidLineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
        return;
    }
    ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    emit textChanged(arg1);
}

void addDialog::on_classComboBox_activated(int index)
{
    emit comboBoxActived(index);
}

void addDialog::on_buttonBox_accepted()
{
    emit okBtnClicked(true);
}
