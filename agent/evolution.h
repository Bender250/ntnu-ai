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

    void update_stats(const Stats &current);
    void finalize_stats();
    void print_fitnesses() const;
public:
    void run();
    Evolution() : _p() {}
    std::unique_ptr<Individual> getLeadingIndividual() const;
};

#endif // EVOLUTION_H
