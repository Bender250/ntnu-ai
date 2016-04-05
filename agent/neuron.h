#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <cmath>

#include "settings.h"

typedef std::vector<float> weights;

class Neuron
{
public: // no care about black boxing (whups)
    weights _w;
    Neuron() = default;
    Neuron(const uint64_t &connections);

    float eval(std::vector<float> const& inputs) const;
    float fast_sigmoid(const float& x) const;
};

#endif // NEURON_H
