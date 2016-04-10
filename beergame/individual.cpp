#include "individual.h"

Individual::Individual() : _fitness(0)
{
    Layer hidden;
    for (uint64_t i = 0; i < 2; ++i) {
        hidden._v.push_back(Neuron(5));
    }
    _ann.push_back(hidden);

    Layer last;
    for (uint64_t i = 0; i < 2; ++i) {
        last._v.push_back(Neuron(2));
    }
    _ann.push_back(last);
}

void Individual::eval_step()
{
    std::vector<float> inputs = _l.get_above();


    for (Layer& l : _ann) {
        std::vector<float> outputs;

        for (Neuron& n : l._v) {
            outputs.push_back(n.eval(inputs));
        }

        inputs = outputs;
    }

    Direction d = (inputs[0] > inputs[1]) ? LEFT : RIGHT;
    uint64_t dist; // TODO

    int collected = _l.step(d, dist);
    if (collected != NOTHING) {
        ++_object_counter[collected + 8];
    }
}

float Individual::evaluate_fitness()
{
    _fitness = 0;
    for (uint64_t step = 0; step < 600; ++step) {
        eval_step();
    }

    // calc fitness
    return _fitness;
}

void Individual::mutate()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, 0xFFFFFFFFFFFFFFFF);
    std::uniform_real_distribution<float> rnd_flt(-1, 1);

    Layer l = _ann[rnd_int(Settings::inst()->_randomness_source) % _ann.size()];
    Neuron n = l._v[rnd_int(Settings::inst()->_randomness_source) % l._v.size()];

    n.mutate();
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
            n.print();
            std::cout << std::endl;
        }
    }
    // text as well?
}
