#include "visualiser.h"

Visualiser::Visualiser(QWidget *parent) : QWidget(parent), _timer(this), _steps(0)
{
    _timer.start(20);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
}

void Visualiser::initiate(std::unique_ptr<Individual>& i)
{
    _i = std::move(i);
}

void Visualiser::draw_flatland(QPainter &p) const
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

    draw_agent(_i->_f.getC(), p);
}

void Visualiser::draw_poisson(const uint64_t &i, const uint64_t &j, QPainter &p) const
{
    p.setBrush(Qt::red);
    p.drawEllipse(QPoint(index_to_pos(j), index_to_pos(i)), 10, 10);
}

void Visualiser::draw_food(const uint64_t &i, const uint64_t &j, QPainter &p) const
{
    p.setBrush(Qt::green);
    p.drawEllipse(QPoint(index_to_pos(j), index_to_pos(i)), 10, 10);
}

void Visualiser::draw_agent(const Coordinates& c, QPainter &p) const
{
    p.setBrush(Qt::blue);
    p.drawEllipse(QPoint(index_to_pos(c._x), index_to_pos(c._y)), 20, 20);

    // direction
    switch (c._o) {
    case UP:
        p.drawLine(QPoint(index_to_pos(c._x), index_to_pos(c._y)), QPoint(index_to_pos(c._x), index_to_pos(c._y) + 25));
        break;
    case RIGHT:
        p.drawLine(QPoint(index_to_pos(c._x), index_to_pos(c._y)), QPoint(index_to_pos(c._x) + 25, index_to_pos(c._y)));
        break;
    case BOTTOM:
        p.drawLine(QPoint(index_to_pos(c._x), index_to_pos(c._y)), QPoint(index_to_pos(c._x), index_to_pos(c._y) - 25));
        break;
    case LEFT:
        p.drawLine(QPoint(index_to_pos(c._x), index_to_pos(c._y)), QPoint(index_to_pos(c._x) - 25, index_to_pos(c._y)));
        break;
    default:
        break;
    }
}

void Visualiser::step()
{
    _i->eval_step();
    ++_steps;
}

void Visualiser::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::black);

    if (_moves_per_sec > 0) {
        _timer.setInterval(1000/_moves_per_sec);
    } else {
        _timer.setInterval(10000);
    }

    draw_flatland(p);
    step();
    if (_steps > 60) {
        _timer.setInterval(1000000);
        _i->print();
        return;
    }
}

void Visualiser::setMoves_per_sec(float moves_per_sec)
{
    _moves_per_sec = moves_per_sec;
}
