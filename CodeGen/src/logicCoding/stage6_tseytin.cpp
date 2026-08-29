#include "Encoder.h"

#include "LogicAst.h"
#include "Tseytin.h"
#include "MakeVar.h"
#include "Utility.h"

using namespace std;
using namespace LogicAst;
using namespace MakeVar;

Cnf::Formula Encoder::genCodingStage6_tseytin() const
{
    Utility::debug("Tseytin 6 start");
    const long m = targetStateCount_;
    const long PathsCount = this->pathsCount();

    Cnf::Formula f;

    // 6a
    // if the target automat size is not a power of two, otherwise the number of bits of the size
    // is greater than these of bitSize, so the inequality holds trivially.
    // Special case when targets equal 1.
    Utility::debug("6a");
    if (targetStateCount_ == 1)
    {
        for (long p = 0; p < PathsCount; p++)
        {
            const long PathLen = this->pathLen(p);
            for (long i = 0; i < PathLen; i++)
            {
                const auto bitVec = CoveringPath(p, i, targetStateCountBitLen_);
                f &= Logic::encode_neq_const(bitVec, 1);
            }
        }
    }
    else if ((targetStateCount_ & (targetStateCount_ - 1)) != 0)
    {
        for (long p = 0; p < PathsCount; p++)
        {
            const long PathLen = this->pathLen(p);
            for (long i = 0; i < PathLen; i++)
            {
                const auto bitVec = CoveringPath(p, i, targetStateCountBitLen_);
                shared_ptr<Node> constraint = Logic::encode_lt_const(bitVec, m);
                if (constraint != nullptr)
                    f &= Tseytin::Transform(constraint);
            }
        }
    }

    Utility::debug("6d");
    // 6d
    for (long c = 0; c < m; c++)
        for (long d = 0; d < dagNodesCount(); d++)
        {
            const auto &NodesSubset = this->sccNode(d);
            shared_ptr<Node> subExp = nullptr;
            for (long p = 0; p < m; p++)
                for (long q : NodesSubset)
                    subExp |= AND(belongs(c, p), prodRS(p, q));
            f &= Tseytin::Transform(IMPLIES(subLangS(d, c), subExp));
        }

    Utility::debug("Tseytin 6 done");
    return f;
}