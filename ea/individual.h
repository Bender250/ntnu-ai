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
    std::vector<bool> _bitset;
    float _fitness;


public:
    Individual() {}
    Individual(const std::vector<bool> &b) : _bitset(b) {}

    virtual std::vector<uint64_t> to_int() const;
    virtual std::string to_string() const;

    virtual float get_fitness() const;
    virtual void mutate();
    virtual Individual cross_over(const Individual &in);

    std::vector<bool> getBitset() const;
};

#endif // INDIVIDUAL_H
