#include "Encoder.h"
#include "MakeVar.h"
#include "Utility.h"

#include <cmath>

using namespace Cnf;
using namespace MakeVar;

Cnf::Formula Encoder::genCodingStage6_cnf() const
{
    Utility::debug("CNF 6 start");
    const long m = targetStateCount_;
    const long PathsCount = this->pathsCount();
    const long StatesCount = obj_->automataStatesCount();
    const long AlphabetSize = obj_->alphabetSize();

    Formula f;

    // 6b
    Utility::debug("6b");
    for (long p = 0; p < PathsCount; p++)
    {
        const long PathLen = this->pathLen(p);
        for (long i = 0; i < PathLen - 1; i++)
        {
            const auto bv1 = CoveringPath(p, i, targetStateCountBitLen_);
            const auto bv2 = CoveringPath(p, i + 1, targetStateCountBitLen_);
            for (long a = 0; a < m; a++)
                for (long b = 0; b < m; b++)
                {
                    const auto eq1 = encode_neq_const(bv1, a);
                    const auto eq2 = encode_neq_const(bv2, b);
                    f &= OR(eq1 | eq2, closureE(a, b));
                }
        }
    }

    // 6c
    Utility::debug("6c");
    for (long p = 0; p < PathsCount; p++)
    {
        const long PathLen = this->pathLen(p);
        for (long i = 0; i < PathLen; i++)
            for (long c = 0; c < m; c++)
            {
                f &= OR(encode_neq_const(CoveringPath(p, i, targetStateCountBitLen_), c), subLangS(this->pathNode(p, i), c));
            }
    }

    // 6e
    Utility::debug("6e");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < StatesCount; q++)
            for (long a = 0; a < AlphabetSize; a++)
                for (long k = 0; k < m; k++)
                {
                    f &= OR({NOT(prodRS(p, q)), NOT(trans(p, a, k)), NOT(sameComponent(p, k)), prodRS(k, obj_->nextState(q, a))});
                    f &= OR({NOT(prodRS(p, q)), NOT(trans(p, a, k)), sameComponent(p, k), prodRS(sinkB(), obj_->nextState(q, a))});
                }

    // 6f
    Utility::debug("6f");
    for (long q = 0; q < StatesCount; q++)
        f &= NOT(prodRS(sinkB(), q));

    Utility::debug("CNF 6 done");
    return f;
}