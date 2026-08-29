#pragma once

#include <vector>

struct SolverResult
{
    std::vector<std::vector<long>> trans;
    long origin;
    std::vector<long> acceptingStates;
    std::vector<std::vector<long>> dagNodes;
    std::vector<std::vector<long>> covers;
    std::vector<std::vector<long>> coveringPaths;
};