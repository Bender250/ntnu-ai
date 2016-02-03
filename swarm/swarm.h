#ifndef SWARM_H
#define SWARM_H

#include <QWidget>
#include <vector>
#include <random>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <algorithm> // for remove_if

#include "boid.h"

const unsigned int seed = 0;

class Swarm : public QWidget
{
    Q_OBJECT
private:
    //boids swarm
    std::vector<Boid> _boids;
    std::vector<Predator> _predators;
    std::vector<Position> _obsticles;

    // auxiliary variables
    std::mt19937 _randomness_source;
    QTimer _timer;
    //QPainter _p;

    void move_all_boids_to_new_positions();
    void move_boids();
    void move_predators();

    Velocity vel_to_avg_pos(const Boid &curr, const double &max_dist) const;
    Velocity get_neighbours_avg_vel(const Boid &curr) const;
    Velocity avoid_neighbours(const Boid &curr) const;
    Velocity avoid_obsticles(const Boid &curr) const;
    Velocity avoid_predators(const Boid &curr) const;
    Position get_closest_boid(const Predator &pred) const;

    void add_boid();
    void add_predator();
    void paint_boid(const Boid& b, QPainter& p) const;
    void paint_predator(const Predator& pred, QPainter& p) const;
    void paint_obsticles(QPainter &p) const;

    void maintain_counts();
    void kill_birds(const Predator &pred);
public:
    explicit Swarm(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);


signals:

public slots:
};

#endif // SWARM_H
