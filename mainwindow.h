#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_autoButton_clicked();

private:
    Ui::MainWindow *ui;
    int getSRUrl(QString path);
    void showSRUrl(QString path);
    QString srUrl;
    QString path;

    void saveConfig();
    void loadConfig();
};
#endif // MAINWINDOW_H
