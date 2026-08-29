#pragma once

#include "CnfAst.h"
#include "IPreCalc.h"

#include <functional>
#include <memory>

class Encoder
{
    std::shared_ptr<const IPreCalc> obj_;
    unsigned long targetStateCount_;
    unsigned long targetStateCountBitLen_;

    long sinkB() const;
    long sinkA() const;

    const std::vector<long> &sccNode(long nodeId) const;
    long dagNodesCount() const;
    long pathNode(long pathId, long idx) const;
    long pathLen(long pathId) const;
    bool isPrefix(long idP1, long idP2) const;
    long lastNode(long pathId) const;
    long pathsCount() const;

    Cnf::Formula genCodingStage1() const;
    Cnf::Formula genCodingStage1_cnf() const;
    Cnf::Formula genCodingStage1_tseytin() const;

    Cnf::Formula genCodingStage2() const;
    Cnf::Formula genCodingStage2_cnf() const;
    Cnf::Formula genCodingStage2_tseytin() const;

    Cnf::Formula genCodingStage3() const;
    Cnf::Formula genCodingStage3_cnf() const;
    Cnf::Formula genCodingStage3_tseytin() const;

    Cnf::Formula genCodingStage4() const;
    Cnf::Formula GenCodingStage4_cnf() const;
    Cnf::Formula GenCodingStage4_tseytin() const;

    Cnf::Formula genCodingStage5() const;
    Cnf::Formula genCodingStage5_cnf() const;
    Cnf::Formula genCodingStage5_tseytin() const;

    Cnf::Formula genCodingStage6() const;
    Cnf::Formula genCodingStage6_cnf() const;
    Cnf::Formula genCodingStage6_tseytin() const;

public:
    Cnf::Formula genCoding() const;
    Cnf::Formula genCodingTargetContained() const; // B <= A
    Cnf::Formula genCodingTargetContains() const;  // A <= B

    Encoder(std::shared_ptr<const IPreCalc>, long);
};
