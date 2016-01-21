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

    _vel = _vel + first  * Settings::inst()->weight_first() * (0.01)
                + second * Settings::inst()->weight_second()
                + third  * Settings::inst()->weight_third();

    double curr_vel = sqrt(_vel.x * _vel.x  + _vel.y * _vel.y);
    if (curr_vel > Settings::inst()->vel_limit()) {
        double multiple = Settings::inst()->vel_limit() / curr_vel;
        _vel = _vel * multiple;
    }
    _pos = (_pos + _vel) % Settings::inst()->world_size();
    //_pos.x = ((int) (_pos.x + _vel.x)) % Settings::inst()->world_size();
    //_pos.y = ((int) (_pos.y + _vel.y)) % Settings::inst()->world_size();
}
