#include "Checker.h"

#include "Utility.h"
#include "AutomataUtil.h"

#include <format>

#include <iostream>

using namespace std;

Checker::Checker(Certificate cert) : cert_(move(cert))
{
    const auto autGraph = Utility::dfaToGraph(cert_.guessed.trans);
    dfaReachibilityByWord_ = Utility::getWordReachibilityTable(autGraph);
    canReachFinal_ = Utility::canReachFinalTable(cert_.guessed.acceptingStates, dfaReachibilityByWord_);
    dagGraph_ = genEdges(cert_.guessed.dagNodes, dfaReachibilityByWord_);
}

bool Checker::pathCanBelongToComponent(const long pathId, const long targetComponentId) const
{
    const auto lastNodeId = getLastNodeId(pathId);
    const auto lastNode = cert_.given.components.at(lastNodeId);
    const auto targetNode = cert_.guessed.dagNodes.at(targetComponentId);

    const auto aut1 = AutomataUtil::getSubAutomata(targetNode, cert_.guessed.trans);
    const auto aut2 = AutomataUtil::getSubAutomata(lastNode, cert_.given.originalDfa.transFun());
    
    return AutomataUtil::checkSubAutomataContainment(aut1, aut2);
}

bool Checker::isPrefix(const long p1Id, const long p2Id) const
{
    return Utility::isPrefix(cert_.given.paths.at(p1Id), cert_.given.paths.at(p2Id));
}

vector<vector<long>> Checker::genEdges(const vector<vector<long>> &components, const vector<vector<bool>> &dfaReachibilityByWord)
{
    const long componentCount = components.size();
    vector<vector<long>> edges(componentCount);

    for (long componentId = 0; componentId < componentCount; componentId++)
    {
        const long leaderA = components.at(componentId).at(0);

        for (long otherComponent = 0; otherComponent < componentCount; otherComponent++)
        {
            if (otherComponent == componentId)
                continue;

            const long leaderB = components.at(otherComponent).at(0);

            if (dfaReachibilityByWord.at(leaderA).at(leaderB))
                edges.at(componentId).push_back(otherComponent);
        }
    }

    return edges;
}

bool Checker::checkPathCovered(
    const long coveredPathId,
    const vector<long> &coveringPath) const
{
    const auto coveredPath = cert_.given.paths.at(coveredPathId);
    const size_t pathLen = coveredPath.size();

    if (pathLen != coveringPath.size())
        throw invalid_argument(format("Path {0} is not the same length as the covering path", coveredPathId));

    for (size_t i = 0; i < pathLen; i++)
    {
        const long coveredNodeId = coveredPath.at(i);
        const auto coveredNode = cert_.given.components.at(coveredNodeId);

        const long coveringNodeId = coveringPath.at(i);
        const auto coveringNode = cert_.guessed.dagNodes.at(coveringNodeId);

        const PartialDFA p1 = AutomataUtil::getSubAutomata(coveredNode, cert_.given.originalDfa.transFun());
        const PartialDFA p2 = AutomataUtil::getSubAutomata(coveringNode, cert_.guessed.trans);

        if (!AutomataUtil::checkSubAutomataContainment(p1, p2))
            throw invalid_argument(format("In path {0}, node {1} from the dag A is not covered by node {2} from the dag B", coveredPathId, coveredNodeId, coveringNodeId));
    }

    return true;
}

bool Checker::checkGuessedPathExists(const std::vector<long> &coveringPath) const
{
    const long coveringPathSize = coveringPath.size();

    if (coveringPathSize == 0)
        throw invalid_argument("Covering path cannot be empty");

    for (long i = 0; i < coveringPathSize - 1; i++)
    {
        const long crNodeId = coveringPath.at(i);
        const long nextNodeId = coveringPath.at(i + 1);

        const long crLeader = cert_.guessed.dagNodes.at(crNodeId).at(0);
        const long nextLeader = cert_.guessed.dagNodes.at(nextNodeId).at(0);

        if (!dfaReachibilityByWord_.at(crLeader).at(nextLeader) && crLeader != nextLeader)
            throw invalid_argument(format("An edge from {0} to {1} does not exist in the clousure of the guessed DAG", crNodeId, nextNodeId));
    }

    return true;
}

long Checker::getLastNodeId(long pathId) const
{
    return cert_.given.paths.at(pathId).back();
}