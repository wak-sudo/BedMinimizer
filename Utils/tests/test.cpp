#include <catch2/catch_test_macros.hpp>
#include "Utility.h"

#include <vector>

using namespace std;

TEST_CASE("SharedUtils - isPrefix", "[SharedUtils]")
{
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

    const long pathsCount = VALID.paths.size();

    SECTION("Check prefix")
    {
        for (long p = 0; p < pathsCount; p++)
        {
            for (long q = 0; q < pathsCount; q++)
            {
                const auto p1 = VALID.paths.at(p);
                const auto p2 = VALID.paths.at(q);

                long id1 = -1, id2 = -1;
                for (long k = 0; k < pathsCount; k++)
                {
                    if (p1 == VALID.paths[k])
                        id1 = k;
                    if (p2 == VALID.paths[k])
                        id2 = k;
                }
                REQUIRE(id1 != -1);
                REQUIRE(id2 != -1);
                CHECK(Utility::isPrefix(VALID.paths.at(p), VALID.paths.at(q)) == VALID.IsPrefix.at(id1).at(id2));
            }
        }
    }
}