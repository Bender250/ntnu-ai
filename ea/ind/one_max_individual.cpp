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
    uint64_t i = 0, j = 0;
    uint64_t x = Settings::inst()->_one_max_target_vector;

    for (const bool &b : _genotype) {
        bool tmp = x & (0x1 << j);
        if (b == tmp) {
            ++i;
        }
        ++j;
    }

    _fitness = ((float) i/(float) j);
    return _fitness;
}

void One_max_individual::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
    uint64_t position = rnd_int(Settings::inst()->_randomness_source);
    _genotype[position] = !_genotype[position];
}

std::unique_ptr<Individual> One_max_individual::cross_over(const std::unique_ptr<Individual> &in) const
{
    //in = dynamic_cast<std::unique_ptr<One_max_individual>>(in);
    uint64_t position = _genotype.size()/2;
    if (Settings::inst()->_crossover_position_random) {
        std::uniform_int_distribution<uint64_t> rnd_int(0, _genotype.size());
        position = rnd_int(Settings::inst()->_randomness_source);
    }


    std::vector<bool> new_indiv(_genotype);

    for (uint64_t i = position; i < _genotype.size(); ++i) {
        new_indiv[i] = in->get_genotype()[i];
    }

    return (std::unique_ptr<One_max_individual>(new One_max_individual(new_indiv)));
}

std::unique_ptr<Individual> One_max_individual::get_copy() const
{
    return (std::unique_ptr<One_max_individual>(new One_max_individual(_genotype)));
}

