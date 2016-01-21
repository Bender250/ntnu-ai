#include "mainwindow.h"
#include "ui_mainwindow.h"

static const unsigned int frac = 25;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    ui->swarm_world->setAutoFillBackground(true);
    ui->swarm_world->setPalette(Pal);
    ui->swarm_world->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_individual_count_sliderMoved(int position)
{
    Settings::inst()->setIndiv_count(position);
}

void MainWindow::on_centering_weight_sliderMoved(int position)
{
    double w = std::pow(2, position/frac);
    Settings::inst()->setWeight_first(w);
}

void MainWindow::on_repealing_weight_sliderMoved(int position)
{
    double w = std::pow(2, position/frac);
    Settings::inst()->setWeight_second(w);
}

void MainWindow::on_alignment_weight_sliderMoved(int position)
{
    double w = std::pow(2, position/frac);
    Settings::inst()->setWeight_second(w);
}

void MainWindow::on_velocity_limit_sliderMoved(int position)
{
    Settings::inst()->setVel_limit(position);
}

void MainWindow::on_perimeter_size_sliderMoved(int position)
{
    Settings::inst()->setBoid_perimeter(position);
}

void MainWindow::on_simulation_speed_sliderMoved(int position)
{
    Settings::inst()->setSim_speed(position);
}
