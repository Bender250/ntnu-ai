#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "population.h"

class Evolution
{
private:
    Population _p;
    Population x;

    Stats _global_best;
    Stats _global_worst;
    Stats _global_average;

    void update_stats(const Stats &current, uint64_t gen);
public:
    void run();
    Evolution() : _p() {}
};

#endif // EVOLUTION_H
