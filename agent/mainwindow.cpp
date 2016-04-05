#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _steps(0)
{
    ui->setupUi(this);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    ui->flatland->setAutoFillBackground(true);
    ui->flatland->setPalette(Pal);
    ui->flatland->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initiate(std::unique_ptr<Individual>& i)
{
    _i = std::move(i);
}

void MainWindow::on_moves_per_sec_sliderMoved(int position)
{
    _moves_per_sec = position/2;
}

void MainWindow::draw_flatland(QPainter &p) const
{
    for (uint64_t i = 0; i < SIZE; ++i) {
        for (uint64_t j = 0; j < SIZE; ++j) {
            switch (_i->_f.getPosition(i, j)) {
            case POISSON:
                draw_poisson(i, j, p);
                break;
            case FOOD:
                draw_food(i, j, p);
                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::draw_poisson(const uint64_t &i, const uint64_t &j, QPainter &p) const
{
    p.setBrush(Qt::red);
    p.drawEllipse(QPoint(index_to_pos(i) - 5, index_to_pos(j) - 5), 10, 10);
}

void MainWindow::draw_food(const uint64_t &i, const uint64_t &j, QPainter &p) const
{
    p.setBrush(Qt::green);
    p.drawEllipse(QPoint(index_to_pos(i) - 5, index_to_pos(j) - 5), 10, 10);
}

void MainWindow::draw_agent(const uint64_t &i, const uint64_t &j, const Orientation &d, QPainter &p) const
{
    p.setBrush(Qt::blue);
    p.drawEllipse(QPoint(index_to_pos(i) - 10, index_to_pos(j) - 10), 20, 20);
    switch (d) {
    case UP:
        p.drawLine(QPoint(index_to_pos(i), index_to_pos(j)), QPoint(index_to_pos(i), index_to_pos(j) + 25));
        break;
    case RIGHT:
        p.drawLine(QPoint(index_to_pos(i), index_to_pos(j)), QPoint(index_to_pos(i) + 25, index_to_pos(j)));
        break;
    case BOTTOM:
        p.drawLine(QPoint(index_to_pos(i), index_to_pos(j)), QPoint(index_to_pos(i), index_to_pos(j) - 25));
        break;
    case LEFT:
        p.drawLine(QPoint(index_to_pos(i), index_to_pos(j)), QPoint(index_to_pos(i) - 25, index_to_pos(j)));
        break;
    default:
        break;
    }
    // direction
}

void MainWindow::step()
{
    _i->eval_step();
    ++_steps;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::black);

    if (_moves_per_sec > 0) {
        _timer.setInterval(1/_moves_per_sec);
    } else {
        _timer.setInterval(100000);
    }

    draw_flatland(p);
    step();
    if (_steps > 60) {
        _timer.setInterval(100000);
        return;
    }
}
