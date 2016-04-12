#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <cmath>

#include "settings.h"

typedef std::vector<float> weights;

class Neuron
{
private:
    weights _w;
    float _bias;
    float _self_w;
    float _layer_w;

    float _gain;
    float _prev;
    float _time;

    float sigmoid(const float &x) const;
public:
    Neuron(const uint64_t &connections);

    float eval(std::vector<float> const& inputs, float layer_prev);
    float fast_sigmoid(const float& x) const;
    void mutate();
    void print() const;

    float prev() const {
        return _prev;
    }
};

#endif // NEURON_H
