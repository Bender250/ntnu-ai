#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "individual.h"

class Population
{
private:
    std::vector<Individual> genome;
public:
    Population();
};

#endif // POPULATION_H
