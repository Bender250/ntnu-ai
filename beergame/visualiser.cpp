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

void Visualiser::draw_land(QPainter &p) const
{
    p.setPen(Qt::red);
    if (_i->_l.getX() <= 25) {
        QRect r(QPoint(40*_i->_l.getX(), 40*14), QPoint(40*(_i->_l.getX() + 5), 40*15));
        p.drawRect(r);
    } else {
        QRect r1(QPoint(40*_i->_l.getX(), 40*14), QPoint(40*30, 40*15));
        QRect r2(QPoint(40*0, 40*14), QPoint(40*((_i->_l.getX() + 5) % 30), 40*15));
        p.drawRect(r1);
        p.drawRect(r2);
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

    draw_land(p);
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