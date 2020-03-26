#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include <QWidget>

namespace Ui {
class errorWidget;
}

class errorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit errorWidget(const QString& errorText_,QWidget *parent = nullptr);
    ~errorWidget();

private:
    Ui::errorWidget *ui;

    QString errorText;
};

#endif // ERRORWIDGET_H
