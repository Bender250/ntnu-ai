#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Evolution &e, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    e.output(ui->centralWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}
