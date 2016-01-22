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

    double _weight_first = 1;
    double _weight_second = 1;
    double _weight_third = 1;

    double _vel_limit = 20;

    double _boid_perimeter = 50;

    double _sim_speed = 20;

    int _indiv_count = 20;

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
        file << "Weight to center: " << _weight_first << std::endl;
        file << "Weight of repealing: " << _weight_second << std::endl;
        file << "Weight of alignment: " << _weight_third << std::endl;
        file << "Velocity limit: " << _vel_limit << std::endl;
        file << "Boid perimeter: " << _boid_perimeter << std::endl;
        file << "Sim speed: " << _sim_speed << std::endl;

    }

    double weight_first() const
    {
        return _weight_first;
    }

    void setWeight_first(double weight_first)
    {
        _weight_first = weight_first;
    }

    double weight_second() const
    {
        return _weight_second;
    }

    void setWeight_second(double weight_second)
    {
        _weight_second = weight_second;
    }

    double weight_third() const
    {
        return _weight_third;
    }

    void setWeight_third(double weight_third)
    {
        _weight_third = weight_third;
    }

    double vel_limit() const
    {
        return _vel_limit;
    }

    void setVel_limit(double vel_limit)
    {
        _vel_limit = vel_limit;
    }

    double boid_perimeter() const
    {
        return _boid_perimeter;
    }

    void setBoid_perimeter(double boid_perimeter)
    {
        _boid_perimeter = boid_perimeter;
    }

    double sim_speed() const
    {
        return _sim_speed;
    }

    void setSim_speed(double sim_speed)
    {
        _sim_speed = sim_speed;
    }

    double indiv_count() const
    {
        return _indiv_count;
    }

    void setIndiv_count(double indiv_count)
    {
        _indiv_count = indiv_count;
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



