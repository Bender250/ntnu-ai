#include "evolution.h"

void Evolution::update_stats(const Stats &current)
{
    _global_best.min = std::max(_global_best.min, current.min);
    _global_best.max = std::max(_global_best.max, current.max);
    _global_best.average = std::max(_global_best.average, current.average);

    _global_worst.min = std::min(_global_worst.min, current.min);
    _global_worst.max = std::min(_global_worst.max, current.max);
    _global_worst.average = std::min(_global_worst.average, current.average);

    _global_average.min += current.min;
    _global_average.max += current.max;
    _global_average.average += current.average;

}

void Evolution::finalize_stats()
{
    _global_average.min /= _p.getCurrent_gen();
    _global_average.max /= _p.getCurrent_gen();
    _global_average.average /= _p.getCurrent_gen();
}

void Evolution::print_fitnesses() const
{
    Settings::inst()->_log << "Global fitness max:" << std::endl;
    std::cout << "Global fitness max:" << std::endl;

    Settings::inst()->_log << "Fitness min:" << _global_best.min << std::endl;
    std::cout << "Fitness min:" << _global_best.min << std::endl;

    Settings::inst()->_log << "Fitness max:" << _global_best.max << std::endl;
    std::cout << "Fitness max:" << _global_best.max << std::endl;

    Settings::inst()->_log << "Fitness avg:" << _global_best.average << std::endl;
    std::cout << "Fitness avg:" << _global_best.average << std::endl;


    Settings::inst()->_log << "Global fitness min:" << std::endl;
    std::cout << "Global fitness min:" << std::endl;

    Settings::inst()->_log << "Fitness min:" << _global_worst.min << std::endl;
    std::cout << "Fitness min:" << _global_worst.min << std::endl;

    Settings::inst()->_log << "Fitness max:" << _global_worst.max << std::endl;
    std::cout << "Fitness max:" << _global_worst.max << std::endl;

    Settings::inst()->_log << "Fitness avg:" << _global_worst.average << std::endl;
    std::cout << "Fitness avg:" << _global_worst.average << std::endl;


    Settings::inst()->_log << "Global fitness avg:" << std::endl;
    std::cout << "Global fitness avg:" << std::endl;

    Settings::inst()->_log << "Fitness min:" << _global_average.min << std::endl;
    std::cout << "Fitness min:" << _global_average.min << std::endl;

    Settings::inst()->_log << "Fitness max:" << _global_average.max << std::endl;
    std::cout << "Fitness max:" << _global_average.max << std::endl;

    Settings::inst()->_log << "Fitness avg:" << _global_average.average << std::endl;
    std::cout << "Fitness avg:" << _global_average.average << std::endl;
}

void Evolution::run()
{
    Stats current;
    if (Settings::inst()->_stop_by_gen) {
        for (uint64_t gen = 0; gen < Settings::inst()->_generations; ++gen) {
            current = _p.evaluate();
            update_stats(current);
            _p.log();
        }
    }
    else {
        bool is_fitnes_below = true;
        while (is_fitnes_below) {
            current = _p.evaluate();
            update_stats(current);
            is_fitnes_below = (current.max < Settings::inst()->_fitness);
            _p.log();
        }
    }
    finalize_stats();
    _p.print_final_fitness();
    print_fitnesses();
    _p.print_final_population();
}
