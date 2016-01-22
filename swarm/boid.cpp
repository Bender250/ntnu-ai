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
    const Velocity first {(float) (first_pos.x() - _pos.x()),
                          (float) (first_pos.y() - _pos.y())};

    _vel = _vel + first  * Settings::inst()->weight_first() * (0.01)
                + second * Settings::inst()->weight_second()
                + third  * Settings::inst()->weight_third();

    if (_vel.length() > Settings::inst()->vel_limit()) {
        double multiple = Settings::inst()->vel_limit() / _vel.length();
        _vel = _vel * multiple;
    }
    _pos = (_pos + _vel.toPointF());
    _pos.setX(my_mod(_pos.x(), Settings::inst()->world_size()));
    _pos.setY(my_mod(_pos.y(), Settings::inst()->world_size()));
}
