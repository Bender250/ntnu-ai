#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>
#include <memory>

#include "individual.h"
#include "./ind/one_max_individual.h"
#include "./ind/lolz.h"
#include "./ind/sequences.h"

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
    uint64_t _current_gen = 0;

    void fitness_testing();
    void adult_selection();
    void adult_selection_deterministic_mixing();
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
    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            return a->getFitness() > b->getFitness();
        }
    } _decreasing_comparator;
    void adult_selection_full_gen_replace_mod();
    void adult_selection_over_production_mod();
    void adult_selection_generational_mixing_mod();
    uint64_t get_best_id() const;
public:
    Population(); //generate initial population
    uint64_t getCurrent_gen() const;
    Stats evaluate();
    void log() const;
    void print_final_fitness() const;
    void print_final_population() const;
};

#endif // POPULATION_H
