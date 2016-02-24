#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "individual.h"

class Sequences : public Individual
{
public:
    std::vector<uint64_t> _genotype;

    Sequences() {
        std::uniform_int_distribution<uint64_t> rnd_int;

        for (uint64_t i = 0; i < Settings::inst()->_sequences_symbols_count; ++i) {
            _genotype.push_back(rnd_int(Settings::inst()->_randomness_source)
                                % Settings::inst()->_sequences_symbols_count);
        }
    }

    Sequences(const std::vector<uint64_t> &g) : _genotype(g) {}

    std::vector<uint64_t> to_phenotype_int() const;
    std::string to_phenotype_string() const;

    float evaluate_fitness();
    void mutate();
    std::unique_ptr<Individual> cross_over(const std::unique_ptr<Individual> &in) const;
    std::unique_ptr<Individual> get_copy() const;
private:
    float evaluate_fitness_local();
    float evaluate_fitness_global();
};

#endif // SEQUENCES_H
