#include "lolz.h"

std::vector<uint64_t> Lolz::to_phenotype_int() const
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

std::string Lolz::to_phenotype_string() const
{
    std::string out;
    for (const bool &b : _genotype) {
        out.push_back( b ? '1' : '0' );
    }
    return out;
}

float Lolz::evaluate_fitness()
{
    uint64_t i = 0;
    if (_genotype[0]) {
        for (const bool &b : _genotype) {
            if (b)
                ++i;
            else
                break;
        }
    } else {
        for (const bool &b : _genotype) {
            if (b)
                break;
            else
                ++i;
        }
        i = std::min(i, Settings::inst()->_lolz_z);
    }

    _fitness = ((float) i/(float) _genotype.size());
    return _fitness;
}

void Lolz::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
    uint64_t position = rnd_int(Settings::inst()->_randomness_source);
    _genotype[position] = !_genotype[position];
}

std::unique_ptr<Individual> Lolz::cross_over(const std::unique_ptr<Individual> &in) const
{
    uint64_t position = _genotype.size()/2;
    if (Settings::inst()->_crossover_position_random) {
        std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
        position = rnd_int(Settings::inst()->_randomness_source);
    }


    std::vector<bool> new_indiv(_genotype);

    for (uint64_t i = position; i < _genotype.size(); ++i) {
        new_indiv[i] = in->get_genotype()[i];
    }

    return (std::unique_ptr<Lolz>(new Lolz(new_indiv)));
}

std::unique_ptr<Individual> Lolz::get_copy() const
{
    return (std::unique_ptr<Lolz>(new Lolz(_genotype)));
}

