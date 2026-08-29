#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <ostream>
#include <sstream>

#include "DimacsWriter.h"
#include "CnfAst.h"

using namespace Cnf;

TEST_CASE("Dimacs Writer", "[Dimacs Writer]")
{
    const Formula f = AND(
        {
            OR({VAR("p1"), VAR("p2"), VAR("p4")}),
            OR({VAR("p4"), VAR("p1"), VAR("p3")}),
            OR({VAR("p2"), VAR("p3"), VAR("p1")}),
        });

    SECTION("DimacsWriter::ToString")
    {
        std::string output = DimacsWriter::ToString(f).first;
        std::istringstream ss(output);

        std::string line;
        std::vector<std::string> tokens;

        while (std::getline(ss, line))
        {
            // c for "comment"
            if (line.empty() || line[0] == 'c')
                continue;

            std::istringstream lineStream(line);
            std::string token;
            while (lineStream >> token)
            {
                tokens.push_back(token);
            }
        }

        struct
        {
            const long variablesCount = 4;
            const long clauseCount = 3;
        } Expected;

        REQUIRE(tokens.size() >= 16);

        CHECK(tokens[0] == "p");
        CHECK(tokens[1] == "cnf");

        const long receivedVarsCount = std::stol(tokens[2]);
        CHECK(receivedVarsCount == Expected.variablesCount);

        const long receivedClauseCount = std::stol(tokens[3]);
        CHECK(receivedClauseCount == Expected.clauseCount);

        // p1 on positions: 4, 9, 14
        const bool P1EQ = (tokens[4] == tokens[9] && tokens[9] == tokens[14]);
        CHECK(P1EQ);

        // p2 on positions: 5, 12
        const bool P2EQ = (tokens[5] == tokens[12]);
        CHECK(P2EQ);

        // p3 on positions: 10, 13
        const bool P3EQ = (tokens[10] == tokens[13]);
        CHECK(P3EQ);

        // p4 on positions: 6, 8
        const bool P4EQ = (tokens[6] == tokens[8]);
        CHECK(P4EQ);

        // p1 is unique
        const bool P1NEQ = (tokens[4] != tokens[5] && tokens[4] != tokens[10] && tokens[4] != tokens[6]);
        CHECK(P1NEQ);

        // p2 is unique
        const bool P2NEQ = (tokens[5] != tokens[10] && tokens[5] != tokens[6]);
        CHECK(P2NEQ);

        // p3 is unique
        const bool P3NEQ = (tokens[10] != tokens[6]);
        CHECK(P3NEQ);

        // Check 0s for clause end.
        const bool zeroes = (tokens[7] == "0" && tokens[11] == "0" && tokens[15] == "0");
        CHECK(zeroes);

        // 0 Is unique.
        const bool nZeros = (tokens[4] != "0" && tokens[5] != "0" && tokens[10] != "0" && tokens[6] != "0");
        CHECK(nZeros);
    }
}