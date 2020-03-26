#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "biliuser.h"
#include "enum.h"
#include "adddialog.h"

#include <QWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_addButton_triggered(QAction *arg1);

    void on_addButton_clicked();

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWidget *ui;

    QString currentUID;
    Class currentUserClass;
    biliUser* user;
    bool firstNew;

    void newUser(const QString& uid,Class userClass);
    void showAddDialog();
    void clearTempFiles();
};

#endif // MAINWIDGET_H
