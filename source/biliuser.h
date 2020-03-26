#ifndef BILIUSER_H
#define BILIUSER_H

#include "enum.h"
#include "statisticwidget.h"
#include "waitedialog.h"
#include "detailinfowidget.h"
#include "errorwidget.h"

#include <QObject>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QAction>
#include <QUrl>
#include <QDebug>
#include <QWidget>
#include <QMap>
#include <QWidget>

class biliUser : public QObject
{
    Q_OBJECT
public:
    explicit biliUser(const QString& uid_,Class userClass_,QObject *parent = nullptr);
    ~biliUser();

    enum class HTMLtype {Vedio,Favlist,Play};

    void reLoad(const QString& uid_,Class userClass_);

signals:
    void downloadHomePageFinished();
    void downloadVideoPageFinished();
    void allAvNumbersHandled();
    void readTagsFinished();

private slots:
    void acceptDownload(QWebEngineDownloadItem *downloadItem,HTMLtype type,const QString& avNumber = QString());
    void showDownloadProcess(qint64 byteRecieved,qint64 byteTotal);
    void excute_getAvNumbers();
    void excute_getTags();
    void handleAvNumbers();

private:
    QString uid;
    QString name;
    QString headPixmap;
    Class userClass;
    QList<QString> avNumberList;
    QList<QString> avNumberList_copy;
    QMap<QString,int> tags;
    QList<QString> keywordList;
    QList<QString> descripeList;

    QString currentDownload;

    QWebEngineView* browser;
    QWebEnginePage* page;

    void getHomePage();
    void downloadHTML(const QUrl& url,HTMLtype type,const QString& avNumber = QString());
    void writeUID();
    void readName();
    void readHeadPixmap();
    void readAvNumbers();
    void leadDownload(bool ok);
    void readTags();
    void readKeyword();
    void readDescripe();
    void showStatisticsWidget();
    void showWaiteDialog();
    void showDetailInfoWidget();
};

#endif // BILIUSER_H
