#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <algorithm>

#include "AugSccGraph.h"
#include "PartialDFA.h"

struct ExpectedValues
{
    long automatStatesCount;
    long alphabetSize;
    std::vector<std::vector<long>> components;
    long sinkId;
    std::vector<std::vector<long>> sameComponent;
    long depth;
};

void assertMetadata(AugSccGraphBasic &graph, const ExpectedValues &expected)
{
    CHECK(graph.automataStatesCount() == expected.automatStatesCount);
    CHECK(graph.alphabetSize() == expected.alphabetSize);
    CHECK(graph.getDepth() == expected.depth);
    CHECK(graph.globalSinkId() == expected.sinkId);
}

void assertComponents(AugSccGraphBasic &graph, const ExpectedValues &expected)
{
    const long validComponentsCount = expected.components.size();
    CHECK(graph.dagNodesCount() == validComponentsCount);

    std::vector<std::vector<long>> res;
    res.reserve(validComponentsCount);

    for (auto nodesVec : graph.sccNodes())
    {
        std::sort(nodesVec.begin(), nodesVec.end());
        res.push_back(std::move(nodesVec));
    }

    std::sort(res.begin(), res.end());
    CHECK(res == expected.components);
}

void assertSameComponent(const AugSccGraphBasic &graph, const ExpectedValues &expected)
{
    for (long state = 0; state < expected.automatStatesCount; state++)
    {
        for (long alphabet = 0; alphabet < expected.alphabetSize; alphabet++)
        {
            CHECK(graph.nextState(state, alphabet) == expected.sameComponent[state][alphabet]);
        }
    }
}

void verifyAugSccGraph(AugSccGraphBasic &graph, const ExpectedValues &expected)
{
    SECTION("Check basic metadata getters")
    {
        assertMetadata(graph, expected);
    }

    SECTION("Check components")
    {
        assertComponents(graph, expected);
    }

    SECTION("Same Component")
    {
        assertSameComponent(graph, expected);
    }
}

TEST_CASE("AugSccGraph - Basic properties and methods", "[AugSccGraph]")
{
    std::vector<std::vector<long>> trans = {
        {1, 2},
        {0, 1},
        {3, 4},
        {2, 7},
        {5, 6},
        {5, 5},
        {6, 6},
        {7, 7}};

    std::vector<long> accepting = {5, 6};
    const long initial = 0;

    ExpectedValues expected{
        .automatStatesCount = 8,
        .alphabetSize = 2,
        .components = {{0, 1}, {2, 3}, {5}, {6}},
        .sinkId = 8,
        .sameComponent = {
            {1, 8},
            {0, 1},
            {3, 8},
            {2, 8},
            {8, 8},
            {5, 5},
            {6, 6},
            {8, 8}},
        .depth = 3};

    DFA dfa(trans, accepting, initial);
    AugSccGraphBasic augGraph(dfa);

    verifyAugSccGraph(augGraph, expected);
}

TEST_CASE("AugSccGraph - minimal #1", "[AugSccGraph]")
{
    std::vector<std::vector<long>> trans = {
        {0}};

    std::vector<long> accepting = {0};
    const long initial = 0;

    ExpectedValues expected{
        .automatStatesCount = 1,
        .alphabetSize = 1,
        .components = {{0}},
        .sinkId = 1,
        .sameComponent = {
            {0}},
        .depth = 1};

    DFA dfa(trans, accepting, initial);
    AugSccGraphBasic augGraph(dfa);

    verifyAugSccGraph(augGraph, expected);
}

TEST_CASE("AugSccGraph - minimal error", "[AugSccGraph]")
{
    std::vector<std::vector<long>> trans = {
        {0}};

    std::vector<long> accepting = {};
    const long initial = 0;

    ExpectedValues expected{
        .automatStatesCount = 1,
        .alphabetSize = 1,
        .components = {},
        .sinkId = 1,
        .sameComponent = {
            {1}},
        .depth = 0};

    DFA dfa(trans, accepting, initial);
    CHECK_THROWS(AugSccGraphBasic(dfa));
}

TEST_CASE("AugSccGraph - depth", "[AugSccGraph]")
{
    std::vector<std::vector<long>> trans = {
        {1, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 1, 1},
        {3, 2, 2, 2, 2, 2},
        {4, 3, 3, 3, 3, 3},
        {4, 4, 4, 4, 4, 4}};

    std::vector<long> accepting = {4};
    const long initial = 0;

    ExpectedValues expected{
        .automatStatesCount = 5,
        .alphabetSize = 6,
        .components = {{0}, {1}, {2}, {3}, {4}},
        .sinkId = 5,
        .sameComponent = {
            {5, 0, 0, 0, 0, 0},
            {5, 1, 1, 1, 1, 1},
            {5, 2, 2, 2, 2, 2},
            {5, 3, 3, 3, 3, 3},
            {4, 4, 4, 4, 4, 4},
        },
        .depth = 5};

    DFA dfa(trans, accepting, initial);
    AugSccGraphBasic augGraph(dfa);

    verifyAugSccGraph(augGraph, expected);
}