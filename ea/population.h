#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>
#include <memory>

#include "individual.h"
#include "./ind/one_max_individual.h"

struct Stats {
    float max, min, average;
};

class Population
{
private:
    std::vector<std::unique_ptr<Individual>> _genome; //todo cast to Individual
    std::vector<std::unique_ptr<Individual>> _children;
    std::vector<uint64_t> _parents;
    Stats _current;

    void fitness_testing();
    void adult_selection();
    void adult_selection_full_gen_replace();
    void adult_selection_over_production();
    void adult_selection_generational_mixing();

    void parent_selection();
    void parent_selection_fitness_proportional();
    void parent_selection_sigma_scaling();
    float get_sigma_scaling(uint64_t i, float sig) const;
    float get_sigma() const;
    void parent_selection_tournament();
    void parent_selection_boltzmann();
    float get_boltzmann(uint64_t i) const;
    void parent_selection_rank();
    void parent_selection_deterministic_uniform();
    void parent_selection_stochastic_uniform();
    void reproduction();

    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            return a->getFitness() < b->getFitness();
        }
    } _increasing_comparator;
public:
    Stats evaluate();
    void log() const;
    void print_final_fitness() const;
    Population(); //generate initial population
};

#endif // POPULATION_H
