#include "Encoder.h"
#include "Tseytin.h"
#include "MakeVar.h"
#include "Utility.h"

using namespace Cnf;
using namespace MakeVar;

Formula Encoder::genCodingStage1_cnf() const
{
    Utility::debug("CNF 1 start");

    const long m = targetStateCount_;
    const long alphabetSize = obj_->alphabetSize();

    Formula formula;
    Clause clause;
    Utility::debug("1a");
    // 1a
    for (long i = 0; i < m; i++)
        clause |= origin(i);

    formula &= clause;

    Utility::debug("1b");
    // 1b
    for (long i = 0; i < m; i++)
        for (long j = i + 1; j < m; j++)
        {
            formula &= OR(NOT(origin(i)), NOT(origin(j)));
        }

    // 1c
    Utility::debug("1c");
    clause.clear();
    for (long i = 0; i < m; i++)
        clause |= final(i);

    formula &= clause;
    clause.clear();

    // 1d
    Utility::debug("1d");
    for (long q = 0; q < m; q++)
        for (long a = 0; a < alphabetSize; a++)
            for (long p1 = 0; p1 < m; p1++)
                for (long p2 = p1 + 1; p2 < m; p2++)
                {
                    formula &= OR(NOT(trans(q, a, p1)), NOT(trans(q, a, p2)));
                }

    // 1e
    Utility::debug("1e");
    for (long q = 0; q < m; q++)
        for (long a = 0; a < alphabetSize; a++)
            for (long p = 0; p < m; p++)
                formula &= AND(OR(NOT(trans(q, a, p)), active(q)), OR(NOT(trans(q, a, p)), active(p)));

    // 1f
    Utility::debug("1f");
    for (long q = 0; q < m; q++)
        for (long a = 0; a < alphabetSize; a++)
        {
            Clause clause;
            clause |= NOT(active(q));
            for (long p = 0; p < m; p++)
                clause |= trans(q, a, p);
            formula &= clause;
        }

    // 1g
    Utility::debug("1g");
    for (long q = 0; q < m; q++)
        formula &= AND(OR(NOT(origin(q)), active(q)), OR(NOT(final(q)), active(q)));

    // 1i
    Utility::debug("1i");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < m; q++)
        {
            Clause clauseFst;
            clauseFst |= NOT(edge(p, q));
            for (long a = 0; a < alphabetSize; a++)
            {
                clauseFst |= trans(p, a, q);
                formula &= OR(NOT(trans(p, a, q)), edge(p, q));
            }
            formula &= clauseFst;
        }

    // 1k
    Utility::debug("1k");
    for (long q = 0; q < m; q++)
        formula &= reachable(q, q, 0);

    // 1k-2
    Utility::debug("1k-2");
    for (long q = 0; q < m; q++)
        for (long p = 0; p < m; p++)
        {
            if(q == p) continue;
            formula &= NOT(reachable(q, p, 0));
        }

    // 1n
    Utility::debug("1n");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < m; q++)
        {
            if (p == q)
                continue;
            formula &= OR(NOT(wordReachable(p, q)), reachable(p, q, m - 1));
            formula &= OR(wordReachable(p, q), NOT(reachable(p, q, m - 1)));
        }
    Utility::debug("CNF 1 done");

    return formula;
}