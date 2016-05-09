#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <string>
#include <cstdint>
#include <random>
#include <iterator>

#include "settings.h"

using City = unsigned char;

struct Fitness {
    float dist, cost;
};

static const Values v;

class Individual
{
private:
    std::array<City, LENGTH> _genotype;
    Fitness _fitness;
    uint64_t _rank;
    float _crowding_distance;

public:
    Individual() : _fitness({0, 0}), _rank(0xFFFFFFFFFFFFFFFF), _crowding_distance(0) {
        for (uint64_t i = 0; i < LENGTH; ++i) {
            _genotype[i] = i;
        }
        std::shuffle(_genotype.begin(), _genotype.end(), Settings::inst()->_randomness_source);
    }
    Individual(std::array<City, LENGTH> genome) : _genotype(genome), _fitness({0, 0}), _rank(0xFFFFFFFFFFFFFFFF), _crowding_distance(0) {
        // _genotype is already set!
    }

    Fitness evaluate_fitness();
    void mutate();
    std::unique_ptr<Individual> cross_over(const std::unique_ptr<Individual> &in) const;
    std::unique_ptr<Individual> get_copy() const;

    float dist_fitness() const {
        return _fitness.dist;
    }
    float cost_fitness() const {
        return _fitness.cost;
    }
    Fitness fitness() const {
        return _fitness;
    }
    uint64_t getRank() const {
        return _rank;
    }
    void setRank(const uint64_t &rank) {
        _rank = rank;
    }
    float getCrowding_distance() const {
        return _crowding_distance;
    }
    void setCrowding_distance(float crowding_distance) {
        _crowding_distance = crowding_distance;
    }
    std::array<City, LENGTH> getGenome() const {
        return _genotype;
    }
};

#endif // INDIVIDUAL_H
