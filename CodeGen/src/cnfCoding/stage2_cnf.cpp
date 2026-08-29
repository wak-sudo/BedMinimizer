#include "Encoder.h"
#include "MakeVar.h"
#include "Utility.h"

using namespace Cnf;
using namespace MakeVar;

Cnf::Formula Encoder::genCodingStage2_cnf() const
{
    Utility::debug("CNF 2 start");
    const long m = targetStateCount_;

    Formula f;

    // 2a
    Utility::debug("2a");
    for (long q = 0; q < m; q++)
        for (long c = 0; c < m; c++)
            for (long k = c + 1; k < m; k++)
            {
                f &= OR(NOT(belongs(c, q)), NOT(belongs(k, q)));
            }

    // 2b
    Utility::debug("2b");
    for (long c = 0; c < m; c++)
        for (long q = 0; q < m; q++)
            for (long p = 0; p < m; p++)
            {
                f &= OR({NOT(belongs(c, q)), NOT(belongs(c, p)), wordReachable(q, p)});
                f &= OR({NOT(belongs(c, q)), NOT(belongs(c, p)), wordReachable(p, q)});
                f &= OR({NOT(belongs(c, q)), NOT(belongs(c, p)), canReachFinal(q)});
            }

    // 2d
    Utility::debug("2d");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < m; q++)
            for (long c1 = 0; c1 < m; c1++)
                for (long c2 = 0; c2 < m; c2++)
                {
                    if (c1 == c2)
                        continue;
                    f &= OR({NOT(wordReachable(p, q)), NOT(belongs(c1, p)), NOT(belongs(c2, q)), dagE(c1, c2)});
                }

    // 2f
    Utility::debug("2f");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < m; q++)
        {
            f &= OR(NOT(sameComponent(p, q)), wordReachable(p, q));
            f &= OR(NOT(sameComponent(p, q)), wordReachable(q, p));
            f &= OR(NOT(sameComponent(p, q)), canReachFinal(q));
            f &= OR({NOT(wordReachable(p, q)), NOT(wordReachable(q, p)), NOT(canReachFinal(q)), sameComponent(p, q)});
        }

    // 2g
    Utility::debug("2g");
    for (long p = 0; p < m; p++)
    {
        Clause c;
        for (long q = 0; q < m; q++)
        {
            c |= belongs(p, q);
            f &= OR(NOT(belongs(p, q)), compNotEmpty(p));
        }
        f &= OR(c, NOT(compNotEmpty(p)));
    }

    Utility::debug("CNF 2 done");

    return f;
}