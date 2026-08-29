#include "Encoder.h"
#include "MakeVar.h"
#include "Utility.h"
#include <cmath>

using namespace Cnf;
using namespace MakeVar;
using namespace std;

Formula Encoder::GenCodingStage4_cnf() const
{
    Utility::debug("CNF 4 start");
    const long PathsCount = this->pathsCount();
    const long PathsCountBitLen = max(1l, (long)ceil(log2(PathsCount)));
    const long m = targetStateCount_;

    Formula f;

    // 4b
    Utility::debug("4b");
    for (long a = 0; a < m; a++)
        for (long b = 0; b < m; b++)
            for (long p = 0; p < PathsCount; p++)
            {
                const Logic::BitVector bitVec = continuationId(a, b, p, PathsCountBitLen);
                for (long d = 0; d < PathsCount; d++)
                {
                    const bool isPrefix = this->isPrefix(p, d);
                    Clause base = OR(NOT(dagE(a, b)), NOT(cover(a, p))) | Logic::encode_neq_const(bitVec, d);
                    if (isPrefix)
                        f &= OR(std::move(base), std::move(cover(b, d)));
                    else
                        f &= std::move(base);
                }
            }
               
    Utility::debug("CNF 4 done");    

    return f;
}