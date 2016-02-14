#ifndef SETTINGS_H
#define SETTINGS_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <random>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//singleton settings
class Settings {
    Settings() {}
    Settings(const Settings &);
    Settings& operator =(const Settings&);
    static Settings* _i;

    uint64_t _individual_count = 20;
    uint64_t _replacement_size = 12;

    float _mutation_probability = 0.05;
    float _crossover_probability = 0.5;

    bool _crossover_position_random = false;

    uint64_t _one_max_vector_size = 40;

    std::mt19937 _randomness_source; //little messy this class purpose

public:
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
        _replacement_size = jsontree.get<uint64_t>("population.replacement_size");

        _mutation_probability = jsontree.get<uint64_t>("evolution.mutation_probability");
        _crossover_probability = jsontree.get<uint64_t>("evolution.crossover_probability");
        _crossover_position_random = jsontree.get<bool>("evolution.crossover_position_random");

        _one_max_vector_size = jsontree.get<uint64_t>("one_max.vector_size");

        _randomness_source.seed(jsontree.get<uint64_t>("seed"));
    }

    void print_to_file(const std::string& filename) const
    {
        std::ofstream file;
        file.open(filename);
        file << "Individuals count:" << _individual_count << std::endl;
    }


    double individual_count() const
    {
        return _individual_count;
    }

    float mutation_probability() const
    {
        return _mutation_probability;
    }

    float crossover_probability() const
    {
        return _crossover_probability;
    }

    uint64_t replacement_size() const
    {
        return _replacement_size;
    }

    bool crossover_position_random() const
    {
        return _crossover_position_random;
    }

    std::mt19937 randomness_source() const
    {
        return _randomness_source;
    }

    uint64_t one_max_vector_size() const
    {
        return _one_max_vector_size;
    }
};

#endif // SETTINGS_H


