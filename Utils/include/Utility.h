#pragma once

#include "DFA.h"

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

// Functions shared by various modules.
namespace Utility
{
    bool canReachFinal(long, const std::vector<long> &, const std::vector<std::vector<bool>> &);
    std::vector<std::vector<bool>> getWordReachibilityTable(const std::vector<std::vector<long>> &graph);
    std::vector<std::vector<long>> dfaToGraph(const std::vector<std::vector<long>> &trans);
    std::vector<bool> canReachFinalTable(const std::vector<long> &finalStates, const std::vector<std::vector<bool>> &reachingTable);
    bool isPrefix(const std::vector<long> &p1, const std::vector<long> &p2);
    bool isSingleScc(const std::vector<std::vector<long>> &transFun);
    bool IsConnected(const DFA &dfa);
    std::string str(const PartialDFA &dfa);
    void debug(const std::string &msg);
    std::string str(const std::vector<long> &vector);
    std::string str(const std::vector<std::vector<long>> &matrix);
    std::string str(const std::vector<std::vector<bool>> &matrix);
    std::string str(const std::unordered_map<long, long> &mp);
    std::string str(const std::set<long> &st);
    DFA minimizeDFA(const DFA &dfa);
    std::string str(const std::vector<bool> &vector);
}