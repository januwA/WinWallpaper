#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <qdebug.h>
#include <windows.h>
#include <QtCore>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString version = "v1.0.0";
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QProcess* mInputPlayProcess;
    QString ffplayProgram;
    HWND wProgman = 0;
    RECT wProgmanRect;
    HWND wFFplay = 0;
    HWND wWorker = 0;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    // 点击托盘图标事件
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    // 重写最小化事件
    void hideEvent(QHideEvent *event) override ;

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *quitAction;
};
#endif // MAINWINDOW_H
