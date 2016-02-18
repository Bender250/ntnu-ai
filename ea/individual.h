#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <string>
#include <cstdint>
#include <random>
#include <iterator>

#include "settings.h"


class Individual
{
protected:
    std::vector<bool> _genotype;
    float _fitness;


public:
    Individual() {}
    Individual(const std::vector<bool> &b) : _genotype(b) {}

    virtual std::vector<uint64_t> to_phenotype_int() const = 0;
    virtual std::string to_phenotype_string() const = 0;

    virtual float evaluate_fitness() = 0;
    virtual void mutate() = 0;
    virtual std::unique_ptr<Individual> cross_over(const std::unique_ptr<Individual> &in) const = 0;
    virtual std::unique_ptr<Individual> get_copy() const = 0;

    std::vector<bool> get_genotype() const {
        return _genotype;
    }

    float getFitness() const {
        return _fitness;
    }
};

#endif // INDIVIDUAL_H
