#ifndef SETTINGS_H
#define SETTINGS_H

#include <cmath>
#include <iostream>
#include <fstream>

//singleton settings
class Settings {
    Settings() {}
    Settings(const Settings &);
    Settings& operator =(const Settings&);
    static Settings* instance;

    double _centering_weight = 1;
    double _repealing_weight = 1;
    double _alignment_weight = 1;

    double _repealing_perimeter = 70;
    double _alignment_perimeter = 250;

    double _vel_limit = 20;

    double _sim_speed = 20;

    size_t _indiv_count = 20;
    size_t _obsticles_count = 0;

    size_t _world_size = 1024;

public:
    static Settings* inst() {
        if (!instance) {
            instance = new Settings;
        }
        return instance;
    }

    void print_to_file(const std::string& filename) const
    {
        std::ofstream file;
        file.open(filename);
        file << "Boid settings:" << std::endl;
        file << "Weight to center: " << _centering_weight << std::endl;
        file << "Weight of repealing: " << _repealing_weight << std::endl;
        file << "Repealing perimeter: " << _repealing_perimeter << std::endl;
        file << "Weight of alignment: " << _alignment_weight << std::endl;
        file << "Alignment perimeter: " << _alignment_perimeter << std::endl;
        file << "Velocity limit: " << _vel_limit << std::endl;
        file << "Sim speed: " << _sim_speed << std::endl;

    }

    double weight_first() const
    {
        return _centering_weight;
    }

    void setWeight_first(double weight_first)
    {
        _centering_weight = weight_first;
    }

    double weight_second() const
    {
        return _repealing_weight;
    }

    void setWeight_second(double weight_second)
    {
        _repealing_weight = weight_second;
    }

    double weight_third() const
    {
        return _alignment_weight;
    }

    void setWeight_third(double weight_third)
    {
        _alignment_weight = weight_third;
    }

    double vel_limit() const
    {
        return _vel_limit;
    }

    void setVel_limit(double vel_limit)
    {
        _vel_limit = vel_limit;
    }

    double repealing_perimeter() const
    {
        return _repealing_perimeter;
    }

    void setRepealing_perimeter(double boid_perimeter)
    {
        _repealing_perimeter = boid_perimeter;
    }

    double alignment_perimeter() const
    {
        return _alignment_perimeter;
    }

    void setAlignment_perimeter(double alignment_perimeter)
    {
        _alignment_perimeter = alignment_perimeter;
    }

    double sim_speed() const
    {
        return _sim_speed;
    }

    void setSim_speed(double sim_speed)
    {
        _sim_speed = sim_speed;
    }

    size_t indiv_count() const
    {
        return _indiv_count;
    }

    void setIndiv_count(size_t indiv_count)
    {
        _indiv_count = indiv_count;
    }

    size_t obsticles_count() const
    {
        return _obsticles_count;
    }

    void setObsticles_count(size_t obsticles_count)
    {
        _obsticles_count = obsticles_count;
    }

    size_t world_size() const
    {
        return _world_size;
    }

    void setWorld_size(const size_t &world_size)
    {
        _world_size = world_size;
    }
};

#endif // SETTINGS_H


