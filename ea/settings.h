#ifndef SETTINGS_H
#define SETTINGS_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <random>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

enum Project {
    ONE_MAX = 1, LOLZ = 2, SEQUENCES = 3
};

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

    Project _project;

    uint64_t _individual_count = 20;
    uint64_t _parent_count = 20;

    float _mutation_probability = 0.05;
    float _crossover_probability = 0.5;

    bool _crossover_position_random = false;

    bool _stop_by_gen = true;
    uint64_t _generations = 100;
    float _fitness = 0.9;

    Adult_sel_strat _adult_sel_strat = FULL_GEN_REPLACE;
    uint64_t _children_count = 20;
    uint64_t _adult_preserve_count = 10;

    Parent_sel_strat _parent_sel_strat = FITNESS_PROPORTIONATE;
    uint64_t _tournament_k = 8;

    uint64_t _one_max_vector_size = 40;
    uint64_t _one_max_target_vector = 40;

    uint64_t _lolz_vector_size = 64;
    uint64_t _lolz_z = 40;

    bool _sequences_local = false;
    uint64_t _sequences_symbols_count = 4;
    uint64_t _sequences_expected_length = 4;
    uint64_t _sequences_d = 2;

    std::mt19937 _randomness_source; //little messy this class purpose
    std::ofstream _log;

    static Settings* inst() {
        if (!_i) {
            _i = new Settings;
        }
        return _i;
    }

    void load(const std::string& filename) {
        boost::property_tree::ptree jsontree;
        boost::property_tree::read_json(filename, jsontree);

        _project = static_cast<Project>(jsontree.get<int>("project"));

        switch (_project) {
        case ONE_MAX:
            _one_max_vector_size = jsontree.get<uint64_t>("project_settings.one_max.vector_size");
            _one_max_target_vector = jsontree.get<uint64_t>("project_settings.one_max.target_vector");
            break;
        case LOLZ:
            _lolz_vector_size = jsontree.get<uint64_t>("project_settings.lolz.vector_size");
            _lolz_z = jsontree.get<uint64_t>("project_settings.lolz.z");
            break;
        case SEQUENCES:
            _sequences_local = jsontree.get<bool>("project_settings.sequences.just_local");
            _sequences_symbols_count = jsontree.get<uint64_t>("project_settings.sequences.symbols_count");
            _sequences_expected_length = jsontree.get<uint64_t>("project_settings.sequences.expected_length");
            _sequences_d = jsontree.get<uint64_t>("project_settings.sequences.d");
            break;
        default:
            std::cerr << "bad project type: " << _project << std::endl;
            exit(1);
            break;
        }

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


        _randomness_source.seed(jsontree.get<uint64_t>("seed"));
        _log.open("../log.txt");
    }
};

#endif // SETTINGS_H

