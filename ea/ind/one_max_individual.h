#ifndef ONE_MAX_INDIVIDUAL_H
#define ONE_MAX_INDIVIDUAL_H

#include "individual.h"

class One_max_individual : public Individual
{
public:
    One_max_individual() {
        std::uniform_int_distribution<uint64_t> rnd_int;
        uint64_t rnd = rnd_int(Settings::inst()->_randomness_source);

        for (uint64_t i = 0; i < Settings::inst()->_one_max_vector_size; ++i) {
            if (i % 64 == 0) {
                rnd = rnd_int(Settings::inst()->_randomness_source);
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
    std::unique_ptr<Individual> cross_over(const std::unique_ptr<Individual> &in) const;
    std::unique_ptr<Individual> get_copy() const;
};

#endif // ONE_MAX_INDIVIDUAL_H
