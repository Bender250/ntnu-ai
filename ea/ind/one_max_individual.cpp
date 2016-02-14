#include "one_max_individual.h"

std::vector<uint64_t> One_max_individual::to_int() const
{
    std::vector<uint64_t> out;
    uint64_t pos = 0;
    uint64_t tmp = 0;
    for (const bool &b : _bitset) {
        tmp = (tmp << 1) + b;
        ++pos;
        if (pos % 64 == 0) {
            out.push_back(tmp);
            tmp = 0;
        }
    }
    return out;
}

std::string One_max_individual::to_string() const
{
    std::string out;
    for (const bool &b : _bitset) {
        out.push_back( b ? '1' : '0' );
    }
    return out;
}

float One_max_individual::get_fitness() const
{
    uint64_t i = 0;

    for (const bool &b : _bitset) {
        if (b)
            ++i;
    }

    return (i/_bitset.size());
}

void One_max_individual::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _bitset.size());
    auto r = Settings::inst()->randomness_source(); //what? why I can not pas it directly?
    uint64_t position = rnd_int(r);
    _bitset[position] = !_bitset[position];
}

Individual One_max_individual::cross_over(const Individual &ind)
{
    uint64_t position = _bitset.size()/2;
    if (Settings::inst()->crossover_position_random()) {
        std::uniform_int_distribution<uint64_t>
                rnd_int(0, _bitset.size());
        auto r = Settings::inst()->randomness_source();
        position = rnd_int(r);
    }


    std::vector<bool> new_indiv(_bitset);

    for (uint64_t i = position; position < _bitset.size(); ++i) {
        new_indiv[i] = ind.getBitset()[i];
    }

    return Individual(new_indiv);
}

