#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QMap>
#include <QSet>

namespace Ui {
class MainWindow;
}

class QNetworkAccessManager;
class QWebEnginePage;
class DbManager;
class QTableWidgetItem;
class QNetworkReply;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWebEnginePage * webPage = nullptr;
    static const QString baseURL;
    int pageCount = 0;
    QStringList anchors;
    QStringList descriptions;
    QStringList pictures;
    DbManager *db = nullptr;
    QUrl lastUrl;
    QNetworkAccessManager *networkAccMgr = nullptr;
    QMap<QString, QPair<int, QMap<QString, QString>>> picOcc;

public slots:
    void OnPageLoad(bool);
    void OnTimerTimeout();
    void OnPicBeginLoad();
    void OnPicEndLoad(QNetworkReply*);

private slots:
    void on_tableWidget_2_itemClicked(QTableWidgetItem *item);
    void on_pushButton_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
