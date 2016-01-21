#include "boid.h"


Position Boid::pos() const
{
    return _pos;
}

void Boid::setPos(const Position &pos)
{
    _pos = pos;
}

Velocity Boid::vel() const
{
    return _vel;
}

void Boid::setVel(const Velocity &vel)
{
    _vel = vel;
}

void Boid::move_boid_to_new_position(const Position &first_pos,
                                     const Velocity &second,
                                     const Velocity &third)
{
    const Velocity first = first_pos - _pos;

    _vel = _vel + first  * Settings::inst()->weight_first()
                + second * Settings::inst()->weight_second()
                + third  * Settings::inst()->weight_third();
    //_pos = (_pos + _vel) % 1024;
    _pos.x = ((int) (_pos.x + _vel.x)) % 1024;
    _pos.y = ((int) (_pos.y + _vel.y)) % 1024;
}
