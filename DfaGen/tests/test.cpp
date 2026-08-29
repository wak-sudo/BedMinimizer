#include <catch2/catch_test_macros.hpp>

#include "RandomGenerator.h"
#include "AugSccGraphBasic.h"
#include "RandomGeneratorExp.h"

#include "Utility.h"

#include <random>

TEST_CASE("RandomGenerator generates DFA with expected properties",
          "[DfaGen1]")
{
    constexpr long attempts = 5;

    std::mt19937 rng{std::random_device{}()};

    std::uniform_int_distribution<long> nodesDist(2, 20);

    for (long i = 0; i < attempts; ++i)
    {
        const long expNodes = nodesDist(rng);

        std::uniform_int_distribution<long> statesDist(expNodes, 100);
        const long expStates = statesDist(rng);

        // Depth cannot be greater than the number of DAG nodes.
        std::uniform_int_distribution<long> depthDist(2, expNodes);
        const long expDepth = depthDist(rng);
        try
        {
            const auto dfa =
                RandomGenerator::generate(
                    expStates,
                    expNodes,
                    expDepth,
                    2);

            AugSccGraphBasic dag(dfa);

            const long depth = dag.getDepth();
            const long dagNodesCount = dag.dagNodesCount();
            const long statesCount = dag.getOriginalDfa().statesCount();

            INFO("Attempt: " << i);
            INFO("Expected depth: " << expDepth);
            INFO("Expected DAG nodes: " << expNodes);
            INFO("Expected states: " << expStates);

            CHECK(depth == expDepth);
            CHECK(dagNodesCount == expNodes);
            CHECK(statesCount == expStates);
            CHECK(Utility::IsConnected(dag.getOriginalDfa()));
        }
        catch (const RandomGeneratorExp &e)
        {
            // sometimes we are not able to generate a valid instance.
        }
    }
}

TEST_CASE("RandomGenerator generates DFA with expected properties, depth one",
          "[DfaGen2]")
{
    constexpr long attempts = 5;

    std::mt19937 rng{std::random_device{}()};

    const long expDepth = 1;
    const long expNodes = 1;

    for (long i = 0; i < attempts; ++i)
    {
        std::uniform_int_distribution<long> statesDist(expNodes+1, 100);
        const long expStates = statesDist(rng);

        try
        {
            const auto dfa =
                RandomGenerator::generate(
                    expStates,
                    expNodes,
                    expDepth,
                    2);

            AugSccGraphBasic dag(dfa);

            const long depth = dag.getDepth();
            const long dagNodesCount = dag.dagNodesCount();
            const long statesCount = dag.getOriginalDfa().statesCount();

            INFO("Attempt: " << i);
            INFO("Expected depth: " << expDepth);
            INFO("Expected DAG nodes: " << expNodes);
            INFO("Expected states: " << expStates);

            CHECK(depth == expDepth);
            CHECK(dagNodesCount == expNodes);
            CHECK(statesCount == expStates);
            CHECK(Utility::IsConnected(dag.getOriginalDfa()));
        }
        catch (const RandomGeneratorExp &e)
        {
            // sometimes we are not able to generate a valid instance.
        }
    }
}