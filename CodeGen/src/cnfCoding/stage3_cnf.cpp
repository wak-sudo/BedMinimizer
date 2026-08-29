#include "Encoder.h"
#include "MakeVar.h"
#include "Utility.h"

using namespace Cnf;
using namespace MakeVar;
using namespace std;

Formula Encoder::genCodingStage3_cnf() const
{
    Utility::debug("CNF 3 start");
    const long m = targetStateCount_;
    const long PathsCount = this->pathsCount();
    const long AlphabetSize = obj_->alphabetSize();
    const long StatesCount = obj_->automataStatesCount();

    Formula f;
    
    // 3a
    Utility::debug("3a");
   for (long c = 0; c < m; c++)
    {
        Clause clause = NOT(compNotEmpty(c));
        for (long p = 0; p < PathsCount; p++)
            clause |= cover(c, p);

        f &= clause;
    }
    
    // 3b
    Utility::debug("3b");
    for (long c = 0; c < m; c++)
        for (long p = 0; p < PathsCount; p++)
        {
          //  Utility::debug("Last node" + to_string(this->LastNode(p)));
            f &= OR(NOT(cover(c, p)), subLangF(c, this->lastNode(p)));

        }
            
    
    // 3d
    Utility::debug("3d");
    for (long p = 0; p < m; p++)
        for (long q = 0; q < StatesCount; q++)
            for (long a = 0; a < AlphabetSize; a++)
                for (long k = 0; k < m; k++)
                {
                    f &= OR({NOT(prodRF(p, q)), NOT(trans(p, a, k)), NOT(sameComponent(p, k)), prodRF(k, obj_->nextState(q, a))});
                    f &= OR({NOT(prodRF(p, q)), NOT(trans(p, a, k)), sameComponent(p, k), prodRF(sinkB(), obj_->nextState(q, a))});
                }
    
    
    // 3e
    Utility::debug("3e");
    for (long q = 0; q < m; q++)
        f &= NOT(prodRF(q, sinkA()));
    

    Utility::debug("CNF 3 done");

    return f;
}