#include "Utility.h"

#include <functional>
#include <set>
#include <queue>

#include <iostream>

using namespace std;

bool Utility::canReachFinal(long id, const std::vector<long> &finalStates, const vector<vector<bool>> &reachingTable)
{
    for (long f : finalStates)
    {
        if (reachingTable.at(id).at(f))
            return true;
    }
    return false;
}

vector<bool> Utility::canReachFinalTable(const std::vector<long> &finalStates, const vector<vector<bool>> &reachingTable)
{
    const long nodesCount = reachingTable.size();
    vector<bool> res(nodesCount, false);
    for (long id = 0; id < nodesCount; id++)
        for (long f : finalStates)
        {
            if (reachingTable.at(id).at(f))
            {
                res.at(id) = true;
            }
        }
    return res;
}

std::vector<std::vector<bool>> Utility::getWordReachibilityTable(const std::vector<std::vector<long>> &graph)
{
    const long graphSize = graph.size();
    vector<vector<bool>> canReach(graphSize, vector<bool>(graphSize, false));

    function<void(vector<bool> &, long)> dfs = [&](vector<bool> &visited, long u)
    {
        visited.at(u) = true;
        for (long v : graph.at(u))
        {
            if (!visited.at(v))
            {
                dfs(visited, v);
            }
        }
    };

    for (long vertex = 0; vertex < graphSize; vertex++)
    {
        // We take the first step from `vertex` to its immediate neighbors
        for (long neighbor : graph.at(vertex))
        {
            if (!canReach.at(vertex).at(neighbor))
            {
                dfs(canReach.at(vertex), neighbor);
            }
        }
    }

    return canReach;
}

// Returns a graph representation of a automaton.
std::vector<std::vector<long>> Utility::dfaToGraph(const vector<vector<long>> &trans)
{
    const long alphabetSize = trans.at(0).size();
    const long nodesCount = trans.size();
    auto adj = std::vector<std::vector<long>>(nodesCount);

    for (long i = 0; i < nodesCount; i++)
    {
        set<long> outVert; // more than one letter can lead to the same node.

        for (long letter = 0; letter < alphabetSize; letter++)
            outVert.insert(trans.at(i).at(letter));

        for (long el : outVert)
            adj.at(i).push_back(el);
    }

    return adj;
}

bool Utility::isPrefix(const vector<long> &p1, const vector<long> &p2)
{
    const long p2Size = p2.size();
    const long p1Size = p1.size();

    if (p2Size < p1Size)
        return false;

    for (long i = 0; i < p1Size; i++)
    {
        if (p2.at(i) != p1.at(i))
            return false;
    }

    return true;
}

bool Utility::isSingleScc(const vector<vector<long>> &transFun)
{
    const auto wrTable = getWordReachibilityTable(dfaToGraph(transFun));
    const long statesCount = transFun.size();
    for (long i = 0; i < statesCount; i++)
        for (long j = i; j < statesCount; j++)
        {
            if (!wrTable.at(i).at(j) || !wrTable.at(j).at(i))
                return false;
        }
    return true;
}

// every state can be reached from origin.
bool Utility::IsConnected(
    const DFA &dfa)
{
    const long statesCount = dfa.statesCount();
    const long initialState = dfa.originalState();

    vector<bool> visited(statesCount, false);
    vector<long> stack;

    stack.push_back(initialState);
    visited.at(initialState) = true;

    while (!stack.empty())
    {
        const long state = stack.back();
        stack.pop_back();

        for (long nextState : dfa.transFun().at(state))
        {
            if (!visited.at(nextState))
            {
                visited.at(nextState) = true;
                stack.push_back(nextState);
            }
        }
    }

    for (long state = 0; state < statesCount; state++)
    {
        if (!visited.at(state))
            return false;
    }

    return true;
}

std::string Utility::str(const PartialDFA &dfa)
{
    std::string result = "PartialDFA:\n";
    result += "  States: " + std::to_string(dfa.statesCount()) + "\n";
    result += "  Alphabet size: " + std::to_string(dfa.transFun().empty() ? 0 : dfa.alphabetSize()) + "\n";

    result += "  Accepting states: [";
    for (size_t i = 0; i < dfa.acceptingStates().size(); ++i)
    {
        result += std::to_string(dfa.acceptingStates().at(i));
        if (i + 1 < dfa.acceptingStates().size())
            result += ", ";
    }
    result += "]\n";

    result += "  Transition table:\n";
    for (size_t state = 0; state < dfa.transFun().size(); ++state)
    {
        result += "    State " + std::to_string(state) + " (";
        result += dfa.stateIsAccepting(state) ? "ACCEPT" : "      ";
        result += "): [";

        for (size_t symbol = 0; symbol < dfa.transFun().at(state).size(); ++symbol)
        {
            result += std::to_string(dfa.transFun().at(state).at(symbol));
            if (symbol + 1 < dfa.transFun().at(state).size())
                result += ", ";
        }
        result += "]\n";
    }

    return result;
}

void Utility::debug(const std::string &msg)
{
    const bool debugFlag = true;
    if (debugFlag)
        std::cout << msg << std::endl;
}
