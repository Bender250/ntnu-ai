#include "population.h"


Population::Population() : _current_gen(0)
{
    for (uint64_t i = 0; i < Settings::inst()->_individual_count; ++i) {
        _genome.push_back(std::unique_ptr<Individual>(new Individual()));
    }
}

uint64_t Population::getCurrent_gen() const
{
    return _current_gen;
}

void Population::fitness_testing()
{
    _current.max = -INFINITY;
    _current.min = 1;
    _current.average = 0;
    for (auto &indiv : _genome) {
        float f = indiv->evaluate_fitness();
        _current.max = std::max(f, _current.max);
        _current.min = std::min(f, _current.min);
        _current.average += f;
    }

    //TODO maybe add fitness evaluation of children too
    for (auto &indiv : _children) {
        /*float f = */indiv->evaluate_fitness(); //just evaluate, not log stat
        /*_current.max = std::max(f, _current.max);
        _current.min = std::min(f, _current.min);
        _current.average += f;*/
    }
    _current.average /= _genome.size();
}

void Population::adult_selection()
{
    //TODO maybe clean the _children vector
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
    case FULL_GEN_REPLACE_MOD:
        adult_selection_full_gen_replace_mod();
        break;
    case OVER_PRODUCTION_MOD:
        adult_selection_over_production_mod();
        break;
    case GENERATIONAL_MIXING_MOD:
        adult_selection_generational_mixing_mod();
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

//with elitism -> we need at least population of size 2
void Population::adult_selection_full_gen_replace_mod()
{
    std::sort(_genome.begin(), _genome.end(), _decreasing_comparator);
    for (uint64_t i = 2; i < _genome.size(); ++i) {
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

void Population::adult_selection_over_production_mod()
{
    std::sort(_children.begin(), _children.end(), _decreasing_comparator);
    for (uint64_t i = 0; i < _genome.size(); ++i) {
        _genome[i] = std::move(_children[i]);
    }
}

void Population::adult_selection_generational_mixing()
{
    std::shuffle(_genome.begin(), _genome.end(), Settings::inst()->_randomness_source);
    uint64_t preserve = Settings::inst()->_adult_preserve_count;
    for (uint64_t i = preserve; i < _genome.size(); ++i) {
        _genome[i] = std::move(_children[i-preserve]);
    }
    for (uint64_t i = Settings::inst()->_individual_count - preserve; i < _children.size(); ++i) {
        _genome.push_back(std::move(_children[i]));
    }
    std::shuffle(_genome.begin(), _genome.end(), Settings::inst()->_randomness_source);
    while (_genome.size() > Settings::inst()->_individual_count) {
        _genome.pop_back();
    }
}

void Population::adult_selection_generational_mixing_mod()
{
    std::sort(_children.begin(), _children.end(), _decreasing_comparator);
    std::sort(_genome.begin(), _genome.end(), _decreasing_comparator);
    uint64_t preserve = Settings::inst()->_adult_preserve_count;
    for (uint64_t i = preserve; i < _genome.size(); ++i) {
        _genome[i] = std::move(_children[i-preserve]);
    }
}

void Population::adult_selection_deterministic_mixing()
{
    for (uint64_t i = 0; i < _children.size(); ++i) {
        _genome.push_back(std::move(_children[i]));
    }

    std::sort(_genome.begin(), _genome.end(), _decreasing_comparator);

    while (_genome.size() > Settings::inst()->_individual_count) {
        _genome.pop_back();
    }
}

void Population::parent_selection()
{
    _parents.erase(_parents.begin(), _parents.end());

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
        while (par < stacked_fitness.size() && r > stacked_fitness[par])
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

uint64_t Population::get_best_id() const {
    uint64_t pos = 0;
    for (uint64_t i = 1; i < _genome.size(); ++i) {
        pos = (_genome[pos]->getFitness() < _genome[i]->getFitness()) ? i : pos;
    }
    return pos;
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
        while (par < stacked_fitness.size() && r > stacked_fitness[par])
            ++par;
        _parents.push_back(par);
    }
}

void Population::parent_selection_tournament()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genome.size()-1);
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
        uint64_t par = rnd_int(Settings::inst()->_randomness_source);

        // to tournament is nominated_tournament_k individuals, the best is choosed as parent
        for (uint64_t j = 0; j < Settings::inst()->_tournament_k; ++j) {
            uint64_t alt = rnd_int(Settings::inst()->_randomness_source);
            if (_genome[alt]->getFitness() > _genome[par]->getFitness())
                par = alt;
        }

        _parents.push_back(par);
    }
}

float Population::get_boltzmann(uint64_t i) const
{
    float res = std::exp(_genome[i]->getFitness() / (_current_gen + 1));
    return (res / std::exp(_current.average / (_current_gen + 1)));
}

void Population::parent_selection_boltzmann()
{
    std::vector<float> stacked_fitness;

    stacked_fitness.push_back(get_boltzmann(0));
    for (uint64_t i = 1; i < _genome.size(); ++i) {
        stacked_fitness.push_back(get_boltzmann(i) + stacked_fitness[i-1]);
    }

    std::uniform_real_distribution<float> rnd_float(0, stacked_fitness[stacked_fitness.size() - 1]);
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
        float r = rnd_float(Settings::inst()->_randomness_source);
        uint64_t par = 0;
        while (par < stacked_fitness.size() && r > stacked_fitness[par])
            ++par;
        _parents.push_back(par);
    }
}

void Population::parent_selection_rank()
{
    std::vector<uint64_t> stacked_fitness;

    std::sort(_genome.begin(), _genome.end(), _increasing_comparator);
    while (_genome[_genome.size() - 1]->getFitness() < 0 && _genome.size() > 2) {
        _genome.pop_back();
    }

    stacked_fitness.push_back(1);
    for (uint64_t i = 1; i < _genome.size(); ++i) {
        stacked_fitness.push_back(i + stacked_fitness[i - 1] + 1);
    }

    std::uniform_int_distribution<uint64_t> rnd_int(0, stacked_fitness[stacked_fitness.size() - 1]);
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
        uint64_t r = rnd_int(Settings::inst()->_randomness_source);
        uint64_t par = 0;
        while (par < stacked_fitness.size() && r > stacked_fitness[par])
            ++par;
        _parents.push_back(par);
    }
}

/*void Population::parent_selection_rank()
{
    std::vector<uint64_t> stacked_fitness;

    std::sort(_genome.begin(), _genome.end(), _increasing_comparator);
    stacked_fitness.push_back(1);
    for (uint64_t i = 1; i < _genome.size(); ++i) {
        stacked_fitness.push_back(i + stacked_fitness[i - 1] + 1);
    }

    std::uniform_int_distribution<uint64_t> rnd_int(0, stacked_fitness[stacked_fitness.size() - 1]);
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
        uint64_t r = rnd_int(Settings::inst()->_randomness_source);
        uint64_t par = 0;
        while (par < stacked_fitness.size() && r > stacked_fitness[par])
            ++par;
        _parents.push_back(par);
    }
}*/

void Population::parent_selection_deterministic_uniform()
{
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
         // I assume, that this is runned with _parent_count beeing multiple of _genome.size
        _parents.push_back(i % _genome.size());
    }
}

void Population::parent_selection_stochastic_uniform()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genome.size() - 1);
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
        _parents.push_back(rnd_int(Settings::inst()->_randomness_source));
    }
}

void Population::reproduction()
{
    std::uniform_int_distribution<uint64_t> rnd_int(0, _parents.size() - 1);
    std::bernoulli_distribution crossover(Settings::inst()->_crossover_probability);
    std::bernoulli_distribution mutate(Settings::inst()->_mutation_probability);

    _children.erase(_children.begin(), _children.end()); //_children from previous run

    for (uint64_t i = 0; i < Settings::inst()->_children_count; ++i) {
        std::unique_ptr<Individual> offspring;
        if (crossover(Settings::inst()->_randomness_source))
            offspring = _genome[
                            _parents[
                                rnd_int(Settings::inst()->_randomness_source)]]
                        ->cross_over(
                            _genome[
                                _parents[
                                    rnd_int(Settings::inst()->_randomness_source)]]);
        else
            offspring = _genome[
                            _parents[
                                rnd_int(Settings::inst()->_randomness_source)]]
                        ->get_copy();

        while (mutate(Settings::inst()->_randomness_source)) {
            offspring->mutate();
        }
        _children.push_back(move(offspring));
    }
}

Stats Population::evaluate()
{
    ++_current_gen;

    fitness_testing(); //so individuals have set current fitness
    if (_current_gen != 1)
        adult_selection();
    parent_selection();
    reproduction();
    return _current;
}

void Population::log() const
{
    Settings::inst()->_log << std::endl << _current_gen;
    Settings::inst()->_log << ";" << _current.min;
    Settings::inst()->_log << ";" << _current.max;
    Settings::inst()->_log << ";" << _current.average;
    Settings::inst()->_log << ";" << get_sigma();
}

void Population::print_final_fitness() const
{
    Settings::inst()->_log << "Final results:" << std::endl;
    std::cout << "Final results:" << std::endl;

    Settings::inst()->_log << "Fitness min:" << _current.min << std::endl;
    std::cout << "Fitness min:" << _current.min << std::endl;

    Settings::inst()->_log << "Fitness max:" << _current.max << std::endl;
    std::cout << "Fitness max:" << _current.max << std::endl;

    Settings::inst()->_log << "Fitness avg:" << _current.average << std::endl;
    std::cout << "Fitness avg:" << _current.average << std::endl;
}

std::unique_ptr<Individual> Population::getBestIndividual() const
{
    return _genome[0]->get_copy();
}
