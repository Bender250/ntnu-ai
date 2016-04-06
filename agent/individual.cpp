#include "individual.h"

Individual::Individual() : _food_counter(0), _poisson_counter(0)
{
    Layer first;
    for (uint64_t i = 0; i < 3; ++i) {
        first._v.push_back(Neuron(1)); // 1: input is directly connected to sensors
    }
    _ann.push_back(first);

    if (Settings::inst()->_internal_layers_count >= 1) {
        Layer int_first;
        for (uint64_t i = 0; i < Settings::inst()->_internal_layers_sizes[0]; ++i) {
            int_first._v.push_back(Neuron(3));
        }
        _ann.push_back(int_first);
    }

    for (uint64_t i = 1; i < Settings::inst()->_internal_layers_sizes.size(); ++i) {
        Layer l;
        for (uint64_t i = 0; i < Settings::inst()->_internal_layers_sizes[i]; ++i) {
            l._v.push_back(Neuron(Settings::inst()->_internal_layers_sizes[i-1]));
        }
        _ann.push_back(l);
    }

    Layer last;
    if (Settings::inst()->_internal_layers_count >= 1) {
        for (uint64_t i = 0; i < 3; ++i) {
            last._v.push_back(Neuron(Settings::inst()->_internal_layers_sizes[Settings::inst()->_internal_layers_sizes.size()-1]));
        }
    } else {
        for (uint64_t i = 0; i < 3; ++i) {
            last._v.push_back(Neuron(3));
        }
    }
    _ann.push_back(last);
}

void Individual::eval_step()
{
    std::vector<float> inputs;
    Surrounding s = _f.getSurrounding();
    inputs.push_back(static_cast<int>(s._l) - 0.9);
    inputs.push_back(static_cast<int>(s._f) - 0.9);
    inputs.push_back(static_cast<int>(s._r) - 0.9);

    bool is_first = true;


    for (Layer const& l : _ann) {
        std::vector<float> outputs;

        if (is_first) {
            uint64_t i = 0;
            for (Neuron const& n : l._v) {
                outputs.push_back(n.eval(inputs, i));
                ++i;
            }
            is_first = false;
        } else {
            for (Neuron const& n : l._v) {
                outputs.push_back(n.eval(inputs));
            }
        }

        inputs = outputs;
    }

    uint64_t index = std::distance(inputs.begin(),
                                   std::max_element(inputs.begin(), inputs.end()));

    if (inputs[index] > 0) {
        Position p = _f.move_agent(index);
        switch (p) {
        case FOOD:
            ++_food_counter;
            break;
        case POISSON:
            ++_poisson_counter;
            break;
        default:
            break;
        }
    }
    // else do nothing
}

float Individual::evaluate_fitness()
{
    _food_counter = 0;
    _poisson_counter = 0;
    if (Settings::inst()->_seed == 0) {
        _f = Flatland();
    } else if (Settings::inst()->_seed < 10 ) {
        _f = Flatland(Settings::inst()->_current_gen / 10);
    } else {
        std::uniform_int_distribution<uint64_t> rnd_int(0, 0xFFFFFFFFFFFFFFFF);
        _f = Flatland(rnd_int(Settings::inst()->_randomness_source));
    }

    for (uint64_t i = 0; i < 60; ++i) {
        eval_step();
    }

    const float food_weight = 1;
    const float poisson_weight = -5;

    float _fitness = ((_food_counter * food_weight) + (_poisson_counter * poisson_weight)) / (60 * food_weight);
    return _fitness;
}

void Individual::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, 0xFFFFFFFFFFFFFFFF);
    std::uniform_real_distribution<float> rnd_flt(-1, 1);

    Layer l = _ann[rnd_int(Settings::inst()->_randomness_source) % _ann.size()];
    Neuron n = l._v[rnd_int(Settings::inst()->_randomness_source) % l._v.size()];
    n._w[rnd_int(Settings::inst()->_randomness_source) % n._w.size()]
            = rnd_flt(Settings::inst()->_randomness_source);
}

// TODO, but I don't expect to use mutation, it's too difficult
std::unique_ptr<Individual> Individual::cross_over(const std::unique_ptr<Individual> &in) const
{
    std::cerr << "Mutation is not implemented yet" << std::endl;
    return std::unique_ptr<Individual>(new Individual());
}

std::unique_ptr<Individual> Individual::get_copy() const
{
    return (std::unique_ptr<Individual>(new Individual(_ann)));
}

void Individual::print() const
{
    std::cout << "Individual:" << std::endl;
    for (auto const& l : _ann) {
        for (auto const& n : l._v) {
            for (auto const& w : n._w) {
                std::cout << w << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "Eaten: food: " << _food_counter << " poisson: " << _poisson_counter << std::endl;
}
