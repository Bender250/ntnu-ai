#include "individual.h"


Fitness Individual::evaluate_fitness()
{
    float cost = 0;
    float dist = 0;

    for (uint64_t i = 0; i < _genome.size() - 1; ++i) {
        cost += v.cost[_genome[i]][_genome[i+1]];
        dist += v.dist[_genome[i]][_genome[i+1]];
    }
    return Fitness { dist, cost };
}

void Individual::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genome.size());
    std::swap(_genome[rnd_int(Settings::inst()->_randomness_source)],
              _genome[rnd_int(Settings::inst()->_randomness_source)]);
}

std::unique_ptr<Individual> Individual::cross_over(const std::unique_ptr<Individual> &in) const
{
    uint64_t position = _genome.size()/2;
    if (Settings::inst()->_crossover_position_random) {
        std::uniform_int_distribution<uint64_t> rnd_int(0, _genome.size());
        position = rnd_int(Settings::inst()->_randomness_source);
    }

    std::array<City, LENGTH> new_indiv(_genome);

    uint64_t j = position;

    for (uint64_t i = 0; i < _genome.size(); ++i) {
        if (std::find(new_indiv.begin(), new_indiv.begin() + position, in->getGenome()[i])
                == new_indiv.begin() + position) {
            new_indiv[j] = in->getGenome()[i];
        }
    }

    return (std::unique_ptr<Individual>(new Individual(new_indiv)));
}

std::unique_ptr<Individual> Individual::get_copy() const
{
    return (std::unique_ptr<Individual>(new Individual(_genome)));
}
