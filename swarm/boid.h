#ifndef BOID_H
#define BOID_H

#include <QtGlobal>
#include <QVector2D>
#include <QPointF>
#include <cmath>

#include "settings.h"

typedef QVector2D Velocity;

typedef QPointF Position;

class Boid
{
private:
    Position _pos;
    Velocity _vel;

    double my_mod(double a, const size_t& m) const
    {
        a = std::fmod(a, m);
        if (a < 0) {
            a += m;
        }
        return a;
    }

public:
    Boid() : _pos(Position()), _vel(Velocity()) { }
    Boid(Position p, Velocity v) : _pos(p), _vel(v) { }
    void move_boid_to_new_position(const Position& first_pos,
                                   const Velocity& second,
                                   const Velocity& third);

    Position pos() const;
    void setPos(const Position &pos);
    Velocity vel() const;
    void setVel(const Velocity &vel);
    double get_distance(const Boid& b) const;
};

#endif // BOID_H
