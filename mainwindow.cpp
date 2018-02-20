#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEnginePage>
#include <QTimer>
#include <QtGlobal>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCryptographicHash>
#include <functional>

const QString MainWindow::baseURL = "https://www.homes.bg/search/апартаменти-под-наем-София/?typeCategory=2&typeId=ApartmentRent&page=";
QString gAdJSCommand = "$('td[align=\"left\"] a.ver15black').map(function(){return $(this).prop('href');}).get();";
QString gDescrJSCommand = "$(\"tbody > tr > td > b:contains('Допълнителна')\").closest('tbody').find('tr > td').last().text()";
QString gPicJSCommand = "$('[onclick^=\"load_img\"] img').map(function(){return $(this).attr('src').replace('s.jpg','b.jpg')}).get()";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qsrand(QDateTime::currentSecsSinceEpoch());
    ui->setupUi(this);

    webPage = new QWebEnginePage(this);
    connect(webPage, SIGNAL(loadFinished(bool)), this, SLOT(OnPageLoad(bool)));

    ui->adProgeresBar->setRange(0, 1);
    ui->adProgeresBar->setValue(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->verticalHeader()->setDefaultSectionSize(100);
    networkAccMgr = new QNetworkAccessManager(this);
    connect(networkAccMgr, SIGNAL (finished(QNetworkReply*)), this, SLOT (OnPicEndLoad(QNetworkReply*)) );

    QTimer::singleShot(100, this, SLOT(OnTimerTimeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnPageLoad(bool ok)
{
    if(ok)
    {
        if(!anchors.empty())
        {
            webPage->runJavaScript(gPicJSCommand, [this](QVariant result){
                pictures = result.toStringList();
                for(QString &pic : pictures)
                {
                    QString protocol = "https";
                    if(pic.startsWith(protocol))
                    {
                        pic.replace(0, protocol.length(), "http");
                    }
                }
                QTimer::singleShot(qrand() % 30 + 30, this, SLOT(OnPicBeginLoad()));
            });
        }
        else
        {
            webPage->runJavaScript(gAdJSCommand, [this](QVariant result){
                anchors = result.toStringList();
                lastUrl = webPage->url();
                ui->adProgeresBar->setValue(0);
                ui->adProgeresBar->setRange(0, anchors.size());
            });

            QTimer::singleShot(qrand() % 30 + 30, this, SLOT(OnTimerTimeout()));
        }
    }
}

void MainWindow::OnTimerTimeout()
{
    if(!anchors.empty())
    {
        descriptions.clear();
        pictures.clear();
        webPage->load(anchors.back());
        ui->adProgeresBar->setValue(ui->adProgeresBar->value() + 1);
        ui->adLabel->setText(anchors.back());
        ui->adLabel->adjustSize();
    }
    else
    {
        QUrl url(QString(baseURL).append(QString::number(pageCount)));
        if(url != lastUrl)
        {
            pageCount++;
            webPage->load(url);
            ui->pageLabel->setText(QString::number(pageCount));
        }
    }
}

void MainWindow::OnPicBeginLoad()
{
    if(!pictures.empty())
    {
        QNetworkRequest request(QUrl(pictures.back()));
        networkAccMgr->get(request);
    }
    else
    {
        QTimer::singleShot(qrand() % 30 + 30, this, SLOT(OnTimerTimeout()));
        anchors.pop_back();
    }
}

void MainWindow::OnPicEndLoad(QNetworkReply * reply)
{
    if(reply)
    {
        auto byteArray = reply->readAll();
        if(byteArray.size())
        {
            QCryptographicHash md5(QCryptographicHash::Md5);
            md5.addData(byteArray.toBase64());
            QString hash(md5.result().toBase64().constData());
            auto found = picOcc.find(hash);
            if(found == picOcc.end())
                found = picOcc.insert(hash, qMakePair(-1, QMap<QString, QString>()));

            found->second.insert(anchors.back(), pictures.back());
            if(found->second.size() > 1 && found->first == -1)
            {
                QTableWidgetItem *item = new QTableWidgetItem(anchors.back());
                QImage img = QImage::fromData(byteArray, "JPG");
                img = img.scaled(100, 100, Qt::KeepAspectRatio);
                item->setData(Qt::UserRole, hash);
                item->setData(Qt::DecorationRole, QPixmap::fromImage(img));
                ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
                found->first = ui->tableWidget_2->rowCount() - 1;
                ui->tableWidget_2->setItem(found->first, 0, item);
            }
        }
        reply->deleteLater();
    }
    pictures.pop_back();
    QTimer::singleShot(qrand() % 30 + 30, this, SLOT(OnPicBeginLoad()));
}

void MainWindow::on_tableWidget_2_itemClicked(QTableWidgetItem *item)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    auto found = picOcc.find(item->data(Qt::UserRole).toString());
    for(const QString &url : found->second.keys())
    {
        QString picUrl = found->second[url];
        int right = picUrl.lastIndexOf('_') - 1;
        int left = picUrl.lastIndexOf('/', right);
        QString date = picUrl.mid(left + 1, right - left);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(date));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(url));
    }
}

void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::on_lineEdit_textChanged(const QString &)
{
}
