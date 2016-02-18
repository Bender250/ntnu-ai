#include "population.h"

void Population::fitness_testing()
{
    _current.max = 0;
    _current.min = 1;
    _current.average = 0;
    for (auto &indiv : _genome) {
        float f = indiv->evaluate_fitness();
        _current.max = std::max(f, _current.max);
        _current.min = std::min(f, _current.min);
        _current.average += f;
    }
    _current.average /= _genome.size();
}

void Population::adult_selection()
{
    switch (Settings::inst()->_adult_sel_strat) {
    case FULL_GEN_REPLACE:
        adult_selection_full_gen_replace();
        break;
    case OVER_PRODUCTION:
        adult_selection_over_production();
        break;
    case GENERATIONAL_MIXING:
        adult_selection_generational_mixing();
        break;
    default:
        std::cerr << "no adult strategy selected: " << Settings::inst()->_adult_sel_strat << std::endl;
        exit(1);
    }

}

void Population::adult_selection_full_gen_replace()
{
    for (uint64_t i = 0; i < _genome.size(); ++i) {
        _genome[i] = std::move(_children[i]);
    }
}

void Population::adult_selection_over_production()
{
    std::shuffle(_children.begin(), _children.end(), Settings::inst()->_randomness_source);
    for (uint64_t i = 0; i < _genome.size(); ++i) {
        _genome[i] = std::move(_children[i]);
    }
}

void Population::adult_selection_generational_mixing()
{
    std::shuffle(_genome.begin(), _genome.end(), Settings::inst()->_randomness_source);
    uint64_t preserve = Settings::inst()->_parent_preserve_count;
    for (uint64_t i = preserve; i < _genome.size(); ++i) {
        _genome[i] = std::move(_children[i-preserve]);
    }
    for (uint64_t i = preserve; i < _children.size(); ++i) {
        _genome.push_back(std::move(_children[i]));
    }
    std::shuffle(_genome.begin(), _genome.end(), Settings::inst()->_randomness_source);
    while (_genome.size() > Settings::inst()->_individual_count) {
        _genome.pop_back();
    }
}

void Population::parent_selection()
{
    // TODO: get fitness and update stats - needed in functions
    switch (Settings::inst()->_parent_sel_strat) {
    case FITNESS_PROPORTIONATE:
        parent_selection_fitness_proportional();
        break;
    case SIGMA_SCALING:
        parent_selection_sigma_scaling();
        break;
    case TOURNAMENT:
        parent_selection_tournament();
        break;
    case BOLTZMANN:
        parent_selection_boltzmann();
        break;
    case RANK:
        parent_selection_rank();
        break;
    case DETERMINISTIC_UNIFORM:
        parent_selection_deterministic_uniform();
        break;
    case STOCHASTIC_UNIFORM:
        parent_selection_stochastic_uniform();
        break;
    default:
        std::cerr << "no parent strategy selected: " << Settings::inst()->_parent_sel_strat << std::endl;
        exit(1);
    }

}

void Population::parent_selection_fitness_proportional()
{
    std::vector<float> stacked_fitness;
    for (const auto &i : _genome) {
        stacked_fitness.push_back(i->getFitness());
    }
    for (uint64_t i = 1; i < stacked_fitness.size(); ++i) {
        stacked_fitness[i] += stacked_fitness[i-1];
    }
    std::uniform_real_distribution<float> rnd_float(0, stacked_fitness[stacked_fitness.size() - 1]);
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
        float r = rnd_float(Settings::inst()->_randomness_source);
        uint64_t par = 0;
        while (par < stacked_fitness.size() && r < stacked_fitness[par])
            ++par;
        _parents.push_back(par);
    }
}

float Population::get_sigma_scaling(uint64_t i, float sig) const {
    return (1 + (_genome[i]->getFitness() + _current.average) / (2 * sig));
}

float Population::get_sigma() const {
    float v = 0;
    for (auto const &i : _genome) {
        v += std::pow(i->getFitness() - _current.average, 2);
    }
    return v / _genome.size();
}

void Population::parent_selection_sigma_scaling()
{
    std::vector<float> stacked_fitness;
    float sig = get_sigma();

    stacked_fitness.push_back(get_sigma_scaling(0, sig));
    for (uint64_t i = 1; i < _genome.size(); ++i) {
        stacked_fitness.push_back(get_sigma_scaling(i, sig) + stacked_fitness[i-1]);
    }
    std::uniform_real_distribution<float> rnd_float(0, stacked_fitness[stacked_fitness.size() - 1]);
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
        float r = rnd_float(Settings::inst()->_randomness_source);
        uint64_t par = 0;
        while (par < stacked_fitness.size() && r < stacked_fitness[par])
            ++par;
        _parents.push_back(par);
    }
}

void Population::parent_selection_tournament()
{

}

void Population::parent_selection_boltzmann()
{

}

void Population::parent_selection_rank()
{

}

void Population::parent_selection_deterministic_uniform()
{

}

void Population::parent_selection_stochastic_uniform()
{

}

void Population::reproduction()
{

}

Stats Population::evaluate()
{

}

void Population::log() const
{

}

void Population::print_final_fitness() const
{

}

Population::Population()
{
    for (uint64_t i = 0; i < Settings::inst()->_individual_count; ++i) {
        if (Settings::inst()->_project == ONE_MAX) {
            _genome.push_back(std::unique_ptr<One_max_individual>(new One_max_individual()));
        }

    }
}
