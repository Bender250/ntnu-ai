#include "population.h"

void Population::fitness_testing()
{
    _current.max = 0;
    _current.min = 1;
    _current.average = 0;
    for (auto &indiv : _genome) {
        float f = indiv.evaluate_fitness();
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

}

void Population::adult_selection_over_production()
{

}

void Population::adult_selection_generational_mixing()
{

}

void Population::parent_selection()
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
            One_max_individual o = One_max_individual();
            _genome.push_back(o);
        }

    }
}
