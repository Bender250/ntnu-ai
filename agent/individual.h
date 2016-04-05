#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "settings.h"
#include "neuron.h"
#include "flatland.h"
#include <vector>

struct Layer {
    std::vector<Neuron> _v;
};

class Individual
{
private:
    float _fitness;
    std::vector<Layer> _ann;
    Flatland _f;
    uint64_t _food_counter;
    uint64_t _poisson_counter;
    void eval_step();

    friend class MainWindow;
public:

    Individual();
    Individual(std::vector<Layer> ann) : _ann(ann) {}
    float getFitness() {
        return _fitness;
    }
    float evaluate_fitness();
    void mutate();
    std::unique_ptr<Individual> cross_over(const std::unique_ptr<Individual> &in) const;
    std::unique_ptr<Individual> get_copy() const;
};

#endif // INDIVIDUAL_H
