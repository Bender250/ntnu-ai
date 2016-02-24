#include "sequences.h"


std::vector<uint64_t> Sequences::to_phenotype_int() const
{
    return _genotype;
}

std::string Sequences::to_phenotype_string() const
{
    std::string out;
    for (const bool &g : _genotype) {
        out.append(std::to_string(g));
        out.append(", ");
    }
    return out;
}

float Sequences::evaluate_fitness() //TODO
{
    if (Settings::inst()->_sequences_local) {
        return evaluate_fitness_local();
    } else {
        return evaluate_fitness_global();
    }
}

float Sequences::evaluate_fitness_local() //TODO
{
    return 0.0;
}

float Sequences::evaluate_fitness_global() //TODO
{
    return 0.0;
}

void Sequences::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
    uint64_t position = rnd_int(Settings::inst()->_randomness_source);
    _genotype[position] = !_genotype[position];
}

std::unique_ptr<Individual> Sequences::cross_over(const std::unique_ptr<Individual> &in) const
{
    uint64_t position = _genotype.size()/2;
    if (Settings::inst()->_crossover_position_random) {
        std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
        position = rnd_int(Settings::inst()->_randomness_source);
    }


    std::vector<uint64_t> new_indiv(_genotype);

    for (uint64_t i = position; i < _genotype.size(); ++i) {
        new_indiv[i] = in->get_genotype()[i];
    }

    return (std::unique_ptr<Sequences>(new Sequences(new_indiv)));
}

std::unique_ptr<Individual> Sequences::get_copy() const
{
    return (std::unique_ptr<Sequences>(new Sequences(_genotype)));
}
