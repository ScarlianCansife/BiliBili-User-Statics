#ifndef DETAILINFOWIDGET_H
#define DETAILINFOWIDGET_H

#include <QWidget>

namespace Ui {
class DetailInfoWidget;
}

class DetailInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailInfoWidget(const QList<QString>& avNumberList_,const QList<QString>& keywordList_,const QList<QString>& descripeList_,QWidget *parent = nullptr);
    ~DetailInfoWidget();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::DetailInfoWidget *ui;

    QList<QString> avNumberList;
    QList<QString> keywordList;
    QList<QString> descripeList;

    QWidget* createWidget(const QString& keyword,const QString& descripe);
};

#endif // DETAILINFOWIDGET_H
