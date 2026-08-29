#include "Encoder.h"

#include "LogicAst.h"
#include "Tseytin.h"
#include "MakeVar.h"
#include "Utility.h"

using namespace std;
using namespace LogicAst;
using namespace MakeVar;

Cnf::Formula Encoder::genCodingStage2_tseytin() const
{
    Utility::debug("Tseytin 2 start");
    const long targetStateCount = targetStateCount_;

    Cnf::Formula f;

    const long m = targetStateCount;

    // 2c
    Utility::debug("2c");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < m; q++)
        {
            shared_ptr<Node> exp = AND(wordReachable(p, q), AND(wordReachable(q, p), canReachFinal(q)));
            shared_ptr<Node> subExp = nullptr;
            for (long c = 0; c < m; c++)
                subExp |= AND(belongs(c, p), belongs(c, q));
            f &= Tseytin::Transform(IMPLIES(exp, subExp));
        }

    // 2e
    Utility::debug("2e");
    for (long c1 = 0; c1 < m; c1++)
        for (long c2 = 0; c2 < m; c2++)
        {
            if(c1 == c2) continue;
            shared_ptr<Node> subExp = nullptr;
            for (long p = 0; p < m; p++)
                for(long q = 0; q < m; q++)
                {
                    subExp |= AND(belongs(c1, p), AND(belongs(c2, q), wordReachable(p, q)));
                }
            f &= Tseytin::Transform(IMPLIES(dagE(c1, c2), subExp));
        }

    Utility::debug("Tseytin 2 done");

    return f;
}