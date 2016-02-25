#include "sequences.h"


std::vector<uint64_t> Sequences::to_phenotype_int() const
{
    return _genotype;
}

std::string Sequences::to_phenotype_string() const
{
    std::string out;
    for (const auto &g : _genotype) {
        out.append(std::to_string(g));
        out.append(", ");
    }
    return out;
}

float Sequences::evaluate_fitness() //TODO
{
    if (Settings::inst()->_sequences_local) {
        _fitness = ((float) fitness_length_local()) / (float)  Settings::inst()->_sequences_expected_length ;
        return _fitness;
    } else {
        _fitness = ((float) fitness_length_global()) / (float) Settings::inst()->_sequences_expected_length;
        return _fitness;
    }
}

uint64_t Sequences::fitness_length_global() const
{
    uint64_t collisions = 0;
    for (uint64_t len = 1; len < _genotype.size() - 1; ++len) {
        for (uint64_t i = 0; i < _genotype.size() - len; ++i) {
            for (uint64_t j = i + 1; j < _genotype.size() - (len); ++j) {
                if (_genotype[i]     == _genotype[j]
                 && _genotype[i+len] == _genotype[j+len])
                    ++collisions;
            }
        }

    }
    return (collisions == 0) ? _genotype.size() : (_genotype.size() / collisions); //TODO think, if I can calculate something more interesting
}

uint64_t Sequences::fitness_length_local() const
{
    uint64_t collisions = 0;
    for (uint64_t i = 0; i < _genotype.size() - 1; ++i) {
        for (uint64_t j = i + 1; j < _genotype.size() - 1; ++j) {
            if (_genotype[i]   == _genotype[j]
             && _genotype[i+1] == _genotype[j+1])
                ++collisions;
        }
    }
    return (collisions == 0) ? _genotype.size() : (_genotype.size() / collisions);
}

void Sequences::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int;
    uint64_t mut_type = rnd_int(Settings::inst()->_randomness_source) % 3;
    if (_genotype.empty()) {
        mut_type = 2;
    }
    switch (mut_type) {
    case 0: {
        uint64_t position = rnd_int(Settings::inst()->_randomness_source) % _genotype.size();
        _genotype[position] = rnd_int(Settings::inst()->_randomness_source) % Settings::inst()->_sequences_symbols_count;
        break;
    }
    case 1:
        _genotype.erase(_genotype.begin() + (rnd_int(Settings::inst()->_randomness_source) % _genotype.size()));
        break;
    case 2:
        _genotype.push_back(rnd_int(Settings::inst()->_randomness_source) % Settings::inst()->_sequences_symbols_count);
        break;
    }
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
