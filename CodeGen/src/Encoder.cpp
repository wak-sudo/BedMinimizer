
#include "CnfAst.h"
#include "Encoder.h"

#include "Utility.h"

#include <cmath>
#include <cassert>

using namespace std;

Encoder::Encoder(shared_ptr<const IPreCalc> obj, long targetStateCount)
    : obj_(obj), targetStateCount_(targetStateCount)
{
    if (targetStateCount <= 0)
        throw invalid_argument("Target states count must be greater than 0");
    if (targetStateCount > obj->automataStatesCount())
        throw invalid_argument("Target states count cannot be bigger than the current size");
    targetStateCountBitLen_ = max(1l, (long)ceil(log2(targetStateCount_)));
}

long Encoder::sinkB() const
{
    return targetStateCount_;
}

long Encoder::sinkA() const
{
    return obj_->automataStatesCount();
}

Cnf::Formula Encoder::genCoding() const
{
    Cnf::Formula formula;
    formula &= genCodingStage1();
    formula &= genCodingStage2();
    formula &= genCodingStage3();
    formula &= genCodingStage4();
    formula &= genCodingStage5();
    formula &= genCodingStage6();
    return formula;
}

Cnf::Formula Encoder::genCodingTargetContained() const
{
    Cnf::Formula formula;
    formula &= genCodingStage1();
    formula &= genCodingStage2();
    formula &= genCodingStage3();
    formula &= genCodingStage4();
    return formula;
}
Cnf::Formula Encoder::genCodingTargetContains() const
{
    Cnf::Formula formula;
    formula &= genCodingStage1();
    formula &= genCodingStage2();
    formula &= genCodingStage5();
    formula &= genCodingStage6();
    return formula;
}

const vector<long> &Encoder::sccNode(long id) const
{
    return obj_->sccNodes().at(id);
}

long Encoder::dagNodesCount() const
{
    return obj_->sccNodes().size();
}

long Encoder::pathNode(long p, long idx) const
{
    return obj_->paths().at(p).at(idx);
}

long Encoder::pathLen(long pathId) const
{
    return obj_->paths().at(pathId).size();
}

bool Encoder::isPrefix(long idP1, long idP2) const
{
    return Utility::isPrefix(obj_->paths().at(idP1), obj_->paths().at(idP2));
}

long Encoder::lastNode(long p) const
{
    return obj_->paths().at(p).back();
}

long Encoder::pathsCount() const
{
    return obj_->paths().size();
}