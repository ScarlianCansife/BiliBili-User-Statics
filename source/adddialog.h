#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QWidget *parent = nullptr);
    ~addDialog();

signals:
    void textChanged(const QString& uid_);
    void comboBoxActived(int argv);
    void okBtnClicked(bool ok);

private slots:
    void on_buttonBox_accepted();

    void on_uidLineEdit_textChanged(const QString &arg1);

    void on_classComboBox_activated(int index);

private:
    Ui::addDialog *ui;
};

#endif // ADDDIALOG_H
