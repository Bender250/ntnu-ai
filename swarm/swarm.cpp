#include "swarm.h"


void Swarm::move_all_boids_to_new_positions()
{
    move_boids();
    move_other();
}

void Swarm::move_boids()
{
    Position avg_position = get_avg_pos();

    foreach (auto boid, _boids) {
        Velocity avoid_neigbours = avoid_neighbours(boid);
        Velocity neigbours_avg_velocity = get_neighbours_avg_vel(boid);
        boid.move_boid_to_new_position(avg_position,
                                       avoid_neigbours,
                                       neigbours_avg_velocity);
    }
}

void Swarm::move_other()
{

}

//TODO the speed must be multiplied by low factor (so it take some
//time to do the distance
Position Swarm::get_avg_pos() const
{
    Position avg {0, 0};
    foreach (auto b, _boids) {
        avg = avg + b.pos();
    }
    return Position {(qint64) (avg.x / _boids.size()),
                     (qint64) (avg.y / _boids.size())};
}

Velocity Swarm::avoid_neighbours(const Boid curr) const
{
    Velocity vel {0, 0};

    foreach (const Boid b, _boids) {
        // omited around pseudocode if (b != curr), because it does nothing

        // if b is in curr perimeter
        const Velocity dist = b.pos() - curr.pos();
        const double d = dist.x * dist.x + dist.y + dist.y;
        if (d < Settings::inst()->boid_perimeter()*Settings::inst()->boid_perimeter()) {
            vel = vel + dist; //TODO maybe - (can be tested with negative multiple
        }
    }

    return vel;
}

Velocity Swarm::get_neighbours_avg_vel(const Boid curr) const
{
    Velocity vel {0, 0};

    foreach (const Boid b, _boids) {
        // omited around pseudocode if (b != curr), because it does almost nothing

        // if b is in curr perimeter
        const Velocity dist = b.pos() - curr.pos();
        const double d = dist.x * dist.x + dist.y + dist.y;
        if (d < Settings::inst()->boid_perimeter()*Settings::inst()->boid_perimeter()) {
            vel = vel + b.vel();
        }
    }

    return Velocity {vel.x / _boids.size(), vel.y / _boids.size()};
}

void Swarm::add_boid()
{
    std::uniform_int_distribution<int>
            rnd_int(0, 1023);
    Position p {rnd_int(_randomness_source), rnd_int(_randomness_source)};

    std::uniform_real_distribution<double>
            rnd_double(0.0, Settings::inst()->vel_limit());
    Velocity v {rnd_double(_randomness_source), rnd_double(_randomness_source)};

    _boids.push_back(Boid(p, v));
}

Swarm::Swarm(QWidget *parent) : QWidget(parent), _randomness_source(seed)
{

}
