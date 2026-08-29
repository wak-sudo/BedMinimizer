#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "AugSccGraph.h"
#include "PartialDFA.h"

using namespace std;

TEST_CASE("AugSccGraph - paths", "[AugSccGraph]")
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

    struct
    {
        const vector<vector<long>> paths = {
            {{0}, {0, 1}, {0, 1, 2}, {0, 1, 3}, {0, 2}, {0, 3}, {1}, {1, 2}, {1, 3}, {2}, {3}}};
        const vector<vector<bool>> IsPrefix =
            {
                {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}};
    } VALID;

    DFA dfa(trans, accepting, initial);

    const AugSccGraph augGraph(dfa);

    SECTION("Check paths")
    {
        const size_t pathsCount = augGraph.paths().size();
        CHECK(pathsCount == VALID.paths.size());

        vector<vector<long>> res;
        res.reserve(pathsCount);
        for (size_t i = 0; i < pathsCount; i++)
        {
            const auto path = augGraph.paths().at(i);
            res.push_back(path);
        }

        sort(res.begin(), res.end());

        CHECK(res == VALID.paths);
    }
}