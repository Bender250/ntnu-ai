#include "neuron.h"

Neuron::Neuron(const uint64_t &connections)
{
    std::uniform_real_distribution<float> rnd_flt1(-5.0, 5.0);
    _self_w = rnd_flt1(Settings::inst()->_randomness_source);
    _layer_w = rnd_flt1(Settings::inst()->_randomness_source);

    std::uniform_real_distribution<float> rnd_flt2(-10.0, 0);
    _bias = rnd_flt2(Settings::inst()->_randomness_source);

    std::uniform_real_distribution<float> rnd_flt3(1.0, 5.0);
    _gain = rnd_flt3(Settings::inst()->_randomness_source);

    std::uniform_real_distribution<float> rnd_flt4(1.0, 2.0);
    _time = rnd_flt4(Settings::inst()->_randomness_source);

    for (uint64_t i = 0; i < connections; ++i) {
        _w.push_back(rnd_flt1(Settings::inst()->_randomness_source));
    }
}

float Neuron::eval(const std::vector<float> &inputs)
{
    float result = 0;
    for (uint64_t i = 0; i < inputs.size(); ++i) {
        result += inputs[i]*_w[i];
    }
    // result is equal to s_i without I_i
    result += _bias; // bias is part of I_i

    result = fast_sigmoid(result);

    result -= _prev;

    _prev = result;
    return result; // fast_sigmoid(result)?
}

float Neuron::fast_sigmoid(const float& x) const {
    return x / (1 + std::abs(x));
}

void Neuron::mutate()
{
    std::uniform_real_distribution<float> rnd_flt(-5.0, 5.0);
    std::uniform_int_distribution<int> rnd_int(0, 1);

    if (rnd_int(Settings::inst()->_randomness_source) == 1) {
        _w[rnd_int(Settings::inst()->_randomness_source)]
                = rnd_flt(Settings::inst()->_randomness_source);
    } else {
        std::uniform_int_distribution<int> rnd_int2(0, 4);
        switch (rnd_int2(Settings::inst()->_randomness_source)) {
        case 0: {
            _self_w = rnd_flt(Settings::inst()->_randomness_source);
            break;
        } case 1: {
            _layer_w = rnd_flt(Settings::inst()->_randomness_source);
            break;
        } case 2: {
            std::uniform_real_distribution<float> rnd_flt2(-10.0, 0);
            _bias = rnd_flt2(Settings::inst()->_randomness_source);
            break;
        } case 3: {
            std::uniform_real_distribution<float> rnd_flt3(1.0, 5.0);
            _gain = rnd_flt3(Settings::inst()->_randomness_source);
            break;
        } case 4: {
            std::uniform_real_distribution<float> rnd_flt4(1.0, 2.0);
            _time = rnd_flt4(Settings::inst()->_randomness_source);
            break;
        } default:
            break;
        }
    }
}

void Neuron::print() const
{
    std::cout << "weights: ";
    for (auto const& w : _w) {
        std::cout << w << ", ";
    }
    std::cout << "self_w: " << _self_w << ", ";
    std::cout << "layer_w: " << _layer_w << ", ";
    std::cout << "gain: " << _gain << ", ";
    std::cout << "bias: " << _bias << ", ";
    // time?
    std::cout << std::endl;
}
