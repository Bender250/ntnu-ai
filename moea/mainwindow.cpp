#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Evolution *e, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), _e(e) {
    ui->setupUi(this);

    _e->output(ui->centralWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    _e->output(ui->centralWidget);
}
