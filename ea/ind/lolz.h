#ifndef LOLZ_H
#define LOLZ_H

#include "individual.h"

class Lolz : public Individual
{
public:
    Lolz() {
        std::uniform_int_distribution<uint64_t> rnd_int;
        uint64_t rnd = rnd_int(Settings::inst()->_randomness_source);

        for (uint64_t i = 0; i < Settings::inst()->_lolz_vector_size; ++i) {
            if (i % 64 == 0) {
                rnd = rnd_int(Settings::inst()->_randomness_source);
            }
            _genotype.push_back(rnd & 0x01);
            rnd >>= 1;
        }
    }

    Lolz(const std::vector<bool> &b) : Individual(b) {}

    std::vector<uint64_t> to_phenotype_int() const;
    std::string to_phenotype_string() const;

    float evaluate_fitness();
    void mutate();
    std::unique_ptr<Individual> cross_over(const std::unique_ptr<Individual> &in) const;
    std::unique_ptr<Individual> get_copy() const;
};

#endif // LOLZ_H
