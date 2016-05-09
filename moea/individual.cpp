#include "individual.h"


Fitness Individual::evaluate_fitness()
{
    uint64_t min = std::min(_genotype[0], _genotype[_genotype.size() - 1]);
    uint64_t max = std::max(_genotype[0], _genotype[_genotype.size() - 1]);
    _fitness.cost = v.cost[max][min];
    _fitness.dist = v.dist[max][min];

    for (uint64_t i = 0; i < _genotype.size() - 1; ++i) {
        min = std::min(_genotype[i], _genotype[i+1]);
        max = std::max(_genotype[i], _genotype[i+1]);
        _fitness.cost += v.cost[max][min];
        _fitness.dist += v.dist[max][min];
    }
    return _fitness;
}

void Individual::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
    std::swap(_genotype[rnd_int(Settings::inst()->_randomness_source)],
              _genotype[rnd_int(Settings::inst()->_randomness_source)]);
}

std::unique_ptr<Individual> Individual::cross_over(const std::unique_ptr<Individual> &in) const
{
    uint64_t position = _genotype.size()/2;
    if (Settings::inst()->_crossover_position_random) {
        std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
        position = rnd_int(Settings::inst()->_randomness_source);
    }

    std::array<City, LENGTH> new_indiv(_genotype);

    uint64_t j = position;

    for (uint64_t i = 0; i < _genotype.size(); ++i) {
        if (std::find(new_indiv.begin(), new_indiv.begin() + position, in->getGenome()[i])
                == new_indiv.begin() + position) {
            new_indiv[j] = in->getGenome()[i];
        }
    }

    return (std::unique_ptr<Individual>(new Individual(new_indiv)));
}

std::unique_ptr<Individual> Individual::get_copy() const
{
    return (std::unique_ptr<Individual>(new Individual(_genotype)));
}
