#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>
#include <vector>

#include "Verify.h"

#include "InputInstance.h"
#include "Certificate.h"

#include <iostream>

using namespace std;

TEST_CASE(
    "Verify test #1",
    "[Verify]")
{
    // Automata A accepts only words of format "b(a^n)"
    constexpr long statesA = 3;
    constexpr long alphabetSize = 2;
    vector<vector<long>> transA(statesA, vector<long>(alphabetSize));
    transA[0][0] = 2;
    transA[0][1] = 1;
    transA[1][0] = 1;
    transA[1][1] = 2;
    transA[2][0] = 2;
    transA[2][1] = 2;
    vector<long> acceptingA = {1};
    constexpr long initalStateA = 0;

    // Automata B accepts only words of format "a^n"
    constexpr long statesB = 2;
    vector<vector<long>> transB(statesB, vector<long>(alphabetSize));
    transB[0][0] = 0;
    transB[0][1] = 1;
    transB[1][0] = 1;
    transB[1][1] = 1;
    vector<long> acceptingB = {0};
    constexpr long initialStateB = 0;

    const vector<vector<long>> componentsA = {{1}};
    const vector<vector<long>> componentsB = {{0}};

    const vector<vector<long>> pathsA = {{0}};
    const vector<vector<long>> pathsB = {{0}};

    const DFA dfaA(transA, acceptingA, initalStateA);

    InputInstanceBasic inpA(dfaA, componentsA, pathsA);

    SolverResult resB{
        .trans = transB,
        .origin = initialStateB,
        .acceptingStates = acceptingB,
        .dagNodes = componentsB,
        .covers = {{0}},
        .coveringPaths = {{0}}};

    Certificate cert{
        .guessed = resB,
        .given = inpA};

    const auto res = Verify::checkCertificate(cert);

    if (res.has_value())
        FAIL(res.value());
}

