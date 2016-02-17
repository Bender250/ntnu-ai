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
    FULL_GEN_REPLACE = 1, OVER_PRODUCTION = 2, GENERATIONAL_MIXING = 3
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
    uint64_t _replacement_size = 12;

    float _mutation_probability = 0.05;
    float _crossover_probability = 0.5;

    bool _crossover_position_random = false;

    bool _stop_by_gen = true;
    uint64_t _generations = 100;
    float _fitness = 0.9;

    Adult_sel_strat _adult_sel_strat = FULL_GEN_REPLACE;

    uint64_t _one_max_vector_size = 40;

    std::mt19937 _randomness_source; //little messy this class purpose

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

            break;
        case LOLZ:

            break;
        case SEQUENCES:

            break;
        default:
            std::cerr << "bad project type: " << _project << std::endl;
            exit(1);
            break;
        }

        _individual_count = jsontree.get<uint64_t>("population.size");
        _replacement_size = jsontree.get<uint64_t>("population.replacement_size");

        _mutation_probability = jsontree.get<float>("evolution.mutation_probability");
        _crossover_probability = jsontree.get<float>("evolution.crossover_probability");
        _crossover_position_random = jsontree.get<bool>("evolution.crossover_position_random");

        _adult_sel_strat = static_cast<Adult_sel_strat>(jsontree.get<uint64_t>("evolution.adult_sel_strat"));

        _stop_by_gen = jsontree.get<bool>("evolution.stop_by_gen");
        _generations = jsontree.get<uint64_t>("evolution.stop.generations");
        _fitness = jsontree.get<float>("evolution.stop.fitness");

        _randomness_source.seed(jsontree.get<uint64_t>("seed"));
    }

    void print_to_file(const std::string& filename) const
    {
        std::ofstream file;
        file.open(filename);
        file << "Individuals count:" << _individual_count << std::endl;
    }
};

#endif // SETTINGS_H

