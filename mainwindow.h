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

    void on_kill_clicked();

    void on_run_clicked();

    void on_suspend_clicked();

    void on_pushButton_clicked();

    void on_runAll_clicked();

    void on_changePriority_clicked();

private:
    Ui::MainWindow *ui;
    int numOfProcesses;
    pid_t PID[8];
    int status;
};
#endif // MAINWINDOW_H
