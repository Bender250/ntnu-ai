#include "boid.h"


Position Boid::pos() const
{
    return _pos;
}

Velocity Boid::vel() const
{
    return _vel;
}

Velocity Boid::get_distance(const Position &p) const
{
    const double size = Settings::inst()->world_size();
    double dx = _pos.x() - p.x();
    if (std::abs(dx) > std::abs((_pos.x() + size) - p.x()))
        dx = (_pos.x() + size) -  p.x();
    if (std::abs(dx) > std::abs(_pos.x() - (p.x()) + size))
        dx = _pos.x() - (p.x()) + size;

    /*
    dx = std::min(dx, std::abs( _pos.x()         -  p.x()));
    dx = std::min(dx, std::abs((_pos.x() + size) -  p.x()));
    dx = std::min(dx, std::abs( _pos.x()         - (p.x()) + size));
    */

    double dy = _pos.y() - p.y();
    if (std::abs(dy) > std::abs((_pos.y() + size) - p.y()))
        dx = (_pos.y() + size) -  p.y();
    if (std::abs(dy) > std::abs(_pos.y() - (p.y()) + size))
        dx = _pos.y() - (p.y()) + size;

    /*
    dy = std::min(dy, std::abs( _pos.y()         -  p.y()));
    dy = std::min(dy, std::abs((_pos.y() + size) -  p.y()));
    dy = std::min(dy, std::abs( _pos.y()         - (p.y()) + size));
    */

    return Velocity {(float) dx, (float) dy};
}

void Boid::move_to_new_position(const Velocity &first,
                                const Velocity &second,
                                const Velocity &third,
                                const Velocity &fourth)
{

    _vel = _vel + first  * Settings::inst()->weight_first() * (0.01)
                + second * Settings::inst()->weight_second()
                + third  * Settings::inst()->weight_third()
                + fourth * Settings::inst()->weight_second();

    if (_vel.length() > Settings::inst()->vel_limit()) {
        double multiple = Settings::inst()->vel_limit() / _vel.length();
        _vel = _vel * multiple;
    }
    _pos = (_pos + _vel.toPointF());
    _pos.setX(my_mod(_pos.x(), Settings::inst()->world_size()));
    _pos.setY(my_mod(_pos.y(), Settings::inst()->world_size()));
}

void Predator::move_to_new_position(const Velocity &farther,
                                    const Velocity &closer,
                                    const Velocity &avoid_obsticles)
{
    const double vel_limit = Settings::inst()->vel_limit() * 1.1;
    _vel = _vel + farther + closer + avoid_obsticles;

    if (_vel.length() > vel_limit) {
        double multiple = vel_limit / _vel.length();
        _vel = _vel * multiple;
    }

    _pos = (_pos + _vel.toPointF());
    _pos.setX(my_mod(_pos.x(), Settings::inst()->world_size()));
    _pos.setY(my_mod(_pos.y(), Settings::inst()->world_size()));
}
