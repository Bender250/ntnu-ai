#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(std::unique_ptr<Individual> &i, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    ui->flatland->initiate(i);
    ui->flatland->setAutoFillBackground(true);
    ui->flatland->setPalette(Pal);
    ui->flatland->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_moves_per_sec_sliderMoved(int position)
{
    ui->flatland->setMoves_per_sec(position/2.0);
}
