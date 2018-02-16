#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEnginePage>
#include <QTimer>
#include "DbManager.h"
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qsrand(QDateTime::currentSecsSinceEpoch());
    ui->setupUi(this);
    baseURL = "https://www.homes.bg/search/апартаменти-под-наем-София/?typeCategory=2&typeId=ApartmentRent&page=";

    webPage = new QWebEnginePage(this);
    connect(webPage, SIGNAL(loadFinished(bool)), this, SLOT(OnPageLoad(bool)));

    db = new DbManager("RentalListings.sqlite");
    ui->adProgeresBar->setRange(0, 1);
    ui->adProgeresBar->setValue(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QTimer::singleShot(100, this, SLOT(OnTimerTimeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
    if(db)
    {
        delete db;
        db = nullptr;
    }
}

void MainWindow::OnPageLoad(bool ok)
{
    if(ok)
    {
        bool proceed = false;
        if(anchors.size())
        {
            QString command = "$(\"tbody > tr > td > b:contains('Допълнителна')\").closest('tbody').find('tr > td').last().text()";
            webPage->runJavaScript(command,
                [this](const QVariant &variant){
                    auto description = variant.toString();
                    descriptions.push_back(description);
                    ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
                    auto tableItem = new QTableWidgetItem(webPage->url().toString());
                    tableItem->setData(Qt::UserRole, description);
                    ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount() - 1, 0, tableItem);
                    db->AddRentalListing(webPage->url().toString(), description);
                    anchors.pop_back();
                }
            );
            proceed = true;
        }
        else if(webPage->url() != lastUrl)
        {
            lastUrl = webPage->url();
            webPage->runJavaScript("$('td[align=\"left\"] a.ver15black').map(function(){return $(this).prop('href');}).get();",
                [this](const QVariant &variant){
                    anchors = variant.toStringList();
                    ui->adProgeresBar->setRange(0, anchors.size());
                    ui->adProgeresBar->setValue(0);
                }
            );
            proceed = true;
        }

        if(proceed)
        {
            int delay = qrand() % 50 + 50;
            QTimer::singleShot(delay, this, SLOT(OnTimerTimeout()));
        }
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
        pageCount++;
        ui->pageLabel->setText(QString("Page ").append(QString::number(pageCount)).append(":"));
        ui->pageLabel->adjustSize();
        webPage->load(baseURL + QString::number(pageCount));
    }
}

void MainWindow::on_tableWidget_2_itemClicked(QTableWidgetItem *item)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    auto matchingListings = db->GetLevenshtineDistance(item->text(), item->data(Qt::UserRole).toString());
    for(ListingMatch & matchListing : matchingListings)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(matchListing.secondUrl));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(matchListing.matchPercent)));
    }
}

void MainWindow::on_pushButton_clicked()
{
    auto searched = ui->lineEdit->text().toLower();
    for (int row = 0; row < ui->tableWidget_2->rowCount(); ++row) {
        auto item = ui->tableWidget_2->item(row, 0);
        if(item && !item->text().toLower().contains(searched))
        {
            ui->tableWidget_2->setRowHidden(row, true);
        }
    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        for (int row = 0; row < ui->tableWidget_2->rowCount(); ++row) {
            ui->tableWidget_2->setRowHidden(row, false);
        }
    }
}
