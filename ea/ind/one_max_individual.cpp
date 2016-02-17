#include "one_max_individual.h"

std::vector<uint64_t> One_max_individual::to_phenotype_int() const
{
    std::vector<uint64_t> out;
    uint64_t pos = 0;
    uint64_t tmp = 0;
    for (const bool &b : _genotype) {
        tmp = (tmp << 1) + b;
        ++pos;
        if (pos % 64 == 0) {
            out.push_back(tmp);
            tmp = 0;
        }
    }
    return out;
}

std::string One_max_individual::to_phenotype_string() const
{
    std::string out;
    for (const bool &b : _genotype) {
        out.push_back( b ? '1' : '0' );
    }
    return out;
}

float One_max_individual::evaluate_fitness()
{
    uint64_t i = 0;

    for (const bool &b : _genotype) {
        if (b)
            ++i;
    }

    _fitness = (i/_genotype.size());
    return _fitness;
}

void One_max_individual::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
    uint64_t position = rnd_int(Settings::inst()->_randomness_source);
    _genotype[position] = !_genotype[position];
}

One_max_individual One_max_individual::cross_over(const One_max_individual &ind) const
{
    uint64_t position = _genotype.size()/2;
    if (Settings::inst()->_crossover_position_random) {
        std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
        position = rnd_int(Settings::inst()->_randomness_source);
    }


    std::vector<bool> new_indiv(_genotype);

    for (uint64_t i = position; position < _genotype.size(); ++i) {
        new_indiv[i] = ind.get_genotype()[i];
    }

    return One_max_individual(new_indiv);
}

