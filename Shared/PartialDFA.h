#pragma once

#include <vector>
#include <utility>

class PartialDFA
{
protected:
    std::vector<bool> stateIsAccepting_;
    std::vector<long> acceptingStates_;
    std::vector<std::vector<long>> transFun_;

public:
    PartialDFA(
        std::vector<std::vector<long>> transFunArg,
        std::vector<long> acceptingStatesArg)
        : acceptingStates_(std::move(acceptingStatesArg)),
          transFun_(std::move(transFunArg))
    {
        const long nodesCount = transFun_.size();
        stateIsAccepting_ = std::vector<bool>(nodesCount, false);
        for (long i : acceptingStates_)
            stateIsAccepting_.at(i) = true;
    }

    PartialDFA(const PartialDFA &) = default;
    PartialDFA(PartialDFA &&) = default;
    PartialDFA &operator=(const PartialDFA &) = default;
    PartialDFA &operator=(PartialDFA &&) = default;

    long statesCount() const { return transFun_.size(); }

    bool stateIsAccepting(long state) const { return stateIsAccepting_.at(state); }

    const std::vector<long> &acceptingStates() const { return acceptingStates_; }

    long alphabetSize() const { return transFun_.empty() ? 0 : transFun_.at(0).size(); }

    const std::vector<std::vector<long>> &transFun() const { return transFun_; }
};