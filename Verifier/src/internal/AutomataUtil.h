#pragma once

#include "DFA.h"

namespace AutomataUtil
{
    PartialDFA getSubAutomata(
        const std::vector<long> &component,
        const std::vector<std::vector<long>> &transFun);

    bool checkContainment(const DFA &fstDfa, const DFA &sndDfa);

    bool checkSubAutomataContainment(
        const PartialDFA &fstDfa,
        const PartialDFA &sndDfa);

    DFA dfaComplement(const DFA &dfa);

    DFA dfaIntersection(const DFA &fstDfa, const DFA &sndDfa);

    bool languageIsEmpty(const DFA &dfa);
}