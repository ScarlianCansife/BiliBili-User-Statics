#ifndef STATISTICWIDGET_H
#define STATISTICWIDGET_H

#include "enum.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QChart>
#include <QtCharts>
#include <QChartView>

namespace Ui {
class statisticWidget;
}

class statisticWidget : public QWidget
{
    Q_OBJECT

public:
    explicit statisticWidget(const QString& uid_,const QString& name_,const QString& headPixmap_,Class userClass_,const QMap<QString,int> tags_,QWidget *parent = nullptr);
    ~statisticWidget();

signals:
    void detailLabelClicked();

private slots:
    void on_detailLabel_linkActivated(const QString &link);

private:
    Ui::statisticWidget *ui;

    QString uid;
    QString name;
    QString headPixmap;
    Class userClass;
    QMap<QString,int> tags;

    void createCharts();
};

#endif // STATISTICWIDGET_H
