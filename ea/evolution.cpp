#include "evolution.h"

void Evolution::update_stats(const Stats &current, uint64_t gen)
{

}

void Evolution::run()
{
    uint64_t actual_generations_count = 0;
    Stats current;
    if (Settings::inst()->_stop_by_gen) {
        for (uint64_t gen = 0; gen < Settings::inst()->_generations; ++gen) {
            current = _p.evaluate();
            update_stats(current, gen);
            _p.log();
        }
    }
    else {
        bool is_fitnes_below = true;
        while (is_fitnes_below) {
            ++actual_generations_count;
            current = _p.evaluate();
            update_stats(current, actual_generations_count);
            is_fitnes_below = (current.average > Settings::inst()->_fitness);
            _p.log();
        }
    }
    _p.print_final_fitness();
}
