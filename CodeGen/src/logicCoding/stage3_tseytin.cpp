#include "Encoder.h"

#include "LogicAst.h"
#include "Tseytin.h"
#include "MakeVar.h"
#include "Utility.h"

using namespace std;
using namespace LogicAst;
using namespace MakeVar;

#include <iostream>

Cnf::Formula Encoder::genCodingStage3_tseytin() const
{
    Utility::debug("Tseytin 3 start");
    const long m = targetStateCount_;
    const long DagNodesCount = this->dagNodesCount();

    Cnf::Formula f;

    Utility::debug("3c");
    // 3c
    for (long c = 0; c < m; c++)
        for (long d = 0; d < DagNodesCount; d++)
        {
            const auto &NodesSubset = this->sccNode(d);
            Utility::debug(to_string(d) + ": " + Utility::str(NodesSubset));
            shared_ptr<Node> subExp = nullptr;
            for (long p = 0; p < m; p++)
                for (long q : NodesSubset)
                    subExp |= AND(belongs(c, p), prodRF(p, q));

            f &= Tseytin::Transform(IMPLIES(subLangF(c, d), subExp));
        }

    Utility::debug("Tseytin 3 done");

    return f;
}