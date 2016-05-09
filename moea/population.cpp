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

void Population::rank_evaluate()
{
    // parent cost sort:
    std::sort(_genome.begin(), _genome.end(), _increasing_cost_comparator); // sort by cost, then compare with dist

    _genome[0]->setRank(1); // leading
    uint64_t leading = 0;
    uint64_t next = 0;
    while (leading + 1 < _genome.size()) {
        next = 0;
        for (uint64_t i = leading + 1; i < _genome.size(); ++i) {
            if (_genome[leading]->getRank() > _genome[i]->getRank()) { // skip already well assigned rank
                continue;
            }
            if (_genome[leading]->dist_fitness() > _genome[i]->dist_fitness()) {
                _genome[i]->setRank(_genome[leading]->getRank());
                leading = i; // neccesary? - moves to another solutions with same rank, seems ok
            } else {
                _genome[i]->setRank(_genome[leading]->getRank() + 1);
                if (next == 0) {
                    next = i;
                }
            }
        }
        if (next == 0) {
            //if (leading + 1 != _genome.size())
            //    std::cerr << "Should not happen" << std::endl; // I dont understand this, but it works, do not touch
            break;
        }
        leading = next;
    }
}

// TODO: randomly gives CD = infinity to few more individuals
void Population::crowding_distance_evaluate()
{
    // parent crowding distance evaluate

    //std::sort(_genome.begin(), _genome.end(), _increasing_cost_comparator); // should not be needed
    //   stable for holding cost sorted -> primary sorted by rank, secondary by cost
    std::stable_sort(_genome.begin(), _genome.end(), _increasing_rank_comparator);

    // parents are already sorted by rank
    _genome[0]->setCrowding_distance(INFINITY);
    for (uint64_t i = 1; i < _genome.size() - 1; ++i) {
        if (_genome[i]->getRank() != _genome[i+1]->getRank()) {
            _genome[i]->setCrowding_distance(INFINITY);
            _genome[i+1]->setCrowding_distance(INFINITY);
            // ++i; // no, this could skip another rank layer
        } else {
            if (_genome[i]->getCrowding_distance() == INFINITY) {
                continue;
            }
            float res = 0;
            res += std::abs(_genome[i]->dist_fitness() - _genome[i-1]->dist_fitness())
                    * std::abs(_genome[i]->cost_fitness() - _genome[i-1]->cost_fitness());
            res += std::abs(_genome[i]->dist_fitness() - _genome[i+1]->dist_fitness())
                    * std::abs(_genome[i]->cost_fitness() - _genome[i+1]->cost_fitness());

            _genome[i]->setCrowding_distance(res);
        }
    }
    _genome[_genome.size() - 1]->setCrowding_distance(INFINITY);
}

void Population::rank_evaluate_children()
{
    // parent cost sort:
    std::sort(_children.begin(), _children.end(), _increasing_cost_comparator); // sort by cost, then compare with dist

    _children[0]->setRank(1); // leading
    uint64_t leading = 0;
    uint64_t next = 0;
    while (leading + 1 < _children.size()) {
        next = 0;
        for (uint64_t i = leading + 1; i < _children.size(); ++i) {
            if (_children[leading]->getRank() > _children[i]->getRank()) { // skip already well assigned rank
                continue;
            }
            if (_children[leading]->dist_fitness() > _children[i]->dist_fitness()) {
                _children[i]->setRank(_children[leading]->getRank());
                leading = i; // neccesary? - moves to another solutions with same rank, seems ok
            } else {
                _children[i]->setRank(_children[leading]->getRank() + 1);
                if (next == 0) {
                    next = i;
                }
            }
        }
        if (next == 0) {
            //if (leading + 1 != _children.size())
            //    std::cerr << "Should not happen" << std::endl; // I dont understand this, but it works, do not touch
            break;
        }
        leading = next;
    }
}

void Population::crowding_distance_evaluate_children()
{
    // parent crowding distance evaluate

    //std::sort(_children.begin(), _children.end(), _increasing_cost_comparator); // should not be needed
    //   stable for holding cost sorted -> primary sorted by rank, secondary by cost
    std::stable_sort(_children.begin(), _children.end(), _increasing_rank_comparator);

    // parents are already sorted by rank
    _children[0]->setCrowding_distance(INFINITY);
    for (uint64_t i = 1; i < _children.size() - 1; ++i) {
        if (_children[i]->getRank() != _children[i+1]->getRank()) {
            _children[i]->setCrowding_distance(INFINITY);
            _children[i+1]->setCrowding_distance(INFINITY);
            // ++i; // no, this could skip another rank layer
        } else {
            if (_children[i]->getCrowding_distance() == INFINITY) {
                continue;
            }
            float res = 0;
            res += std::abs(_children[i]->dist_fitness() - _children[i-1]->dist_fitness())
                    * std::abs(_children[i]->cost_fitness() - _children[i-1]->cost_fitness());
            res += std::abs(_children[i]->dist_fitness() - _children[i+1]->dist_fitness())
                    * std::abs(_children[i]->cost_fitness() - _children[i+1]->cost_fitness());

            _children[i]->setCrowding_distance(res);
        }
    }
    _children[_children.size() - 1]->setCrowding_distance(INFINITY);
}

void Population::fitness_testing()
{
    for (auto &indiv : _genome) {
        /*Fitness f = */indiv->evaluate_fitness();
    }

    for (auto &indiv : _children) {
        indiv->evaluate_fitness();
    }

    // eval parents
    rank_evaluate();
    crowding_distance_evaluate();

    // eval children
    if (_current_gen > 1) {
        rank_evaluate_children();
        crowding_distance_evaluate_children();
    }
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
    std::sort(_genome.begin(), _genome.end(), _best_first_comparator);
    for (uint64_t i = 2; i < _genome.size(); ++i) { // keep 2 best individuals
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
    std::sort(_children.begin(), _children.end(), _best_first_comparator); // TODO: can I do this?
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
    std::sort(_children.begin(), _children.end(), _best_first_comparator); // TODO: can I do this?
    std::sort(_genome.begin(), _genome.end(), _best_first_comparator);
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

    std::sort(_genome.begin(), _genome.end(), _best_first_comparator);

    while (_genome.size() > Settings::inst()->_individual_count) {
        _genome.pop_back();
    }
}

void Population::parent_selection()
{
    _parents.erase(_parents.begin(), _parents.end());

    // TODO: change to algorithm from slides
    std::uniform_int_distribution<uint64_t> rnd_int(0, _genome.size()-1);
    for (uint64_t i = 0; i < Settings::inst()->_parent_count; ++i) {
        uint64_t par = rnd_int(Settings::inst()->_randomness_source);

        // to tournament is nominated_tournament_k individuals, the best is choosed as parent
        for (uint64_t j = 0; j < Settings::inst()->_tournament_k; ++j) {
            uint64_t alt = rnd_int(Settings::inst()->_randomness_source);
            if ( _genome[alt]->getRank() < _genome[par]->getRank() // lower rank is better
                    || (_genome[alt]->getRank() == _genome[par]->getRank() // or in case of same rank
                        && _genome[alt]->getCrowding_distance() > _genome[par]->getCrowding_distance()) ) // higher CD is better
                par = alt;
        }

        _parents.push_back(par);
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
        // debug
        if (!offspring->is_valid()) {
            std::cerr << "Invalid reproduction! Gen: " << _current_gen << std::endl;
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
    //return _current;
}

void Population::log() const
{
    Settings::inst()->_log << _current_gen;
    /*Settings::inst()->_log << ";" << _current.min;
    Settings::inst()->_log << ";" << _current.max;
    Settings::inst()->_log << ";" << _current.average;
    Settings::inst()->_log << ";" << get_sigma();
    Settings::inst()->_log << ";" << _genome[get_best_id()]->to_phenotype_string() << std::endl;*/
}

void Population::print_final_fitness() const
{
    Settings::inst()->_log << "Final results:" << std::endl;
    std::cout << "Final results:" << std::endl;

    /*Settings::inst()->_log << "Fitness min:" << _current.min << std::endl;
    std::cout << "Fitness min:" << _current.min << std::endl;

    Settings::inst()->_log << "Fitness max:" << _current.max << std::endl;
    std::cout << "Fitness max:" << _current.max << std::endl;

    Settings::inst()->_log << "Fitness avg:" << _current.average << std::endl;
    std::cout << "Fitness avg:" << _current.average << std::endl;*/
}

void Population::print_final_population(QCustomPlot *customPlot)
{
    Settings::inst()->_log << "Final population:" << std::endl;
    std::cout << "Final population:" << std::endl;
    fitness_testing();

    QVector<double> cost_pareto, dist_pareto;
    QVector<double> cost_other, dist_other;
    for (uint64_t i = 0; i < Settings::inst()->_individual_count; ++i) {
        std::cout << "c "   << _genome[i]->cost_fitness()
                  << ", d " << _genome[i]->dist_fitness()
                  << ", r " << _genome[i]->getRank()
                  << ", c " << _genome[i]->getCrowding_distance() << std::endl;
        if (_genome[i]->getRank() == 1) {
            cost_pareto.push_back(_genome[i]->cost_fitness());
            dist_pareto.push_back(_genome[i]->dist_fitness());
            _genome[i]->print_genome();
        } else {
            cost_other.push_back(_genome[i]->cost_fitness());
            dist_other.push_back(_genome[i]->dist_fitness());
        }
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QColor(0, 0, 0, 250)); // black
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(0)->setName("Cost/dist others");
    customPlot->graph(0)->setData(dist_other, cost_other);

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QColor(255, 0, 0, 250)); // red
    customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    customPlot->graph(1)->setName("Cost/dist pareto front");
    customPlot->graph(1)->setData(dist_pareto, cost_pareto);

    // give the axes some labels:
    customPlot->xAxis->setLabel("dist");
    customPlot->yAxis->setLabel("cost");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 350000);
    customPlot->yAxis->setRange(0, 3500);
    customPlot->replot();
}
