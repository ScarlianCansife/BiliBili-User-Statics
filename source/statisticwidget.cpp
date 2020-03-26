#include "statisticwidget.h"
#include "ui_statisticwidget.h"


statisticWidget::statisticWidget(const QString& uid_,const QString& name_,const QString& headPixmap_,Class userClass_,const QMap<QString,int> tags_,QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::statisticWidget)
    ,uid(uid_)
    ,name(name_)
    ,headPixmap(headPixmap_)
    ,userClass(userClass_)
    ,tags(tags_)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QString headPixmapPath = qApp->applicationDirPath() + "/temp/" + uid + ((userClass == Class::UP)?"Video_files":"Favlist_files") + "/" + headPixmap;
    ui->headPixLabel->setPixmap(QPixmap(headPixmapPath));

    ui->nameLabel->setText(name);
    ui->uidLabel->setText("UID: " + uid);
    if (userClass == Class::UP)
    {
        ui->classLabel->setText(QStringLiteral("UP主"));
        ui->typeLabel->setText(QStringLiteral("投稿了"));
    }
    else
    {
        ui->classLabel->setText(QStringLiteral("观众"));
        ui->typeLabel->setText(QStringLiteral("收藏了"));
    }

    int count = 0;
    int maxCount = 0;
    QString maxTag;
    auto iter = tags.cbegin();
    while (iter != tags.cend())
    {
        count += iter.value();
        if (iter.value() > maxCount)
        {
            maxCount = iter.value();
            maxTag = iter.key();
        }
        ++iter;
    }

    ui->countLabel->setText(QString::number(count));
    ui->tagLabel->setText(maxTag);

    createCharts();
}

statisticWidget::~statisticWidget()
{
    delete ui;
}

void statisticWidget::createCharts()
{
    //Initialize Statistics
    QStringList tagsLabel;
    QList<int> tagsCtn;
    int maxTagCtn = 0;
    auto iter = tags.cbegin();
    while (iter != tags.cend())
    {
        if (iter.value() > 0)
        {
            tagsLabel.append(iter.key());
            tagsCtn.append(iter.value());
            if (iter.value() > maxTagCtn)
                maxTagCtn = iter.value();
        }
        ++iter;
    }

    //Bar Chart
    auto barSet = new QBarSet(QStringLiteral("视频数目"));
    for (auto tagCtn : tagsCtn)
        *barSet << tagCtn;
    auto barSeries = new QBarSeries;
    barSeries->append(barSet);

    auto axisX = new QBarCategoryAxis;
    for (auto tagLabel : tagsLabel)
        axisX->append(tagLabel);
    auto axisY = new QValueAxis;
    axisY->setRange(0,maxTagCtn);

    auto barChart = new QChart;
    barChart->addSeries(barSeries);
    barChart->addAxis(axisX,Qt::AlignBottom);
    barChart->addAxis(axisY,Qt::AlignLeft);
    barChart->axes()[0]->setLabelsFont(QFont("Microsoft Yahei",5));
    barChart->axes()[1]->setLabelsFont(QFont("Microsoft Yahei",5));
    barChart->setTheme(QChart::ChartThemeBlueCerulean);
    barChart->setAnimationOptions(QChart::AllAnimations);
    barChart->setDropShadowEnabled(true);
    barChart->legend()->setVisible(false);

    auto barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    ui->chartLayout->addWidget(barChartView);

    //Pie Chart
    auto pieSeries = new QPieSeries;
    iter = tags.cbegin();
    while (iter != tags.cend())
    {
        if (iter.value() > 0)
            pieSeries->append(iter.key(),iter.value());
        ++iter;
    }
    pieSeries->setVisible(true);
    pieSeries->setUseOpenGL(true);

    auto pieChart = new QChart;
    pieChart->addSeries(pieSeries);
    pieChart->setTheme(QChart::ChartThemeBlueCerulean);
    pieChart->setDropShadowEnabled(true);
    pieChart->setAnimationOptions(QChart::AllAnimations);
    auto legend = pieChart->legend();
    legend->setVisible(true);
    legend->setAutoFillBackground(false);
    legend->setAlignment(Qt::AlignLeft);
    legend->setFont(QFont("Microsoft Yahei",9));

    auto pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);

    ui->chartLayout->addWidget(pieChartView);
}

void statisticWidget::on_detailLabel_linkActivated(const QString &link)
{
    emit detailLabelClicked();
}
