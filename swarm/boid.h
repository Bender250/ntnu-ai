#ifndef BOID_H
#define BOID_H

#include <QtGlobal>
#include <cmath>

#include "settings.h"

struct Velocity {
    Velocity(const double set_x = 0,const double  set_y = 0)
        : x(set_x), y(set_y) { }
    double x, y;

    Velocity operator +(const Velocity& a) const {
        return Velocity(a.x+x, a.y+y);
    }

    Velocity operator *(const double w) const {
        return Velocity(x * w, y * w);
    }
};

struct Position {
    Position(const qint64 set_x = 0, const qint64 set_y = 0)
        : x(set_x), y(set_y) { }

    qint64 x, y;

    // for sum of positions
    Position operator +(const Position& a) {
        return Position(a.x+x, a.y+y);
    }

    /*Position operator -(const Position& a) {
        return Position(a.x-x, a.y-y);
    }*/

    //enable sum of position with velocity (for move)
    Position operator +(const Velocity& a) const {
        return Position(a.x+x, a.y+y);
    }

    //enable sum of position with velocity (for move)
    Velocity operator -(const Position& a) const {
        return Velocity(a.x-x, a.y-y);
    }

    bool operator ==(const Position& a) const {
        return (a.x == x && a.y == y);
    }

    Position operator %(const int mod) const {
        return Position(x % mod, y % mod);
    }
};

class Boid
{
private:
    Position _pos;
    Velocity _vel;

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
};

#endif // BOID_H
