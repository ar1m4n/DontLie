#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class QWebEnginePage;
class DbManager;
class QTableWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWebEnginePage * webPage = nullptr;
    QString baseURL;
    int pageCount = 0;
    QStringList anchors;
    QStringList descriptions;
    DbManager *db = nullptr;
    QUrl lastUrl;

public slots:
    void OnPageLoad(bool);
    void OnTimerTimeout();
private slots:
    void on_tableWidget_2_itemClicked(QTableWidgetItem *item);
    void on_pushButton_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
