#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <stdlib.h>
#include<unistd.h>
#include<errno.h>
#include <sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<sys/resource.h>
#include<string.h>
#include <math.h>
#include <time.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    numOfProcesses = ui->numOfProcesses->currentText().toInt();
    ui->listOfProcesses->setRowCount(numOfProcesses);
    ui->listOfProcesses->setColumnCount(3);
    ui->listOfProcesses->setHorizontalHeaderItem(0, new QTableWidgetItem("PID"));
    ui->listOfProcesses->setHorizontalHeaderItem(1, new QTableWidgetItem("Priority"));
    ui->listOfProcesses->setHorizontalHeaderItem(2, new QTableWidgetItem("State"));
    int steps_q = ui->steps->toPlainText().toInt()/numOfProcesses;
    double A = 0.2, B = 0.8;
    double division = (B-A)/numOfProcesses;

    for(int i = 0; i < numOfProcesses; i++)
    {
        B = A + division;
        PID[i] = fork();

        if(PID[i] < 0){
            std::perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if(PID[i] == 0)
        {

            std::string o_file = "tabulation" + std::to_string(i);

            /*char* args[] = { (char*)"/home/student/OS/linux_processes/program_exe",
                             (char*)"home/student/OS/linux_processes/program_exe",
                           (char*)&A, (char*)&B, (char*)&steps_q, (char*)&o_file,
                           NULL};
            execv(args[0], args);*/

            execlp("/home/yaroslav/student/OS/linux_processes/program_exe",
                              "program_exe",
                              std::to_string(A).c_str(),
                              std::to_string(B).c_str(),
                              std::to_string(steps_q).c_str(),
                              o_file.c_str(), nullptr);
        }
        else
        {
            kill(PID[i], SIGSTOP);
        }
        A = B;
        ui->listOfProcesses->setItem(i, 0, new QTableWidgetItem(QString::number(PID[i])));
        ui->listOfProcesses->setItem(i, 1, new QTableWidgetItem(QString::number(getpriority(PRIO_PROCESS, PID[i]))));
        ui->listOfProcesses->setItem(i,2,new QTableWidgetItem("Ready"));

    }
}

void MainWindow::on_runAll_clicked()
{
    for(int i = 0; i < numOfProcesses; i++)
    {
        kill(PID[i], SIGCONT);
        //wait(&status);
        ui->listOfProcesses->setItem(i, 1, new QTableWidgetItem(QString::number(getpriority(PRIO_PROCESS, PID[i]))));
        ui->listOfProcesses->setItem(i,2,new QTableWidgetItem("Running"));
    }
}

void MainWindow::on_changePriority_clicked()
{
    setpriority(PRIO_PROCESS, PID[ui->listOfProcesses->currentIndex().row()], ui->priority->toPlainText().toInt());
    ui->listOfProcesses->setItem(ui->listOfProcesses->currentIndex().row(), 1, new QTableWidgetItem(QString::number(getpriority(PRIO_PROCESS, PID[ui->listOfProcesses->currentIndex().row()]))));
}

void MainWindow::on_kill_clicked()
{
    kill(PID[ui->listOfProcesses->currentIndex().row()], SIGTERM);
    ui->listOfProcesses->setItem(ui->listOfProcesses->currentIndex().row(),2,new QTableWidgetItem("Terminated"));
}

void MainWindow::on_run_clicked()
{

    kill(PID[ui->listOfProcesses->currentIndex().row()], SIGCONT);
    ui->listOfProcesses->setItem(ui->listOfProcesses->currentIndex().row(),2,new QTableWidgetItem("Running"));
    wait(&status);
}

void MainWindow::on_suspend_clicked()
{
    kill(PID[ui->listOfProcesses->currentIndex().row()], SIGSTOP);
    ui->listOfProcesses->setItem(ui->listOfProcesses->currentIndex().row(),2,new QTableWidgetItem("Ready"));
}
