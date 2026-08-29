#include <catch2/catch_test_macros.hpp>

#include <togasat.hpp>

#include <sstream>
#include <string>
#include <vector>

#include "BitVector.h"
#include "DimacsWriter.h"
#include "Tseytin.h"

using namespace Cnf;

bool isSatTogaSat(const std::string &dimacsStr)
{
    togasat::Solver solver;

    std::istringstream input(dimacsStr);
    std::string line;

    while (std::getline(input, line))
    {
        if (line.empty() || line[0] == 'c' || line[0] == 'p')
        {
            continue;
        }

        std::istringstream clauseStream(line);

        std::vector<int> clause;
        int lit;

        while (clauseStream >> lit)
        {
            if (lit == 0)
            {
                break;
            }

            clause.push_back(lit);
        }

        if (!clause.empty())
        {
            solver.addClause(clause);
        }
    }

    return solver.solve() == togasat::l_True;
}

Cnf::Formula fixVectorValueDimacs(
    const Logic::BitVector &X,
    unsigned long val)
{
    Cnf::Formula result;

    for (size_t i = 0; i < X.size(); ++i)
    {
        const bool bit = (val >> i) & 1;
        const Variable lit = X.bit(i);

        if (bit)
        {
            result &= lit;
        }
        else
            result &= Cnf::NOT(lit);
    }

    return result;
}

TEST_CASE(
    "BitVector encode_lt_const exhaustive test",
    "[bitvector][sat]")
{
    constexpr size_t NUM_BITS = 4;
    constexpr unsigned long MAX_VAL = (1UL << NUM_BITS) - 1;

    Logic::BitVector X("x", NUM_BITS);

    for (unsigned long C = 1; C <= MAX_VAL; ++C)
    {
        DYNAMIC_SECTION("Testing X < " << C)
        {
            const auto ltFormula = Logic::encode_lt_const(X, C);
            const auto subCnf = Tseytin::Transform(ltFormula);

            for (unsigned long val = 0; val < MAX_VAL; ++val)
            {
                const Cnf::Formula f = subCnf & fixVectorValueDimacs(X, val);
                const std::string dimacs = DimacsWriter::ToString(f).first;

                const bool expectedSat = val < C;
                const bool actualSat = isSatTogaSat(dimacs);

                INFO("For X = " << val << ", C = " << C);
                REQUIRE(actualSat == expectedSat);
            }
        }
    }
}

TEST_CASE(
    "BitVector encode_neq_const exhaustive test",
    "[bitvector][sat]")
{
    constexpr size_t NUM_BITS = 4;
    constexpr unsigned long MAX_VAL = (1UL << NUM_BITS) - 1;

    Logic::BitVector X("x", NUM_BITS);

    for (unsigned long C = 0; C <= MAX_VAL; ++C)
    {
        DYNAMIC_SECTION("Testing X != " << C)
        {
            const Cnf::Clause neqClause = Logic::encode_neq_const(X, C);
            
            Cnf::Formula neqFormula;
            neqFormula &= neqClause;

            for (unsigned long val = 0; val <= MAX_VAL; ++val)
            {
                const Cnf::Formula f = neqFormula & fixVectorValueDimacs(X, val);
                const std::string dimacs = DimacsWriter::ToString(f).first;

                const bool expectedSat = (val != C);
                const bool actualSat = isSatTogaSat(dimacs);

                INFO("For X = " << val << ", C = " << C);
                REQUIRE(actualSat == expectedSat);
            }
        }
    }
}