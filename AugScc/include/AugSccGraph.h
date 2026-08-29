#pragma once

#include "AugSccGraphBasic.h"
#include "IPreCalc.h"

class AugSccGraph : public AugSccGraphBasic, public IPreCalc
{
public:
    AugSccGraph(DFA aut);
    AugSccGraph(AugSccGraphBasic &&augScc);

    const std::vector<std::vector<long>> &sccNodes() const override;
    const std::vector<std::vector<long>> &paths() const override;
    long nextState(long q, long a) const override;
    long automataStatesCount() const override;
    long alphabetSize() const override;
    long globalSinkId() const;

private:
    std::vector<std::vector<long>> pathIdToNodes_;
};