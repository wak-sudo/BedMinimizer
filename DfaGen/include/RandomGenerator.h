#pragma once

#include <random>

#include "DFA.h"

namespace RandomGenerator
{
    DFA generate(long numberOfStates, long numberOfNodes, long depth, long alphabetSize);
};