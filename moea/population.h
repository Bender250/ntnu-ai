#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>
#include <memory>

#define QCUSTOMPLOT_USE_LIBRARY // TODO: is necessary?
#include <qcustomplot.h>

#include "individual.h"

struct Stats {
    float max, min, average;
};

class Population
{
private:
    std::vector<std::unique_ptr<Individual>> _genome; //todo cast to Individual
    std::vector<std::unique_ptr<Individual>> _children;
    std::vector<uint64_t> _parents;

    uint64_t _current_gen = 0;

    void fitness_testing();
    void adult_selection();
    void adult_selection_deterministic_mixing();
    void adult_selection_full_gen_replace();
    void adult_selection_over_production();
    void adult_selection_generational_mixing();

    void parent_selection();
    void parent_selection_tournament();
    void reproduction();

    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            return a->dist_fitness() > b->dist_fitness();
        }
    } _decreasing_dist_comparator;

    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            return a->cost_fitness() > b->cost_fitness();
        }
    } _decreasing_cost_comparator;

    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            return a->dist_fitness() < b->dist_fitness();
        }
    } _increasing_dist_comparator;

    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            return a->cost_fitness() < b->cost_fitness();
        }
    } _increasing_cost_comparator;

    // comparators based on rank
    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            return a->getRank() < b->getRank();
        }
    } _increasing_rank_comparator;
    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            return a->getRank() > b->getRank();
        }
    } _decreasing_rank_comparator;

    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            if (a->getRank() == b->getRank()) {
                return a->getCrowding_distance() > b->getCrowding_distance();
            } else {
                return a->getRank() > b->getRank();
            }
        }
    } _decreasing_total_comparator;
    struct {
        bool operator ()(std::unique_ptr<Individual> &a, std::unique_ptr<Individual> &b) {
            if (a->getRank() == b->getRank()) {
                return a->getCrowding_distance() < b->getCrowding_distance();
            } else {
                return a->getRank() < b->getRank();
            }
        }
    } _increasing_total_comparator;

    void adult_selection_full_gen_replace_mod();
    void adult_selection_over_production_mod();
    void adult_selection_generational_mixing_mod();
    uint64_t get_best_id() const;
    void rank_evaluate();

    void crowding_distance_evaluate();

public:
    Population(); //generate initial population
    uint64_t getCurrent_gen() const;
    Stats evaluate();
    void log() const;
    void print_final_fitness() const;
    void print_final_population(QCustomPlot *customPlot);
};

#endif // POPULATION_H
