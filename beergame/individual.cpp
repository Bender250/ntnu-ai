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

std::vector<Layer> Individual::getAnn() const
{
    return _ann;
}

void Individual::eval_step()
{
    std::vector<float> inputs = _l.get_above();


    for (Layer& l : _ann) {
        std::vector<float> outputs;

        for (uint64_t i = 0; i < 2; ++i) {
            outputs.push_back(l._v[i].eval(inputs, l._v[i ^ 0x1].prev()));
        }

        inputs = outputs;
    }

    Direction d = (inputs[0] > inputs[1]) ? LEFT : RIGHT;
    uint64_t dist = 4 * std::abs(inputs[0] - inputs[1]) / std::abs(inputs[0] + inputs[1]);

    dist = std::min(dist, (uint64_t) 4);

    int collected = NOTHING;

    if (Settings::inst()->_pull && dist == 0) {
            collected = _l.pull();
    } else {
        collected = _l.step(d, dist);
    }

    if (collected != NOTHING) {
        ++_object_counter[collected + 6];
    }
}

float Individual::evaluate_fitness()
{
    _fitness = 0;

    for (uint64_t i = 0; i < Settings::inst()->_rep_per_gen; ++i) {
        _l = Beergame_land(Settings::inst()->_current_gen * Settings::inst()->_rep_per_gen + i);
        for (uint64_t& o : _object_counter) {
            o = 0;
        }

        for (uint64_t step = 0; step < 600; ++step) {
            eval_step();
        }

        if (Settings::inst()->_pull) {

            // calc fitness
            const float penalty = 100;
            _fitness += 1         * _object_counter[0];
            _fitness += 1         * _object_counter[1];
            _fitness -= 1*penalty * _object_counter[2];
            _fitness -= 1*penalty * _object_counter[3];
            _fitness -= 1*penalty * _object_counter[4];
            _fitness -= 1*penalty * _object_counter[5];
            _fitness += 1*penalty * _object_counter[6]; //partially above
            _fitness += 1        * _object_counter[7];
            _fitness += 1         * _object_counter[8];
            _fitness += 1         * _object_counter[9];
            _fitness += 1         * _object_counter[10];
            _fitness -= 5*penalty * _object_counter[11]; // impossible
            _fitness -= 6*penalty * _object_counter[12]; // impossible
        } else {

            // calc fitness
            const float penalty =0;
            _fitness += 1         * _object_counter[0];
            _fitness += 1         * _object_counter[1];
            _fitness -= 1*penalty * _object_counter[2];
            _fitness -= 1*penalty * _object_counter[3];
            _fitness -= 1*penalty * _object_counter[4];
            _fitness -= 1*penalty * _object_counter[5];
            _fitness -= 40 * _object_counter[6]; //partially above
            _fitness += 1        * _object_counter[7];
            _fitness += 1         * _object_counter[8];
            _fitness += 1         * _object_counter[9];
            _fitness += 1         * _object_counter[10];
            _fitness -= 5*penalty * _object_counter[11]; // impossible
            _fitness -= 6*penalty * _object_counter[12]; // impossible
        }
    }

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
    std::vector<Layer> a = {_ann[0], in->getAnn()[1] };
    return std::unique_ptr<Individual>(new Individual(a));
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
    std::cout << "Collected:" << std::endl;
    std::cout << "-6: " << _object_counter[0] << std::endl;
    std::cout << "-5: " << _object_counter[1] << std::endl;
    std::cout << "-4: " << _object_counter[2] << std::endl;
    std::cout << "-3: " << _object_counter[3] << std::endl;
    std::cout << "-2: " << _object_counter[4] << std::endl;
    std::cout << "-1: " << _object_counter[5] << std::endl;
    std::cout << "partially fallen: " << _object_counter[6] << std::endl;
    std::cout << "+1: " << _object_counter[7] << std::endl;
    std::cout << "+2: " << _object_counter[8] << std::endl;
    std::cout << "+3: " << _object_counter[9] << std::endl;
    std::cout << "+4: " << _object_counter[10] << std::endl;
    std::cout << "+5: " << _object_counter[11] << std::endl;
    std::cout << "+6: " << _object_counter[12] << std::endl;
}
