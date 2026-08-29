#include "AutomataUtil.h"

using namespace std;

#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>

PartialDFA AutomataUtil::getSubAutomata(const vector<long> &component, const vector<vector<long>> &transFun)
{
    unordered_map<long, long> nodesRelabeling;
    long counter = 0;

    for (long vertex : component)
    {
        nodesRelabeling.emplace(vertex, counter);
        counter++;
    }

    const long localNodesCount = component.size();
    const long dfaNodesCount = localNodesCount + 1; // +1 for the sink
    const long sinkStateId = localNodesCount;

    const long alphabetSize = transFun.at(0).size();
    const unordered_set<long> componentSet(component.begin(), component.end());

    vector<vector<long>> newTransFun(dfaNodesCount, vector<long>(alphabetSize, sinkStateId));

    for (long vertex : component)
    {
        const long newSource = nodesRelabeling.at(vertex);

        for (long letter = 0; letter < alphabetSize; letter++)
        {
            const long dest = transFun.at(vertex).at(letter);

            if (componentSet.contains(dest))
            {
                newTransFun.at(newSource).at(letter) = nodesRelabeling.at(dest);
            }
        }
    }

    // All states in the component (excluding the sink).
    vector<long> finalStates(localNodesCount);
    for (long i = 0; i < localNodesCount; i++)
    {
        finalStates.at(i) = i;
    }

    return PartialDFA(newTransFun, finalStates);
}

// by lemma 4.4, fst <= snd
bool AutomataUtil::checkSubAutomataContainment(const PartialDFA &fst, const PartialDFA &snd)
{
    const long statesCountFst = fst.statesCount();
    const long statesCountSnd = snd.statesCount();

    vector<DFA> prebuiltDFAs;
    prebuiltDFAs.reserve(statesCountSnd - 1);
    for (long vertexId = 0; vertexId < statesCountSnd - 1; vertexId++) // -1 to skip the sink
        prebuiltDFAs.emplace_back(DFA(snd, vertexId));

    for (long vertexId = 0; vertexId < statesCountFst - 1; vertexId++) // -1 to skip the sink
    {
        DFA currentDfa(fst, vertexId);
        for (long k = 0; k < statesCountSnd - 1; k++)
        {
            if (checkContainment(currentDfa, prebuiltDFAs.at(k)))
                return true;
        }
    }

    return false;
}

// A <= B
bool AutomataUtil::checkContainment(const DFA &fst, const DFA &snd)
{
    const DFA comp = dfaComplement(snd);
    const DFA inter = dfaIntersection(fst, comp);
    return languageIsEmpty(inter);
}

bool AutomataUtil::languageIsEmpty(const DFA &dfa)
{
    const long alphabetSize = dfa.alphabetSize();
    const long nodesCount = dfa.statesCount();
    const long originalState = dfa.originalState();
    const auto &transFun = dfa.transFun();

    vector<bool> visited(nodesCount, false);
    queue<long> myQueue;
    myQueue.push(originalState);
    visited.at(originalState) = true;

    while (!myQueue.empty())
    {
        const long crState = myQueue.front();
        myQueue.pop();

        if (dfa.stateIsAccepting(crState))
            return false;

        for (long letter = 0; letter < alphabetSize; letter++)
        {
            const long nextState = transFun.at(crState).at(letter);

            if (visited.at(nextState))
                continue;

            visited.at(nextState) = true;
            myQueue.push(nextState);
        }
    }

    return true;
}

DFA AutomataUtil::dfaComplement(const DFA &org)
{
    const long nodesCount = org.statesCount();
    vector<long> newAccepting;

    for (long i = 0; i < nodesCount; i++)
        if (!org.stateIsAccepting(i))
            newAccepting.push_back(i);

    const auto &transFun = org.transFun();
    const auto originalState = org.originalState();

    return DFA(transFun, newAccepting, originalState);
}

DFA AutomataUtil::dfaIntersection(const DFA &fst, const DFA &snd)
{
    const long n1 = fst.statesCount();
    const long n2 = snd.statesCount();

    auto stateId = [n2](long i, long j)
    {
        return i * n2 + j;
    };

    const auto &transFst = fst.transFun();
    const auto &transSnd = snd.transFun();

    const auto alphabetSize = fst.alphabetSize();

    vector<vector<long>> transFun(n1 * n2);

    for (long i = 0; i < n1; i++)
        for (long j = 0; j < n2; j++)
        {
            const long state = stateId(i, j);
            transFun.at(state).resize(alphabetSize);

            for (long letter = 0; letter < alphabetSize; letter++)
                transFun.at(state).at(letter) = stateId(
                    transFst.at(i).at(letter),
                    transSnd.at(j).at(letter));
        }

    vector<long> acceptingState;
    acceptingState.reserve(n1 * n2);

    for (long i = 0; i < n1; i++)
        for (long j = 0; j < n2; j++)
            if (fst.stateIsAccepting(i) && snd.stateIsAccepting(j))
                acceptingState.push_back(stateId(i, j));

    const long start = stateId(fst.originalState(), snd.originalState());

    return DFA(transFun, acceptingState, start);
}