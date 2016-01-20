#ifndef SWARM_H
#define SWARM_H

#include <QWidget>
#include <vector>
#include <random>

#include "boid.h"

const unsigned int seed = 0;

class Swarm : public QWidget
{
    Q_OBJECT
private:
    //boids swarm
    std::vector<Boid> _boids;

    // auxiliary variables
    std::mt19937 _randomness_source;

    void move_all_boids_to_new_positions();
    void move_boids();
    void move_other();

    Position get_avg_pos() const;
    Velocity get_neighbours_avg_vel(const Boid curr) const;
    Velocity avoid_neighbours(const Boid curr) const;

    void add_boid();

public:
    explicit Swarm(QWidget *parent = 0);


signals:

public slots:
};

#endif // SWARM_H
