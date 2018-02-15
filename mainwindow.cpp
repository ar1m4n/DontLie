#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEnginePage>
#include <QTimer>
#include "DbManager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    baseURL = "https://www.homes.bg/search/апартаменти-под-наем-София/?typeCategory=2&typeId=ApartmentRent&page=";

    webPage = new QWebEnginePage(this);
    connect(webPage, SIGNAL(loadFinished(bool)), this, SLOT(OnPageLoad(bool)));

    QTimer::singleShot(500, this, SLOT(OnTimerTimeout()));
    db = new DbManager("RentalListings.sqlite");
    ui->pageProgressBar->setRange(0, 500);
    ui->pageProgressBar->setValue(0);
    ui->adProgeresBar->setRange(0, 1);
    ui->adProgeresBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnPageLoad(bool ok)
{
    if(ok)
    {
        if(anchors.size())
        {
            QString command = "$(\"tbody > tr > td > b:contains('Допълнителна')\").closest('tbody').find('tr > td').last().text()";
            webPage->runJavaScript(command,
                [this](const QVariant &variant){
                    auto description = variant.toString();
                    descriptions.push_back(description);
                    db->AddRentalListing(webPage->url().toString(), description);
                    anchors.pop_back();
                }
            );
        }
        else
        {
            webPage->runJavaScript("$('a.ver15black').map(function(){return $(this).prop('href');}).get();",
                [this](const QVariant &variant){
                    anchors = variant.toStringList();
                    ui->adProgeresBar->setRange(0, anchors.size());
                    ui->adProgeresBar->setValue(0);
                }
            );
        }

        QTimer::singleShot(100, this, SLOT(OnTimerTimeout()));
    }
}

void MainWindow::OnTimerTimeout()
{
    if(anchors.size())
    {
        webPage->load(anchors.back());
        ui->adProgeresBar->setValue(ui->adProgeresBar->value() + 1);
        ui->adLabel->setText(anchors.back());
        ui->adLabel->adjustSize();
    }
    else
    {
        if(pageCount < 500)
        {
            pageCount++;
            ui->pageProgressBar->setValue(pageCount);
            ui->pageLabel->setText(QString("Page ").append(QString::number(pageCount)).append(":"));
            ui->pageLabel->adjustSize();
            webPage->load(baseURL + QString::number(pageCount));
        }
    }
}
