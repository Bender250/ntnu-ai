#ifndef ONE_MAX_INDIVIDUAL_H
#define ONE_MAX_INDIVIDUAL_H

#include "individual.h"

class One_max_individual : public Individual
{
public:
    One_max_individual() {
        std::uniform_int_distribution<uint64_t> rnd_int;
        uint64_t rnd;
        auto r = Settings::inst()->_randomness_source;

        for (uint64_t i = 0; i < Settings::inst()->_one_max_vector_size; ++i) {
            if (i % 64 == 0) {
                rnd = rnd_int(r);
            }
            _genotype.push_back(rnd & 0x01);
            rnd >>= 1;
        }
    }

    One_max_individual(const std::vector<bool> &b) : Individual(b) {}

    std::vector<uint64_t> to_phenotype_int() const;
    std::string to_phenotype_string() const;

    float evaluate_fitness();
    void mutate();
    One_max_individual cross_over(const One_max_individual &ind) const;
};

#endif // ONE_MAX_INDIVIDUAL_H
