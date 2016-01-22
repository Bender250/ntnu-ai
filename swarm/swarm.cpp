#include "swarm.h"


void Swarm::move_all_boids_to_new_positions()
{
    move_boids();
    move_other();
}

void Swarm::move_boids()
{
    Position avg_position = get_avg_pos();

    for (Boid& boid : _boids) {
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

//TODO get only center from some area
Position Swarm::get_avg_pos() const
{
    if (_boids.empty()) {
        return Position {0, 0};
    }
    Position avg {0, 0};
    for (const Boid& b : _boids) {
        avg = avg + b.pos();
    }
    return avg / _boids.size(); //TODO: maybe, we go out of the world here
}

Velocity Swarm::avoid_neighbours(const Boid& curr) const
{
    Velocity vel {0, 0};

    for (const Boid& b : _boids) {
        // omited around pseudocode if (b != curr), because it does nothing

        const double dist = curr.get_distance(b);

        if (dist < Settings::inst()->repealing_perimeter()) {
            const Velocity diff {(float) (b.pos().x() - curr.pos().x()),
                                 (float) (b.pos().y() - curr.pos().y())};
            vel = vel - diff; //TODO maybe - (can be tested with negative multiple)
        }
    }

    return vel;
}

Velocity Swarm::get_neighbours_avg_vel(const Boid& curr) const
{
    Velocity vel {0, 0};

    for (const Boid& b : _boids) {
        // omited around pseudocode if (b != curr), because it does almost nothing

        const double dist = curr.get_distance(b);

        if (dist < Settings::inst()->alignment_perimeter()) {
            vel = vel + b.vel();
        }
    }

    return vel / _boids.size();
}

void Swarm::add_boid()
{
    std::uniform_real_distribution<double>
            rnd_double(0, 1023);
    Position p {rnd_double(_randomness_source), rnd_double(_randomness_source)};

    std::uniform_real_distribution<float>
            rnd_float(0.0, Settings::inst()->vel_limit());
    Velocity v {rnd_float(_randomness_source), rnd_float(_randomness_source)};

    _boids.push_back(Boid(p, v));
}

void Swarm::paint_boid(const Boid& b, QPainter& p) const
{
    p.drawEllipse(b.pos(), 5, 5); //Warning: may came out of screen
    p.drawLine(b.pos(), b.pos() + QPointF(5, 5) + b.vel().toPointF());
}

Swarm::Swarm(QWidget *parent) : QWidget(parent),
    _randomness_source(seed), _timer(this)
{
    _timer.start(20);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
}

void Swarm::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::black);

    while (Settings::inst()->indiv_count() != _boids.size()) {
        if (Settings::inst()->indiv_count() > _boids.size()) {
            add_boid();
        } else {
            _boids.pop_back();
        }
    }

    _timer.setInterval(Settings::inst()->sim_speed());


    move_all_boids_to_new_positions();
    for (const Boid& b : _boids) {
        paint_boid(b, p);
    }
}
