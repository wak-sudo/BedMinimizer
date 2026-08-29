#include "Encoder.h"

#include "LogicAst.h"
#include "Tseytin.h"
#include "MakeVar.h"

#include "Utility.h"

using namespace LogicAst;
using namespace MakeVar;
using namespace std;

Cnf::Formula Encoder::genCodingStage1_tseytin() const
{
    Utility::debug("Tseytin 1 start");
    const long targetStateCount = targetStateCount_;
    const long alphabetSize = obj_->alphabetSize();

    Cnf::Formula formula;

    // 1h
    Utility::debug("1h");
    for (long q = 0; q < targetStateCount; q++)
    {
        shared_ptr<Node> subExp = nullptr;
        for (long p = 0; p < targetStateCount; p++)
        {
            subExp |= AND(origin(p), reachable(p, q, targetStateCount - 1));
        }
        formula &= Tseytin::Transform(IMPLIES(active(q), subExp));
    }

    // 1j
    Utility::debug("1j");
    for (long q = 0; q < targetStateCount; q++)
    {
        shared_ptr<Node> subExp = nullptr;
        for (long p = 0; p < targetStateCount; p++)
        {
            subExp |= AND(final(p), reachable(q, p, targetStateCount - 1));
        }
        formula &= Tseytin::Transform(IFF(canReachFinal(q), subExp));
    }

    // 1l
    Utility::debug("1l");
    for (long i = 0; i < targetStateCount; i++)
        for (long j = 0; j < targetStateCount; j++)
            for (long k = 0; k < targetStateCount - 1; k++)
            {
                shared_ptr<Node> subExp = nullptr;
                for (long l = 0; l < targetStateCount; l++)
                {
                    subExp |= AND(edge(i, l), reachable(l, j, k));
                }
                const auto rightExp = OR(reachable(i, j, k), subExp);
                formula &= Tseytin::Transform(IFF(reachable(i, j, k + 1), rightExp));
            }

    // 1m
    Utility::debug("1m");
    for (long p = 0; p < targetStateCount; p++)
    {
        shared_ptr<Node> subExpOne = nullptr;
        for (long a = 0; a < alphabetSize; a++)
        {
            subExpOne |= trans(p, a, p);
        }
        shared_ptr<Node> subExpTwo = nullptr;
        for (long q = 0; q < targetStateCount; q++)
        {
            subExpTwo |= AND(edge(p, q), reachable(q, p, targetStateCount - 1));
        }
        const auto rightExp = OR(subExpOne, subExpTwo);
        formula &= Tseytin::Transform(IFF(wordReachable(p, p), rightExp));
    }

    Utility::debug("Tseytin 1 done");

    return formula;
}