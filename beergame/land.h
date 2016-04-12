#ifndef FLATLAND_H
#define FLATLAND_H

#include <vector>

#include "settings.h"

enum Direction {
    LEFT, RIGHT
};

struct Falling_object {
    uint64_t x, y, l;
};

static const std::size_t X_SIZE = 30;
static const std::size_t Y_SIZE = 15;

static const int NOTHING = 0xFFFFFFFF;


class Beergame_land
{
private:
    Falling_object _f;
    uint64_t _x;
    std::mt19937 _randomness;
    std::uniform_int_distribution<uint64_t> _gen;

    bool is_object_above(uint64_t const& x) const {
        if (_f.x + _f.l >= X_SIZE) {
            if (x < _f.x) {
                return (x < ((_f.x + _f.l) % X_SIZE));
            } else {
                return true;
            }
        } else {
            if (x < _f.x) {
                return false;
            } else {
                return (x < (_f.x + _f.l));
            }
        }
    }

    bool is_pad_below(uint64_t const& x) const {
        if (_x > 25) {
            if (x > 25) {
                return true;
            } else {
                return (((_x + 5) % X_SIZE) >= x);
            }
        } else {
            return ((_x <= x) && (_x+5 >= x));
        }
    }

    void generate_new_object() {
        std::uniform_int_distribution<uint64_t> gen(1, 6);
        _f.x = _gen(_randomness) % X_SIZE;
        _f.y = Y_SIZE - 1;
        _f.l = (_gen(_randomness) % 6) + 1;
    }

    int resolve_falled_abject() {
        uint64_t object_len = _f.l;

        bool catched = (object_len < 5);
        bool partially_above = false;

        for (uint64_t i = 0; i < 5; ++i) {
            catched &= is_object_above((_x + i) % X_SIZE);
            partially_above |= is_object_above((_x + i) % X_SIZE);
        }

        /*for (uint64_t i = 0; i < _f.l; ++i) {
            catched &= is_pad_below((_f.x + i) % X_SIZE);
            partially_above |= is_pad_below((_f.x + i) % X_SIZE);
        }*/

        generate_new_object();

        if (catched) {
            return object_len;
        } else if (partially_above) {
            return 0;
        } else {
            return -object_len;
        }
    }

public:
    Beergame_land(uint64_t seed = 0) : _randomness(seed), _gen() {
        _f.x = _gen(_randomness) % X_SIZE;
        _x = _gen(_randomness) % X_SIZE;
        _f.y = Y_SIZE - 1;
        _f.l = (_gen(_randomness) % 6) + 1;
    }

    std::vector<float> get_above() const {
        std::vector<float> out;

        for (uint64_t i = 0; i < 5; ++i) {
            out.push_back( (is_object_above(_x + i)) ? 1.0 : 0.0);
        }

        return out;
    }

    int step(Direction const& d, uint64_t const& dist) {
        int res = NOTHING;

        if (d == LEFT) {
            _x = (_x + (X_SIZE - dist)) % X_SIZE;
        } else {
            _x = (_x + dist) % X_SIZE;
        }

        if (_f.y == 0) {
            res = resolve_falled_abject();
        }

        --_f.y;
        return res;
    }
    uint64_t getX() const {
        return _x;
    }
    Falling_object getF() const {
        return _f;
    }
};

#endif // FLATLAND_H
