#ifndef ONE_MAX_INDIVIDUAL_H
#define ONE_MAX_INDIVIDUAL_H

#include "individual.h"

class One_max_individual : public Individual
{
public:
    One_max_individual() {
        std::uniform_int_distribution<uint64_t> rnd_int;
        uint64_t rnd;
        auto r = Settings::inst()->randomness_source();

        for (uint64_t i = 0; i < Settings::inst()->one_max_vector_size(); ++i) {
            if (i % 64 == 0) {
                rnd = rnd_int(r);
            }
            _bitset.push_back(rnd & 0x01);
            rnd >>= 1;
        }
    }

    One_max_individual(const std::vector<bool> &b) : Individual(b) {}

    std::vector<uint64_t> to_int() const;
    std::string to_string() const;

    float get_fitness() const;
    void mutate();
    Individual cross_over(const Individual &ind);
};

#endif // ONE_MAX_INDIVIDUAL_H
