#include "detailinfowidget.h"
#include "ui_detailinfowidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

DetailInfoWidget::DetailInfoWidget(const QList<QString>& avNumberList_,const QList<QString>& keywordList_,const QList<QString>& descripeList_,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailInfoWidget),
    avNumberList(avNumberList_),
    keywordList(keywordList_),
    descripeList(descripeList_)
{
    ui->setupUi(this);

    for (auto avNumber : avNumberList)
    {
        ui->listWidget->addItem(avNumber);
    }
    ui->listWidget->setFont(QFont("Microsoft Yahei"));

    auto count = (keywordList.length() < descripeList.length())?keywordList.length():descripeList.length();
    for (int index = 0;index != count;++index)
    {
        ui->stackedWidget->addWidget(createWidget(keywordList[index],descripeList[index]));
    }
}

DetailInfoWidget::~DetailInfoWidget()
{
    delete ui;
}

QWidget *DetailInfoWidget::createWidget(const QString &keyword, const QString &descripe)
{
    QFont font("Microsoft Yahei");

    auto widget = new QWidget;
    auto widgetLayout = new QVBoxLayout(widget);

    auto keywordLabel = new QLabel;
    keywordLabel->setText(QStringLiteral("标题&关键词："));
    keywordLabel->setFont(font);
    auto descripeLabel = new QLabel;
    descripeLabel->setText(QStringLiteral("简介："));
    descripeLabel->setFont(font);

    auto keywordTextEdit = new QTextEdit;
    keywordTextEdit->setText(keyword);
    keywordTextEdit->setFont(font);
    auto descripeTextEdit = new QTextEdit;
    descripeTextEdit->setText(descripe);
    descripeTextEdit->setFont(font);

    widgetLayout->addWidget(keywordLabel);
    widgetLayout->addWidget(keywordTextEdit);
    widgetLayout->addWidget(descripeLabel);
    widgetLayout->addWidget(descripeTextEdit);

    return widget;
}

void DetailInfoWidget::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}
