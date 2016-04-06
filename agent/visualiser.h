#ifndef VISUALISER_H
#define VISUALISER_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

#include "flatland.h"
#include "individual.h"
#include "settings.h"

class Visualiser : public QWidget
{
    Q_OBJECT
public:
    explicit Visualiser(QWidget *parent = 0);
    void initiate(std::unique_ptr<Individual>& i);
    void paintEvent(QPaintEvent *);

    void setMoves_per_sec(float moves_per_sec);

private:

    std::unique_ptr<Individual> _i;
    QTimer _timer;
    uint64_t _steps;

    float _moves_per_sec = 1.0;

    uint64_t index_to_pos(uint64_t const& i) const {
        return (i * 50) + 25;
    }

    void draw_flatland(QPainter &p) const;
    void draw_poisson(uint64_t const& i, uint64_t const& j, QPainter &p) const;
    void draw_food(uint64_t const& i, uint64_t const& j, QPainter &p) const;
    void draw_agent(const Coordinates& c, QPainter &p) const;

    void step();
};

#endif // VISUALISER_H
