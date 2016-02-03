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
protected:
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
    void move_to_new_position(const Velocity &first,
                              const Velocity &second,
                              const Velocity &third,
                              const Velocity &fourth);

    Position pos() const;
    Velocity vel() const;
    Velocity get_distance(const Position &b) const;
};

class Predator : public Boid
{
public:
    Predator(Position p, Velocity v) : Boid(p, v) {}

    void move_to_new_position(const Velocity &farther,
                              const Velocity &closer,
                              const Velocity &avoid_obsticles);
};

#endif // BOID_H
