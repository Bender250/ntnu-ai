#include "neuron.h"

Neuron::Neuron(const uint64_t &connections) : _bias(0.0)
{
    std::uniform_real_distribution<float> rnd_flt(-1, 1);
    for (uint64_t i = 0; i < connections; ++i) {
        _w.push_back(rnd_flt(Settings::inst()->_randomness_source));
    }
}

Neuron::Neuron(const uint64_t &connections, const float &w)
{
    for (uint64_t i = 0; i < connections; ++i) {
        _w.push_back(w);
    }
}

float Neuron::eval(const std::vector<float> &inputs) const
{
    float result = 0;
    for (uint64_t i = 0; i < inputs.size(); ++i) {
        result += inputs[i]*_w[i];
    }
    return result + _bias; // fast_sigmoid(result)?
}

float Neuron::eval(const std::vector<float> &inputs, uint64_t const& i) const
{
    return (inputs[i]*_w[0]) + _bias; // fast_sigmoid(result)?
}

float Neuron::fast_sigmoid(const float& x) const {
    return x / (1 + std::abs(x));
}
