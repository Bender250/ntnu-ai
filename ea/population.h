#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "individual.h"
#include "./ind/one_max_individual.h"
#include <algorithm>

struct Stats {
    float max, min, average;
};

class Population
{
private:
    std::vector<One_max_individual> _genome; //todo cast to Individual
    Stats _current;

    void fitness_testing();
    void adult_selection();
    void adult_selection_full_gen_replace();
    void adult_selection_over_production();
    void adult_selection_generational_mixing();

    void parent_selection();
    void reproduction();
public:
    Stats evaluate();
    void log() const;
    void print_final_fitness() const;
    Population(); //generate initial population
};

#endif // POPULATION_H
