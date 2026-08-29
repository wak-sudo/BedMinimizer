#pragma once

#include "PartialDFA.h"
#include <utility>

class DFA : public PartialDFA
{
private:
    long originalState_;

public:
    DFA(
        const std::vector<std::vector<long>> &transFun,
        const std::vector<long> &acceptingStates,
        long originalState) 
        : PartialDFA(transFun, acceptingStates), originalState_(originalState)
    {
    }

    DFA(const PartialDFA &pDfa, long originalState) 
        : PartialDFA(pDfa), originalState_(originalState)
    {
    }

    DFA(const DFA &) = default;
    DFA(DFA &&) = default;
    DFA &operator=(const DFA &) = default;
    DFA &operator=(DFA &&) = default;

    long originalState() const { return originalState_; }
};