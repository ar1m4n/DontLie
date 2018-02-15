#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QWebEnginePage;
class DbManager;

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

public slots:
    void OnPageLoad(bool);
    void OnTimerTimeout();
};

#endif // MAINWINDOW_H
