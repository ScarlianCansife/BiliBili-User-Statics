#include "biliuser.h"

#include <QApplication>
#include <QEventLoop>
#include <QWebEngineProfile>
#include <QNetworkProxyFactory>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <windows.h>

biliUser::biliUser(const QString& uid_, Class userClass_, QObject *parent)
    :QObject(parent),uid(uid_),userClass(userClass_)
{
    showWaiteDialog();

    browser = new QWebEngineView;
    page = browser->page();
    connect(page,&QWebEnginePage::loadFinished,this,&biliUser::leadDownload);
    QNetworkProxyFactory::setUseSystemConfiguration(false);

    connect(this,&biliUser::downloadHomePageFinished,this,&biliUser::excute_getAvNumbers);
    connect(this,&biliUser::downloadVideoPageFinished,this,&biliUser::handleAvNumbers);
    connect(this,&biliUser::allAvNumbersHandled,this,&biliUser::excute_getTags);
    connect(this,&biliUser::readTagsFinished,this,&biliUser::showStatisticsWidget);

    writeUID();
    getHomePage();
}

biliUser::~biliUser()
{
    delete browser;
}

void biliUser::reLoad(const QString &uid_, Class userClass_)
{
    uid = uid_;
    userClass = userClass_;
    avNumberList.clear();
    tags.clear();
    keywordList.clear();
    descripeList.clear();

    showWaiteDialog();

    writeUID();
    getHomePage();
}

void biliUser::acceptDownload(QWebEngineDownloadItem *downloadItem, biliUser::HTMLtype type, const QString& avNumber)
{
    QString fileName;
    switch (type)
    {
    case HTMLtype::Vedio:
        if (currentDownload != uid + "Video")
            return;
        fileName = uid + "Video";
        currentDownload = "";
        break;
    case HTMLtype::Favlist:
        if (currentDownload != uid + "Favlist")
            return;
        fileName = uid + "Favlist";
        currentDownload = "";
        break;
    case HTMLtype::Play:
        if (avNumber == "")
            throw std::runtime_error("acceptDownload: empty avNumber");
        if (currentDownload != avNumber)
            return;
        fileName = avNumber;
        currentDownload = "";
        break;
    default:
        return;
        break;
    }
    if (fileName == "")
        throw std::runtime_error("acceptDownload: empty fileName");

    downloadItem->setSavePageFormat(QWebEngineDownloadItem::CompleteHtmlSaveFormat);
    downloadItem->setDownloadDirectory(qApp->applicationDirPath() + "/temp");
    downloadItem->setDownloadFileName(fileName);
    connect(downloadItem,&QWebEngineDownloadItem::downloadProgress,[this](qint64 current,qint64 total) {showDownloadProcess(current,total);});
    connect(downloadItem,&QWebEngineDownloadItem::finished,[this,type]()
                        {
        qDebug()<<"download finished";
        if (type == HTMLtype::Vedio || type == HTMLtype::Favlist)
            emit downloadHomePageFinished();
        else
            emit downloadVideoPageFinished();
    });
    qDebug()<<"download start: fileName: "<<fileName;
    downloadItem->accept();
}

void biliUser::showDownloadProcess(qint64 byteRecieved, qint64 byteTotal)
{
    qDebug()<<"download process: "<<byteRecieved<<" in "<<byteTotal;
}

void biliUser::excute_getAvNumbers()
{
    if (!avNumberList.isEmpty())
        return;

    QString exeName = (userClass == Class::UP)?"findLinkUP.exe":"findLinkAudience.exe";
    QString path_qs = "\"" + qApp->applicationDirPath() + "/" + exeName + "\"";
    QByteArray path_ba;
    path_ba.append(path_qs);
    Sleep(2000);
    WinExec(path_ba.data(),SW_HIDE);
    Sleep(2000);

    readName();
    readHeadPixmap();
    readAvNumbers();
    for (auto av : avNumberList)
    {
        qDebug()<<av;
    }
    qDebug()<<avNumberList.length();
    handleAvNumbers();
}

void biliUser::excute_getTags()
{
    QString path_qs = "\"" + qApp->applicationDirPath() + "/findTag.exe" + "\"";
    QByteArray path_ba;
    path_ba.append(path_qs);
    Sleep(2000);
    WinExec(path_ba.data(),SW_HIDE);
    Sleep(3000);

    readTags();
    readKeyword();
    readDescripe();
}

void biliUser::getHomePage()
{
    auto type = (userClass == Class::UP)?HTMLtype::Vedio:HTMLtype::Favlist;
    QUrl url("https://space.bilibili.com/" + uid + "/" + QString((userClass == Class::UP)?"video":"favlist"));
    currentDownload = uid + QString((userClass == Class::UP)?"Video":"Favlist");
    downloadHTML(url,type);
}

void biliUser::handleAvNumbers()
{
    if (avNumberList.isEmpty())
    {
        qDebug()<<"all avNumbers handled";
        currentDownload = "";
        emit allAvNumbersHandled();
        return;
    }

    auto avNumber = avNumberList[0];
    avNumberList.pop_front();
    qDebug()<<"handle avNumber: "<<avNumber;
    qDebug()<<QString("https://www.bilibili.com/video/" + avNumber);
    currentDownload = avNumber;
    downloadHTML(QUrl("https://www.bilibili.com/video/" + avNumber),HTMLtype::Play,avNumber);
}

void biliUser::downloadHTML(const QUrl &url, biliUser::HTMLtype type, const QString& avNumber)
{
    page->load(url);
    connect(page->profile(),&QWebEngineProfile::downloadRequested,
            [this,type,avNumber](QWebEngineDownloadItem *downloadItem) {acceptDownload(downloadItem,type,avNumber);});

    if (type == HTMLtype::Play)
        Sleep(1000);
    else
        Sleep(5000);
}

void biliUser::writeUID()
{
    QFile uidFile(qApp->applicationDirPath() + "/temp/currentUID.txt");
    if (uidFile.open(QFile::WriteOnly|QIODevice::Text))
    {
        QTextStream textStream(&uidFile);
        textStream.setCodec("UTF-8");
        textStream << uid << "\n";
    }
    else
        throw std::runtime_error("cannot open temp/currentUID.txt");
    uidFile.close();
}

void biliUser::readName()
{
    QFile nameFile(qApp->applicationDirPath() + "/temp/currentName.txt");
    if (nameFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream textStream(&nameFile);
        textStream.setCodec("UTF-8");
        name = textStream.readLine();
    }
    else
        throw std::runtime_error("cannot open temp/currentName.txt");
    nameFile.close();
}

void biliUser::readHeadPixmap()
{
    QString fileName(qApp->applicationDirPath() + "/temp/" + uid + "HeadPixmap.txt");
    QFile headPixFile(fileName);
    if (headPixFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream textStream(&headPixFile);
        textStream.setCodec("UTF-8");
        headPixmap = textStream.readLine();
    }
    else
        throw std::runtime_error("cannot open temp/" + uid.toStdString() + "HeadPixmap.txt");
    headPixFile.close();
}

void biliUser::readAvNumbers()
{
    QString fileName(qApp->applicationDirPath() + "/temp/" + uid + QString((userClass == Class::UP)?"VideoLink.txt":"FavlistLink.txt"));
    QFile linkFile(fileName);
    if (linkFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream inStream(&linkFile);
        inStream.setCodec("UTF-8");
        while (!inStream.atEnd())
        {
            auto link = inStream.readLine();
            if (link.length() > 30)
                avNumberList.append(link.mid(31));
            else
                qDebug()<<"readAvNumbers: abnormal link length: "<<link;
        }
    }
    else
        throw std::runtime_error("cannot open file: " + fileName.toStdString());
    linkFile.close();

    avNumberList_copy = avNumberList;

    fileName = qApp->applicationDirPath() + "/temp/currentAV.txt";
    QFile currentAVFile(fileName);
    if (currentAVFile.open(QFile::WriteOnly|QIODevice::Text))
    {
        QTextStream textStream(&currentAVFile);
        textStream.setCodec("UTF-8");
        for (auto avNumber : avNumberList)
        {
            textStream << avNumber <<"\n";
        }
    }
    else
        throw std::runtime_error("cannot open file: temp/currentAV.txt");
}

void biliUser::leadDownload(bool ok)
{
    if (ok)
    {
        page->triggerAction(QWebEnginePage::SavePage,true);
        qDebug()<<"load finished";
    }
}

void biliUser::readTags()
{
    QStringList tagList = {QStringLiteral("动漫"),QStringLiteral("番剧"),QStringLiteral("国创"),QStringLiteral("音乐"),QStringLiteral("舞蹈"),QStringLiteral("游戏"),QStringLiteral("科技"),QStringLiteral("数码"),QStringLiteral("生活"),QStringLiteral("鬼畜"),QStringLiteral("时尚"),QStringLiteral("广告"),QStringLiteral("娱乐"),QStringLiteral("影视"),QStringLiteral("放映厅")};
    for (auto tag : tagList)
        tags[tag] = 0;

    QString fileName(qApp->applicationDirPath() + "/temp/" + uid + "Tags.txt");
    QFile tagFile(fileName);
    if (tagFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream textStream(&tagFile);
        textStream.setCodec("UTF-8");
        while (!textStream.atEnd())
        {
            auto tag = textStream.readLine();
            if (tags.find(tag) != tags.end())
                ++tags[tag];
            else
                qDebug()<<tag;
        }
    }
    else
        throw std::runtime_error("cannot open file: temp/" + uid.toStdString() + "Tags.txt");
    tagFile.close();

    emit readTagsFinished();
}

void biliUser::readKeyword()
{
    QString fileName = qApp->applicationDirPath() + "/temp/" + uid + "Keyword.txt";
    QFile keywordFile(fileName);
    if (keywordFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream textStream(&keywordFile);
        textStream.setCodec("UTF-8");
        while (!textStream.atEnd())
        {
            keywordList.append(textStream.readLine());
        }
    }
    else
        throw std::runtime_error("cannot open file: " + fileName.toStdString());
    keywordFile.close();
}

void biliUser::readDescripe()
{
    QString fileName = qApp->applicationDirPath() + "/temp/" + uid + "Descripe.txt";
    QFile descripeFile(fileName);
    if (descripeFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream textStream(&descripeFile);
        textStream.setCodec("UTF-8");
        while (!textStream.atEnd())
        {
            descripeList.append(textStream.readLine());
        }
    }
    else
        throw std::runtime_error("cannot open file: " + fileName.toStdString());
    descripeFile.close();
}

void biliUser::showStatisticsWidget()
{
    if (avNumberList_copy.isEmpty())
    {
        auto error = new errorWidget(QStringLiteral("什么也没找到..."));
        error->setAttribute(Qt::WA_DeleteOnClose);
        error->show();
        return;
    }

    auto resWidget = new statisticWidget(uid,name,headPixmap,userClass,tags);
    resWidget->setAttribute(Qt::WA_DeleteOnClose);
    connect(resWidget,&statisticWidget::detailLabelClicked,this,&biliUser::showDetailInfoWidget);
    resWidget->show();
}

void biliUser::showWaiteDialog()
{
    auto waiteDialog = new WaiteDialog;
    waiteDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(this,&biliUser::readTagsFinished,[waiteDialog]() {waiteDialog->close();});
    waiteDialog->show();
}

void biliUser::showDetailInfoWidget()
{
    auto detailInfoWidget = new DetailInfoWidget(avNumberList_copy,keywordList,descripeList);
    detailInfoWidget->setAttribute(Qt::WA_DeleteOnClose);
    detailInfoWidget->show();
}
