#include "Encoder.h"
#include "Utility.h"
#include <iostream>

using namespace std;

Cnf::Formula Encoder::genCodingStage1() const
{
    Cnf::Formula f = genCodingStage1_cnf();
    f &= genCodingStage1_tseytin();
    return f;
}

Cnf::Formula Encoder::genCodingStage2() const
{
    Cnf::Formula f = genCodingStage2_cnf();
    f &= genCodingStage2_tseytin();
    return f;
}

Cnf::Formula Encoder::genCodingStage3() const
{
    Cnf::Formula f = genCodingStage3_cnf();
    f &= genCodingStage3_tseytin();
    return f;
}

Cnf::Formula Encoder::genCodingStage4() const
{
    Cnf::Formula f = GenCodingStage4_cnf();
    f &= GenCodingStage4_tseytin();
    return f;
}

Cnf::Formula Encoder::genCodingStage5() const
{
    Cnf::Formula f = genCodingStage5_cnf();
    f &= genCodingStage5_tseytin();
    return f;
}

Cnf::Formula Encoder::genCodingStage6() const
{
    Cnf::Formula f = genCodingStage6_cnf();
    f &= genCodingStage6_tseytin();
    return f;
}