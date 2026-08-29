#pragma once

#include "DFA.h"

#include <vector>
#include <memory>
#include <optional>

class AugSccGraphBasic
{
public:
    AugSccGraphBasic(DFA aut);

    long dagNodesCount() const;

    const std::vector<std::vector<long>> &sccNodes() const;
    long nextState(long q, long a) const;
    long automataStatesCount() const;
    long alphabetSize() const;
    long globalSinkId() const;
    long getDepth();

    const DFA &getOriginalDfa() const;
    const std::vector<std::vector<long>> &getComponents() const;

protected:
    std::vector<std::vector<long>> graph_;
    std::vector<std::vector<long>> components_;
    std::vector<long> stateToComponent_;
    DFA internalDfa_;
    std::optional<long> sccDepth_;

    bool dfaNodeIsInTheComponent(long dfaNode, long componentId) const;
};