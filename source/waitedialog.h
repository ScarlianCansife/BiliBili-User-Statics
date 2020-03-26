#ifndef WAITEDIALOG_H
#define WAITEDIALOG_H

#include <QDialog>

namespace Ui {
class WaiteDialog;
}

class WaiteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaiteDialog(QWidget *parent = nullptr);
    ~WaiteDialog();

    void setText(const QString& text);

private:
    Ui::WaiteDialog *ui;
};

#endif // WAITEDIALOG_H
