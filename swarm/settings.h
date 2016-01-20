#ifndef SETTINGS_H
#define SETTINGS_H

#include <cmath>

//singleton settings
class Settings {
    Settings() {}
    Settings(const Settings &);
    Settings& operator =(const Settings&);
    static Settings* instance;

    double _weight_first;
    double _weight_second;
    double _weight_third;

    double _vel_limit;

    double _boid_perimeter;

    double _sim_speed;

    int _indiv_count;

public:
    static Settings* inst() {
        if (!instance) {
            instance = new Settings;
        }
        return instance;
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
};

#endif // SETTINGS_H

