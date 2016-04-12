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
        p.fillRect(r, Qt::red);
    } else {
        QRect r1(QPoint(40*_i->_l.getX(), 40*14), QPoint(40*30, 40*15));
        QRect r2(QPoint(40*0, 40*14), QPoint(40*((_i->_l.getX() + 5) % 30), 40*15));
        p.fillRect(r1, Qt::red);
        p.fillRect(r2, Qt::red);
    }

    p.setPen(Qt::black);
    if (_i->_l.getF().x <= 25) {
        QRect r(QPoint(40*_i->_l.getF().x, 40*(14 - _i->_l.getF().y)), QPoint(40*(_i->_l.getF().x + _i->_l.getF().l), 40*((14 - _i->_l.getF().y) + 1)));
        p.fillRect(r, Qt::black);
    } else {
        QRect r1(QPoint(40*_i->_l.getF().x, 40*(14 - _i->_l.getF().y)), QPoint(40*30, 40*((14 - _i->_l.getF().y) + 1)));
        QRect r2(QPoint(40*0, 40*(14 - _i->_l.getF().y)), QPoint(40*((_i->_l.getF().x + _i->_l.getF().l) % 30), 40*((14 - _i->_l.getF().y) + 1)));
        p.fillRect(r1, Qt::black);
        p.fillRect(r2, Qt::black);
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
    if (_steps > 600) {
        _i->print();
        _timer.setInterval(1000000);
        return;
    }
}

void Visualiser::setMoves_per_sec(float moves_per_sec)
{
    _moves_per_sec = moves_per_sec;
}
