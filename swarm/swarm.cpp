#include "swarm.h"


void Swarm::move_all_boids_to_new_positions()
{
    move_boids();
    move_predators();
}

void Swarm::move_boids()
{
    for (Boid& boid : _boids) {
        Velocity cohesion = vel_to_avg_pos(boid, Settings::inst()->alignment_perimeter());
        Velocity avoid_neigbours = avoid_neighbours(boid);
        Velocity neigbours_avg_velocity = get_neighbours_avg_vel(boid);
        Velocity avoid = avoid_obsticles(boid) + avoid_predators(boid);
        boid.move_to_new_position(cohesion,
                                  avoid_neigbours,
                                  neigbours_avg_velocity,
                                  avoid);
    }
}

void Swarm::move_predators()
{
    for (Predator& pred : _predators) {
        Velocity farther = vel_to_avg_pos(pred, Settings::inst()->alignment_perimeter() * 2);
        Velocity closer = pred.get_distance(get_closest_boid(pred)) * (-5);
        Velocity avoid_obst = avoid_obsticles(pred);

        pred.move_to_new_position(farther, closer, avoid_obst);
        kill_birds(pred);
    }
}

Velocity Swarm::vel_to_avg_pos(const Boid &curr, const double &max_dist) const
{
    Position avg {0, 0};
    size_t counter = 0;
    for (const Boid& b : _boids) {
        const Velocity dist = curr.get_distance(b.pos());

        if (dist.length() < max_dist) {
            avg = avg + b.pos();
            ++counter;
        }
    }
    if (counter == 0) {
        return Velocity {0, 0};
    }
    avg = avg / counter;

    return -curr.get_distance(avg); // minus, because it should be from curr to avg
}

Velocity Swarm::avoid_neighbours(const Boid& curr) const
{
    Velocity vel {0, 0};

    for (const Boid& b : _boids) {
        // omited around pseudocode if (b != curr), because it does nothing

        const Velocity dist = curr.get_distance(b.pos());

        if (dist.length() < Settings::inst()->repealing_perimeter()) {
            vel = vel + (dist * (10.0 / (dist.length() + 0.01)));
        }
    }

    return vel;
}

Velocity Swarm::get_neighbours_avg_vel(const Boid& curr) const
{
    Velocity vel {0, 0};

    for (const Boid& b : _boids) {
        // omited around pseudocode if (b != curr), because it does almost nothing

        const Velocity dist = curr.get_distance(b.pos());

        if (dist.length() < Settings::inst()->alignment_perimeter()) {
            vel = vel + b.vel();
        }
    }

    return vel / _boids.size();
}

Velocity Swarm::avoid_obsticles(const Boid& curr) const
{
    Velocity vel {0, 0};

    for (const Position& o : _obsticles) {

        const Velocity dist = curr.get_distance(o);

        if ((dist.length() - 5.0) < Settings::inst()->repealing_perimeter()) {
            vel = vel + (dist * (200.0 / (dist.length() * dist.length() + 0.01) ));
        }
    }

    return vel;
}

Velocity Swarm::avoid_predators(const Boid& curr) const
{
    Velocity vel {0, 0};

    for (const Predator &p : _predators) {

        const Velocity dist = curr.get_distance(p.pos());

        if ((dist.length() - 5.0) < Settings::inst()->repealing_perimeter() * 2) {
            vel = vel + (dist * (15.0 / ( dist.length() + 0.01) ));
        }
    }

    return vel;
}

Position Swarm::get_closest_boid(const Predator &pred) const
{
    double dist = Settings::inst()->world_size() * 2;
    const Boid* closest = NULL;

    for (const Boid &b : _boids) {
        if (pred.get_distance(b.pos()).length() < dist) {
            closest = &b;
            dist = pred.get_distance(b.pos()).length();
        }
    }

    return closest->pos();
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

void Swarm::add_predator()
{
    std::uniform_real_distribution<double>
            rnd_double(0, 1023);
    Position p {rnd_double(_randomness_source), rnd_double(_randomness_source)};

    std::uniform_real_distribution<float>
            rnd_float(0.0, Settings::inst()->vel_limit());
    Velocity v {rnd_float(_randomness_source), rnd_float(_randomness_source)};

    _predators.push_back(Predator(p, v));
}

void Swarm::paint_boid(const Boid& b, QPainter& p) const
{
    p.drawEllipse(b.pos(), 5, 5);
    p.drawLine(b.pos(), b.pos() + QPointF(5, 5) + b.vel().toPointF());
}

void Swarm::paint_predator(const Predator &pred, QPainter &p) const
{
    p.setBrush(Qt::red);
    p.drawEllipse(pred.pos(), 5, 5);
    p.drawLine(pred.pos(), pred.pos() + QPointF(5, 5) + pred.vel().toPointF());
    p.setBrush(Qt::white);
}

void Swarm::paint_obsticles(QPainter& p) const
{
    p.setBrush(Qt::black);
    for (const Position& o : _obsticles) {
        p.drawEllipse(o, 10, 10);
    }
    p.setBrush(Qt::white);
}

void Swarm::maintain_counts()
{
    while (Settings::inst()->obsticles_count() != _obsticles.size()) {
        if (Settings::inst()->obsticles_count() > _obsticles.size()) {
            std::uniform_real_distribution<double>
                    rnd_double(0, 1023);
            Position o {rnd_double(_randomness_source), rnd_double(_randomness_source)};

            _obsticles.push_back(o);
        } else {
            _obsticles.pop_back();
        }
    }

    while (Settings::inst()->indiv_count() != _boids.size()) {
        if (Settings::inst()->indiv_count() > _boids.size()) {
            add_boid();
        } else {
            _boids.pop_back();
        }
    }

    while (Settings::inst()->predators_count() != _predators.size()) {
        if (Settings::inst()->predators_count() > _predators.size()) {
            add_predator();
        } else {
            _predators.pop_back();
        }
    }
}

void Swarm::kill_birds(const Predator &pred)
{
    _boids.erase(std::remove_if(_boids.begin(), _boids.end(),
                                [pred] (const Boid boid)
                                { return ( boid.get_distance(pred.pos()).length() < 5.0 ); }),
                 _boids.end());
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

    maintain_counts();

    _timer.setInterval(Settings::inst()->sim_speed());


    move_all_boids_to_new_positions();
    for (const Boid& b : _boids) {
        paint_boid(b, p);
    }
    for (const Predator& pred : _predators) {
        paint_predator(pred, p);
    }
    paint_obsticles(p);
}
