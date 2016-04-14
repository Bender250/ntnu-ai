#ifndef SETTINGS_H
#define SETTINGS_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <random>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

enum Adult_sel_strat {
    FULL_GEN_REPLACE = 1, OVER_PRODUCTION = 2, GENERATIONAL_MIXING = 3,
    FULL_GEN_REPLACE_MOD = 4, OVER_PRODUCTION_MOD = 5, GENERATIONAL_MIXING_MOD = 6
};

enum Parent_sel_strat {
    FITNESS_PROPORTIONATE = 1, SIGMA_SCALING = 2, TOURNAMENT = 3,
    BOLTZMANN = 4, RANK = 5, DETERMINISTIC_UNIFORM = 6, STOCHASTIC_UNIFORM = 7
};

//singleton settings
class Settings {
    Settings() {}
    Settings(const Settings &);
    Settings& operator =(const Settings&);
    static Settings* _i;

public:

    /////////////////////////////////////// EA
    uint64_t _individual_count = 20;
    uint64_t _parent_count = 20;

    float _mutation_probability = 0.05;
    float _crossover_probability = 0.5;

    bool _crossover_position_random = false;

    bool _stop_by_gen = true;
    uint64_t _generations = 100;
    float _fitness = 0.9;

    uint64_t _rep_per_gen = 1;

    uint64_t _current_gen = 0;

    Adult_sel_strat _adult_sel_strat = FULL_GEN_REPLACE;
    uint64_t _children_count = 20;
    uint64_t _adult_preserve_count = 10;

    Parent_sel_strat _parent_sel_strat = FITNESS_PROPORTIONATE;
    uint64_t _tournament_k = 8;
    /////////////////////////////////////// \EA

    /////////////////////////////////////// ANN
    uint64_t _internal_layers_count = 1;
    std::vector<uint64_t> _internal_layers_sizes;
    /////////////////////////////////////// \ANN

    /////////////////////////////////////// OTHER
    std::mt19937 _randomness_source; //little messy this class purpose
    std::ofstream _log;
    uint64_t _seed = 0;
    bool _final_regenerate = true;

    static Settings* inst() {
        if (!_i) {
            _i = new Settings;
        }
        return _i;
    }

    void load(const std::string& filename) {
        boost::property_tree::ptree jsontree;
        boost::property_tree::read_json(filename, jsontree);

        _individual_count = jsontree.get<uint64_t>("population.size");
        _parent_count = jsontree.get<uint64_t>("population.parent_count");

        _mutation_probability = jsontree.get<float>("evolution.mutation_probability");
        _crossover_probability = jsontree.get<float>("evolution.crossover_probability");
        _crossover_position_random = jsontree.get<bool>("evolution.crossover_position_random");

        _adult_sel_strat = static_cast<Adult_sel_strat>(jsontree.get<uint64_t>("evolution.adult_sel_strat"));
        _children_count = jsontree.get<uint64_t>("evolution.adult_sel_strat_settings.children_count");
        _adult_preserve_count = jsontree.get<uint64_t>("evolution.adult_sel_strat_settings.adult_preserve_count");
        _parent_sel_strat = static_cast<Parent_sel_strat>(jsontree.get<uint64_t>("evolution.parent_sel_strat"));
        if (_parent_sel_strat == TOURNAMENT) {
            _tournament_k = jsontree.get<uint64_t>("evolution.parent_sel_strat_settings.tournament_k");
        }

        _stop_by_gen = jsontree.get<bool>("evolution.stop_by_gen");
        _generations = jsontree.get<uint64_t>("evolution.stop.generations");
        _fitness = jsontree.get<float>("evolution.stop.fitness");

        _rep_per_gen = jsontree.get<uint64_t>("rep_per_gen");

        _internal_layers_count = jsontree.get<uint64_t>("network.internal_layers_count");
        switch (_internal_layers_count) {
        case 1:
            _internal_layers_sizes.push_back(jsontree.get<uint64_t>("network.0"));
            break;
        default:
            break;
        }

        _seed = jsontree.get<uint64_t>("seed");
        _final_regenerate = jsontree.get<bool>("final_regenerate");
        _randomness_source.seed(_seed);
        _log.open("../log.txt");
    }
};

#endif // SETTINGS_H

