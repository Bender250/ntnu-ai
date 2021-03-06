#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "settings.h"
#include "neuron.h"
#include "land.h"
#include <vector>
#include <array>

struct Layer {
    std::vector<Neuron> _v;
};

class Individual
{
private:
    float _fitness;
    std::vector<Layer> _ann;
    Beergame_land _l;
    std::array<uint64_t, 13> _object_counter;
    void eval_step();

    friend class Visualiser;
public:

    Individual();
    Individual(std::vector<Layer> ann) : _fitness(-INFINITY), _ann(ann), _object_counter() {}
    float getFitness() {
        if (_fitness == -INFINITY)
            evaluate_fitness();

        return _fitness;
    }
    float evaluate_fitness();
    void mutate();
    std::unique_ptr<Individual> cross_over(const std::unique_ptr<Individual> &in) const;
    std::unique_ptr<Individual> get_copy() const;
    void print() const;
    std::vector<Layer> getAnn() const;
};

#endif // INDIVIDUAL_H
