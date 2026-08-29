#include "Encoder.h"

#include "LogicAst.h"
#include "Tseytin.h"
#include "MakeVar.h"
#include "Utility.h"

using namespace std;
using namespace LogicAst;
using namespace MakeVar;

Cnf::Formula Encoder::genCodingStage5_tseytin() const
{
    Utility::debug("Tseytin 5 start");
    const long m = targetStateCount_;

    Cnf::Formula f;

    // 5c
    Utility::debug("5c");
    for (long i = 0; i < m; i++)
        for (long j = 0; j < m; j++)
            for (long k = 0; k < m - 1; k++)
            {
                shared_ptr<Node> subExp = starDagR(i, j, k);
                for (long l = 0; l < m; l++)
                {
                    subExp |= AND(dagE(i, l), starDagR(l, j, k));
                }
                f &= Tseytin::Transform(IFF(starDagR(i, j, k + 1), subExp));
            }

    Utility::debug("Tseytin 5 done");
    return f;
}