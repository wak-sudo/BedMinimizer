#pragma once

#include "RandomDagGenerator.h"

#include <random>
#include <vector>

#include "DFA.h"

class RandomDfaGenerator
{
public:
    RandomDfaGenerator(GeneratedDag dag, long alphabetSize);

    DFA generate();

private:
    GeneratedDag dag_;
    std::mt19937 rng_;
    std::vector<long> offsets_;
    long alphabetSize_;
    long globalSink_;
    long freeLetter_;
    long baseNumberOfStates_;

    static std::vector<long> calculateOffsets(const std::vector<long> &statesPerNode);

    std::vector<std::vector<std::vector<long>>> generateLocalTrans();

    void setTransitions(
        std::vector<std::vector<long>> &transFun);

    void setRandomFreeTransition(
        std::vector<std::vector<long>> &transFun,
        long node,
        long source);

    void setRandomDestination(
        std::vector<std::vector<long>> &transFun,
        long node,
        long source);

    void setAcceptingStates(
        std::vector<long> &acceptingStates);

    long generateInitialState();

    std::vector<std::vector<long>> randomTransFun(long n, long alphabetSize);

    void connectRandom(std::vector<std::vector<long>> &trans, long src, long dest);

    void shuffleLetters(std::vector<std::vector<long>>& trans);
};