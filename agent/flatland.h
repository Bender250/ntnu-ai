#ifndef FLATLAND_H
#define FLATLAND_H

#include <array>

#include "settings.h"

enum Position {
    EMPTY, FOOD, POISSON
};

enum Orientation {
    UP, RIGHT, BOTTOM, LEFT
};

struct Coordinates {
    std::size_t _x, _y;
    Orientation _o;
};

struct Surrounding {
    Position _l, _f, _r;
};


static const std::size_t SIZE = 10;

class Flatland
{
private:
    std::array<std::array<Position, SIZE>, SIZE> _f;
    Coordinates _c;

public:
    Flatland(int seed = 0) {
        std::mt19937 randomness(seed);
        std::bernoulli_distribution third(1.0/3.0);
        for (std::size_t i = 0; i < SIZE; ++i) {
            for (std::size_t j = 0; j < SIZE; ++j) {
                if (third(randomness)) {
                    _f[j][i] = FOOD;
                } else  if (third(randomness)) {
                    _f[j][i] = POISSON;
                } else {
                    _f[j][i] = EMPTY;
                }
            }
        }

        std::uniform_int_distribution<uint64_t> rnd_int(0, 40);
        _c._o = static_cast<Orientation>(rnd_int(randomness) % 4);
        _c._x = rnd_int(randomness) % 10;
        _c._y = rnd_int(randomness) % 10;
    }

    Surrounding getSurrounding() const {
        Surrounding s;
        switch (_c._o) {
        case UP:
            s._l = _f[_c._y][(_c._x + 9) % SIZE]; // L  +9 = -1 in mod 10
            s._r = _f[_c._y][(_c._x + 1) % SIZE]; // R
            s._f = _f[(_c._y + 1) % SIZE][_c._x]; // U
            break;
        case RIGHT:
            s._l = _f[(_c._y + 1) % SIZE][_c._x]; // U
            s._r = _f[(_c._y + 9) % SIZE][_c._x]; // B
            s._f = _f[_c._y][(_c._x + 1) % SIZE]; // R
            break;
        case BOTTOM:
            s._l = _f[_c._y][(_c._x + 1) % SIZE]; // R
            s._r = _f[_c._y][(_c._x + 9) % SIZE]; // L
            s._f = _f[(_c._y + 9) % SIZE][_c._x]; // B
            break;
        case LEFT:
            s._l = _f[(_c._y + 9) % SIZE][_c._x]; // B
            s._r = _f[(_c._y + 1) % SIZE][_c._x]; // U
            s._f = _f[_c._y][(_c._x + 9) % SIZE]; // L
            break;
        default:
            break;
        }
        return s;
    }

    Position move_in_direction(Orientation const& d) {
        if (   (d == UP     && _c._o == BOTTOM)
            || (d == RIGHT  && _c._o == LEFT)
            || (d == BOTTOM && _c._o == UP)
            || (d == LEFT   && _c._o == RIGHT) ) {
            std::cerr << "Incorrect movement" << d << _c._o << std::endl;
        }

        _c._o = d;
        switch (d) {
        case UP:
            _c._y = (_c._y + 1) % 10;
            break;
        case RIGHT:
            _c._x = (_c._x + 1) % 10;
            break;
        case BOTTOM:
            _c._y = (_c._y + 9) % 10;
            break;
        case LEFT:
            _c._x = (_c._x + 9) % 10;
            break;
        default:
            break;
        }

        Position p = _f[_c._y][_c._y];

        _f[_c._y][_c._y] = EMPTY;

        return p;
    }

    Position move_agent(uint64_t const& i) {
        return move_in_direction(static_cast<Orientation>((_c._o + i + 3) % 4));
    }

    Position getPosition(uint64_t const& i, uint64_t const& j) const {
        return _f[i][j];
    }
};

#endif // FLATLAND_H
