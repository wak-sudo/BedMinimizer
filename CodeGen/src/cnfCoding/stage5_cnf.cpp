#include "Encoder.h"
#include "MakeVar.h"
#include "Utility.h"

using namespace Cnf;
using namespace MakeVar;

Cnf::Formula Encoder::genCodingStage5_cnf() const
{
    Utility::debug("CNF 5 start");
    const long m = targetStateCount_;
    Formula f;

    // 5a
    Utility::debug("5a");
    for (long c = 0; c < m; c++)
    {
        Clause clause = NOT(starDagR(c, c, 0));
        for (long p = 0; p < m; p++)
            clause |= belongs(c, p);

        f &= clause;
    }

    // 5b
    Utility::debug("5b");
    for (long c = 0; c < m; c++)
        for (long q = 0; q < m; q++)
            f &= OR(NOT(belongs(c, q)), starDagR(c, c, 0));

    // 5b-2
    Utility::debug("5b-2");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < m; q++)
        {
            if(p == q) continue;
            f &= NOT(starDagR(p, q, 0));
        }

    // 5d
    Utility::debug("5d");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < m; q++)
        {
            f &= OR(NOT(closureE(p, q)), starDagR(p, q, m - 1));
            f &= OR(closureE(p, q), NOT(starDagR(p, q, m - 1)));
        }

    Utility::debug("CNF 5 done");

    return f;
}