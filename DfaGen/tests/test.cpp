#include <catch2/catch_test_macros.hpp>

#include "RandomGenerator.h"
#include "AugSccGraphBasic.h"
#include "RandomGeneratorExp.h"

#include "Utility.h"

#include <random>

TEST_CASE("RandomGenerator generates DFA with expected properties",
          "[DfaGen1]")
{
    constexpr long testCasesCount = 50;
    constexpr long attemptsPerTest = 5;

    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<long> nodesDist(2, 20);
    std::uniform_int_distribution<long> alphabetDist(2, 10);

    for (long caseNumber = 0; caseNumber < testCasesCount; caseNumber++)
    {
        for (long i = 0; i < attemptsPerTest; ++i)
        {
            const long expNodes = nodesDist(rng);
            const long alphabetSize = alphabetDist(rng);

            std::uniform_int_distribution<long> statesDist(expNodes + 1, 100);
            std::uniform_int_distribution<long> depthDist(2, expNodes);

            const long expStates = statesDist(rng);

            // Depth cannot be greater than the number of DAG nodes.

            const long expDepth = depthDist(rng);
            try
            {
                const auto dfa = RandomGenerator::generate(expStates, expNodes, expDepth, alphabetSize);

                AugSccGraphBasic dag(dfa);

                const long depth = dag.getDepth();
                const long dagNodesCount = dag.dagNodesCount();
                const long statesCount = dag.getOriginalDfa().statesCount();

                INFO("Expected depth: " << expDepth << " real: " << depth);
                INFO("Expected DAG nodes: " << expNodes << " real: " << dagNodesCount);
                INFO("Expected states: " << expStates << " real: " << statesCount);

                CHECK(depth == expDepth);
                CHECK(dagNodesCount == expNodes);
                CHECK(statesCount == expStates);
                CHECK(Utility::IsConnected(dfa));
            }
            catch (const RandomGeneratorExp &e)
            {
                // sometimes we are not able to generate a valid instance.
            }
        }
    }
}

TEST_CASE("RandomGenerator generates DFA with expected properties, depth one",
          "[DfaGen2]")
{
    constexpr long testCasesCount = 5;
    constexpr long attempts = 5;

    std::mt19937 rng{std::random_device{}()};

    const long expDepth = 1;
    const long expNodes = 1;

    std::uniform_int_distribution<long> statesDist(expNodes + 1, 100);
    std::uniform_int_distribution<long> alphabetDist(2, 10);

    for (long caseNumber = 0; caseNumber < testCasesCount; caseNumber++)
    {
        for (long i = 0; i < attempts; ++i)
        {
            const long expStates = statesDist(rng);
            const long alphabetSize = alphabetDist(rng);

            try
            {
                const auto dfa = RandomGenerator::generate(expStates, expNodes, expDepth, alphabetSize);

                AugSccGraphBasic dag(dfa);

                const long depth = dag.getDepth();
                const long dagNodesCount = dag.dagNodesCount();
                const long statesCount = dag.getOriginalDfa().statesCount();

                INFO("Expected depth: " << expDepth << " real: " << depth);
                INFO("Expected DAG nodes: " << expNodes << " real: " << dagNodesCount);
                INFO("Expected states: " << expStates << " real: " << statesCount);

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
}