#include "AugSccGraphBuilder.h"
#include "Utility.h"

#include <functional>
#include <stack>
#include <map>
#include <ranges>
#include <algorithm>
#include <vector>

#include <iostream>

using namespace std;

vector<vector<long>> AugSccGraphBuilder::getValidComponentGrouping(const vector<vector<bool>> &dfaWordReachiblity, const PartialDFA &aut)
{
    return filterGrouping(getRawNodesGrouping(dfaWordReachiblity), aut.acceptingStates(), dfaWordReachiblity);
}

vector<long> AugSccGraphBuilder::getNodeToComponent(const vector<vector<long>> &grouping, long nodesCount)
{
    vector<long> nodeToComponent(nodesCount, NONE);
    const long groupsCount = grouping.size();
    for (long idx = 0; idx < groupsCount; idx++)
    {
        for (long node : grouping.at(idx))
            nodeToComponent.at(node) = idx;
    }
    return nodeToComponent;
}

vector<vector<long>> AugSccGraphBuilder::getRawNodesGrouping(const vector<vector<bool>> &reachability)
{
    const long rSize = reachability.size();
    long counter = 0;
    vector<long> nodeToScc(rSize, NONE);

    for (long vertex = 0; vertex < rSize; vertex++)
    {
        if (nodeToScc.at(vertex) != NONE)
            continue;

        vector<long> currentGroup;

        for (long j = vertex; j < rSize; j++)
        {
            if (nodeToScc.at(j) != NONE)
                continue;

            if (reachability.at(vertex).at(j) && reachability.at(j).at(vertex))
            {
                currentGroup.push_back(j);
            }
        }

        if (!currentGroup.empty())
        {
            for (long member : currentGroup)
            {
                nodeToScc.at(member) = counter;
            }
            counter++;
        }
    }

    vector<vector<long>> grouping(counter);
    for (long i = 0; i < rSize; i++)
    {
        if (nodeToScc.at(i) != NONE)
        {
            grouping.at(nodeToScc.at(i)).push_back(i);
        }
    }

    return grouping;
}

vector<vector<long>> AugSccGraphBuilder::filterGrouping(
    const vector<vector<long>> &components,
    const vector<long> &finalStates,
    const vector<vector<bool>> &reachingTable)
{
    vector<vector<long>> cleanedComponents;

    for (const auto &nodesVector : components)
    {
        if (nodesVector.empty())
            continue;

        const long leader = nodesVector.at(0);

        if (Utility::canReachFinal(leader, finalStates, reachingTable))
            cleanedComponents.push_back(nodesVector);
    }

    return cleanedComponents;
}

// Creates a graph from the SCC components given a DFA.
// The first element returned is a DAG created from the SCC of the DFA.
// The second element is a map : scc index -> contains an accepting node.
vector<vector<long>> AugSccGraphBuilder::createSccGraph(
    const vector<vector<bool>> &reachiblity,
    const vector<vector<long>> &componentsGrouping)
{
    if (componentsGrouping.empty())
        return vector<vector<long>>();

    const long componentsCount = componentsGrouping.size();

    vector<vector<long>> graph(componentsCount);

    for (long idx = 0; idx < componentsCount; idx++)
    {
        const long leader = componentsGrouping.at(idx).at(0);
        for (long otherIdx = idx + 1; otherIdx < componentsCount; otherIdx++)
        {
            const long otherLeader = componentsGrouping.at(otherIdx).at(0);
            if (reachiblity.at(leader).at(otherLeader))
                graph.at(idx).push_back(otherIdx);
            else if (reachiblity.at(otherLeader).at(leader))
                graph.at(otherIdx).push_back(idx);
        }
    }

    return graph;
}

vector<vector<long>> AugSccGraphBuilder::genPathIdToNodes(const vector<vector<long>> &g)
{
    const long nodesCount = g.size();
    vector<vector<long>> pathIdToNodes;
    vector<long> stack;

    function<void(long)> dfs = [&](long crIdx)
    {
        stack.push_back(crIdx);
        pathIdToNodes.push_back(stack);

        for (long next : g.at(crIdx))
            dfs(next);

        stack.pop_back();
    };

    for (long currentStart = 0; currentStart < nodesCount; currentStart++)
        dfs(currentStart);

    return pathIdToNodes;
}

bool AugSccGraphBuilder::isEveryComponentReachableFromOrigin(
    const std::vector<std::vector<long>> &components,
    const std::vector<std::vector<bool>> &wrTable,
    long origin)
{
    for(const auto& comp : components)
    {
        const long leader = comp.at(0);
        if(!wrTable.at(origin).at(leader))
        {
            cout << "ERROR:" << origin << leader << endl;
             return false;
        }
           
    }
    return true;
}