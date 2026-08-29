#pragma once

#include <random>
#include <vector>
#include <set>

struct GeneratedDag
{
    std::vector<std::vector<long>> edges;
    std::vector<std::vector<long>> layers;
    std::vector<long> statesPerNode;
};

class RandomDagGenerator
{
public:
    RandomDagGenerator();

    GeneratedDag generate(
        long numberOfStates,
        long numberOfNodes,
        long depth);

private:
    std::mt19937 rng_;

    std::vector<std::vector<long>> generateLayers(
        long numberOfNodes,
        long depth);

    std::vector<std::set<long>> generateEdges(
        const std::vector<std::vector<long>> &layers,
        long numberOfStates);

    std::vector<long> calculateStatesPerNode(
        const std::vector<std::vector<long>> &edges,
        long numberOfStates);

    std::vector<std::vector<long>> convertSetVectorToVecVector(const std::vector<std::set<long>> &input);
};