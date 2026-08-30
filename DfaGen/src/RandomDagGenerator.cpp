#include "RandomDagGenerator.h"
#include "RandomGeneratorExp.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <set>

using namespace std;

RandomDagGenerator::RandomDagGenerator()
    : rng_(random_device{}())
{
}

GeneratedDag RandomDagGenerator::generate(
    long numberOfStates,
    long numberOfNodes,
    long depth)
{
    if (numberOfNodes > 1 && depth == 1)
        throw invalid_argument("With a depth set to 1 only a single node creates a valid instance.");

    if (numberOfStates <= 0)
        throw invalid_argument("Number of states must be positive.");

    if (numberOfNodes <= 0)
        throw invalid_argument("Number of nodes must be positive.");

    if (numberOfStates < numberOfNodes)
        throw invalid_argument("Number of states must be greater than the number of nodes.");

    if (depth <= 0 || depth > numberOfNodes)
        throw invalid_argument("Invalid depth.");

    const long maxAttempts = 100;
    for (long i = 0; i < maxAttempts; i++)
    {
        auto layers = generateLayers(numberOfNodes, depth);
        auto edges = convertSetVectorToVecVector(generateEdges(layers));

        auto statesPerNode = calculateStatesPerNode(edges, numberOfStates);

        if (statesPerNode.empty())
            continue;

        return GeneratedDag{
            .edges = move(edges),
            .layers = move(layers),
            .statesPerNode = move(statesPerNode)};
    }

    throw RandomGeneratorExp(
        "Failed to generate a valid DAG after " +
        to_string(maxAttempts) +
        " attempts.");
}

vector<vector<long>> RandomDagGenerator::generateLayers(
    long numberOfNodes,
    long depth)
{
    vector<vector<long>> layers(depth);

    // Root.
    layers.at(0).push_back(0);

    long nextNode = 1;
    long remainingNodes = numberOfNodes - 1;

    for (long layer = 1; layer < depth - 1; layer++)
    {
        const long remainingLayers = depth - layer;
        const long maxForThisLayer = remainingNodes - (remainingLayers - 1);

        uniform_int_distribution<long> dist(1, maxForThisLayer);

        const long count = dist(rng_);

        for (long i = 0; i < count; i++)
            layers.at(layer).push_back(nextNode++);

        remainingNodes -= count;
    }

    if (depth != 1)
    {
        for (long i = 0; i < remainingNodes; i++)
            layers.at(depth - 1).push_back(nextNode++);
    }

    return layers;
}

/*vector<set<long>> RandomDagGenerator::generateEdges(
    const vector<vector<long>> &layers,
    long numberOfStates)*/
vector<set<long>> RandomDagGenerator::generateEdges(
    const vector<vector<long>> &layers)
{
    const long numberOfNodes = accumulate(
        layers.begin(),
        layers.end(),
        0L,
        [](long sum, const auto &layer)
        {
            return sum + layer.size();
        });

    vector<set<long>> edges(numberOfNodes);

    for (size_t layer = 1; layer < layers.size(); layer++)
    {
        for (long to : layers.at(layer))
        {
            const long parentLayer = layer - 1;
            // Randomly select a vertex from the previous layer.
            uniform_int_distribution<long> parentDist(0, layers.at(parentLayer).size() - 1);

            const long from = layers.at(parentLayer).at(parentDist(rng_));

            edges.at(from).insert(to);
        }
    }

    // long statesLimit = numberOfStates - layers.size(); // heuristics.

    bernoulli_distribution addEdge(0.25);

    for (size_t fromLayer = 0; fromLayer < layers.size(); fromLayer++)
    {
        for (size_t toLayer = fromLayer + 1; toLayer < layers.size(); toLayer++)
        {
            for (long from : layers.at(fromLayer))
            {
                for (long to : layers.at(toLayer))
                {
                    const auto &outgoingEdges = edges.at(from);

                    if (outgoingEdges.contains(to))
                        continue;

                    if (addEdge(rng_))
                    {
                        edges.at(from).insert(to);
                        // statesLimit -= 2;
                        //  if(statesLimit <= 0)
                        //    return edges;
                    }
                }
            }
        }
    }

    return edges;
}

vector<long> RandomDagGenerator::calculateStatesPerNode(
    const vector<vector<long>> &edges,
    long numberOfStates)
{
    const long numberOfNodes = edges.size();

    vector<long> statesPerNode(numberOfNodes);

    long requiredStates = 0;

    for (long v = 0; v < numberOfNodes; v++)
    {
        statesPerNode.at(v) = edges.at(v).size() + 1;

        requiredStates += statesPerNode.at(v);
    }

    long remainingStates = numberOfStates - requiredStates;

    if (remainingStates < 0)
        return {};

    uniform_int_distribution<long> nodeDist(0, numberOfNodes - 1);

    for (long i = 0; i < remainingStates; i++)
        statesPerNode[nodeDist(rng_)]++;

    return statesPerNode;
}

vector<vector<long>> RandomDagGenerator::convertSetVectorToVecVector(const vector<set<long>> &input)
{
    vector<vector<long>> result;
    result.reserve(input.size());

    for (const auto &s : input)
    {
        result.emplace_back(s.begin(), s.end());
    }

    return result;
}