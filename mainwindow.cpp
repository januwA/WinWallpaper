#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mInputPlayProcess = new QProcess(this);
    ffplayProgram = qApp->applicationDirPath() + "/ffplay.exe";
    wProgman = FindWindowA("Progman", 0);
    if(wProgman)
        GetWindowRect(wProgman, &wProgmanRect);

    // 禁用最大化按钮
    // https://www.cnblogs.com/ajanuw/p/13563896.html
    Qt::WindowFlags flags = this->windowFlags();
    flags &= ~Qt::WindowMaximizeButtonHint;
    this->setWindowFlags(flags);

    // 托盘图标
    QIcon icon = QIcon(":/assets/images/logo.ico");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("WinWallpaper");

    // 设置托盘右键菜单
    quitAction = new QAction("退出", this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mInputPlayProcess;
    delete trayIcon;
    delete trayIconMenu;
    delete quitAction;
}


void MainWindow::on_pushButton_clicked()
{
    QString filters = "*";
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", NULL, filters);
    if(filePath == "") return;
    ui->lineEdit->setText( filePath );
}

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
    MainWindow* _this = (MainWindow*)Lparam;
    HWND SHELLDLL_DefView = FindWindowExA(hwnd, 0, "SHELLDLL_DefView", 0);
    if(SHELLDLL_DefView != 0)
    {
        _this->wWorker = FindWindowExA(0, hwnd, "WorkerW", 0);
        ShowWindow(_this->wWorker, 0);
        return FALSE;
    }
    return TRUE;
}
void MainWindow::on_pushButton_2_clicked()
{
    QString path =  ui->lineEdit->text();
    if(path.isEmpty() || !wProgman) return;

    mInputPlayProcess->close();

    QStringList arguments = {
        "-noborder",
        "-x", "1920",
        "-y", "1080",
        "-hide_banner",
        "-loglevel", "panic",
    };
    if(ui->checkBox_3->isChecked())
    {
        arguments.push_back("-loop");
        arguments.push_back("0");
    }
    if(ui->checkBox->isChecked())
    {
        arguments.push_back("-an");
    }
    if(ui->checkBox_2->isChecked())
    {
        arguments.push_back("-vn");
    }
    arguments.push_back(path);
    mInputPlayProcess->start(ffplayProgram, arguments);
}

void MainWindow::on_pushButton_3_clicked()
{
    wFFplay = FindWindowA("SDL_app", 0);
    if(wWorker == 0)
    {
        SendMessageTimeoutA(wProgman, 0x52C, 0, 0, 0, 1000, 0);
        EnumWindows(EnumWindowsProc, (LPARAM)this);
    }
    SetParent(wFFplay, wProgman);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        this->showNormal();
        trayIcon->hide();
    }
}

void MainWindow::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    trayIcon->show();
    if(trayIcon->isVisible())
    {
        hide();
    }
}
