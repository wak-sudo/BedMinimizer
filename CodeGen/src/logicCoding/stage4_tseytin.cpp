#include "Encoder.h"

#include "LogicAst.h"
#include "Tseytin.h"
#include "MakeVar.h"
#include "Utility.h"

#include <cmath>

using namespace std;
using namespace LogicAst;
using namespace MakeVar;

Cnf::Formula Encoder::GenCodingStage4_tseytin() const
{
    Utility::debug("Tseytin 4 start");
    const long PathsCount = this->pathsCount();
    const long bitSize = max(1l, (long)ceil(log2(PathsCount)));
    const long m = targetStateCount_;

    Cnf::Formula f;

    Utility::debug("4a");
    // 4a
    // if the PathsCount is a power of two, then the number of bits of PathsCount 
    // is greater than these of bitSize, so the inequality holds trivially.
    // See that there's no problem when PathsCount = 1 (in contrast to stage 6).
    if ((PathsCount & (PathsCount - 1)) == 0)
        return f;
    
    for (long a = 0; a < m; a++)
        for (long b = 0; b < m; b++)
            for (long p = 0; p < PathsCount; p++)
            {
                const auto bitVec = continuationId(a, b, p, bitSize);
                shared_ptr<Node> fst = Logic::encode_lt_const(bitVec, PathsCount);
                f &= Tseytin::Transform(fst);
            }

    Utility::debug("Tseytin 4 done");
    return f;
}