#include "AugSccGraphBasic.h"
#include "AugSccGraphBuilder.h"
#include "Utility.h"

#include <stdexcept>
#include <map>
#include <functional>
#include <algorithm>
#include <set>

#include <iostream>

using namespace std;

AugSccGraphBasic::AugSccGraphBasic(DFA internalDfa) : internalDfa_(move(internalDfa))
{
    const auto dfaWordReachiblity = Utility::getWordReachibilityTable(Utility::dfaToGraph(internalDfa_.transFun()));
    const auto componentsGrouping = AugSccGraphBuilder::getValidComponentGrouping(dfaWordReachiblity, internalDfa_);

    if(componentsGrouping.empty())
        throw invalid_argument("There must be at least one SCC component.");

    if(!AugSccGraphBuilder::isEveryComponentReachableFromOrigin(componentsGrouping, dfaWordReachiblity, internalDfa_.originalState()))
        throw invalid_argument("Every SCC must be reachable from the initial state");

    const auto stateToComponent = AugSccGraphBuilder::getNodeToComponent(componentsGrouping, internalDfa_.statesCount());
    const auto sccGraph = AugSccGraphBuilder::createSccGraph(dfaWordReachiblity, componentsGrouping);

    graph_ = move(sccGraph);
    components_ = move(componentsGrouping);
    stateToComponent_ = move(stateToComponent);
}

bool AugSccGraphBasic::dfaNodeIsInTheComponent(long dfaNode, long componentId) const
{
    return stateToComponent_.at(dfaNode) == componentId;
}

long AugSccGraphBasic::dagNodesCount() const
{
    return graph_.size();
}

const vector<vector<long>> &AugSccGraphBasic::sccNodes() const
{
    return components_;
}

long AugSccGraphBasic::nextState(long q, long a) const
{
    const long sink = globalSinkId();
    if (q == sink || stateToComponent_.at(q) == AugSccGraphBuilder::NONE)
        return sink;
    const long next = internalDfa_.transFun().at(q).at(a);
    if (stateToComponent_.at(q) == stateToComponent_.at(next))
        return next;
    else
        return sink;
}

long AugSccGraphBasic::globalSinkId() const
{
    return automataStatesCount();
}

long AugSccGraphBasic::automataStatesCount() const
{
    return internalDfa_.statesCount();
}

long AugSccGraphBasic::alphabetSize() const
{
    return internalDfa_.alphabetSize();
}

long AugSccGraphBasic::getDepth()
{
    if (sccDepth_.has_value())
        return sccDepth_.value();

    const long graphSize = graph_.size();

    if (graphSize == 0)
        return 0;

    std::vector<long> depth(graphSize, -1);

    std::function<long(long)> dfs = [&](long v) -> long
    {
        if (depth.at(v) != -1)
            return depth.at(v);

        long maximalDepth = 1;

        for (auto next : graph_.at(v))
            maximalDepth = std::max(maximalDepth, dfs(next) + 1);

        depth.at(v) = maximalDepth;
        return maximalDepth;
    };

    for (long v = 0; v < graphSize; v++)
        dfs(v);

    const long res = std::ranges::max(depth);
    sccDepth_ = res;

    return res;
}

const DFA &AugSccGraphBasic::getOriginalDfa() const
{
    return internalDfa_;
}

const std::vector<std::vector<long>> &AugSccGraphBasic::getComponents() const
{
    return components_;
}