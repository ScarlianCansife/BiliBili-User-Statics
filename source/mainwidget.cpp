#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QIcon>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    firstNew(true)
{
    ui->setupUi(this);

    QPixmap pixmap2233(":images/2233");
    auto fixedPixmap2233 = pixmap2233.scaled(ui->pixLabel->width(),ui->pixLabel->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->pixLabel->setPixmap(fixedPixmap2233);

    auto mainPalette = palette();
    mainPalette.setColor(QPalette::Background,Qt::cyan);
    setPalette(mainPalette);

    QPixmap pixmapAdd(":icons/add");
    auto fixedPixmapAdd = pixmapAdd.scaled(ui->addButton->width(),ui->addButton->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->addButton->setIcon(QIcon(fixedPixmapAdd));
    ui->addButton->setStyleSheet("QToolButton{border:0px;}");
    ui->addButton->setIconSize(QSize(ui->addButton->width(),ui->addButton->height()));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::newUser(const QString &uid, Class userClass)
{
    if (firstNew)
    {
        user = new biliUser(uid,userClass,this);
        firstNew = false;
    }
    else
    {
        user->reLoad(uid,userClass);
    }
}

void MainWidget::showAddDialog()
{
    auto AddDialog = new addDialog(this);
    AddDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(AddDialog,&addDialog::textChanged,[this](const QString& uid_) {currentUID = uid_;});
    connect(AddDialog,&addDialog::comboBoxActived,[this](int index) {currentUserClass = (index == 0)?Class::Audience:Class::UP;});
    connect(AddDialog,&addDialog::okBtnClicked,[this](bool ok)
    {if (ok)
        {
            newUser(currentUID,currentUserClass);
        }
    });
    AddDialog->exec();
}

void MainWidget::clearTempFiles()
{
    QString path = qApp->applicationDirPath() + "/temp";
    QDir Dir(path);
    if(Dir.isEmpty())
    {
        return;
    }
    QDirIterator DirsIterator(path, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while(DirsIterator.hasNext())
    {
        if (!Dir.remove(DirsIterator.next()))
        {
            QDir(DirsIterator.filePath()).removeRecursively();
        }
    }
}

void MainWidget::on_addButton_triggered(QAction *arg1)
{
    showAddDialog();
}

void MainWidget::on_addButton_clicked()
{
    showAddDialog();
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    clearTempFiles();
    event->accept();
}
