#pragma once

#include "PartialDFA.h"

#include <vector>

namespace AugSccGraphBuilder
{
    constexpr long NONE = -1;
    std::vector<long> getNodeToComponent(const std::vector<std::vector<long>> &grouping, long nodesCount);
    std::vector<std::vector<long>> getRawNodesGrouping(const std::vector<std::vector<bool>> &reachability);

    std::vector<std::vector<long>> filterGrouping(
        const std::vector<std::vector<long>> &components,
        const std::vector<long> &finalStates,
        const std::vector<std::vector<bool>> &reachable);

    std::vector<std::vector<long>> createSccGraph(const std::vector<std::vector<bool>> &reachiblity, const std::vector<std::vector<long>> &componentsGrouping);
    std::vector<std::vector<long>> getValidComponentGrouping(const std::vector<std::vector<bool>> &dfaWordReachiblity, const PartialDFA &aut);
    std::vector<std::vector<long>> genPathIdToNodes(const std::vector<std::vector<long>> &graph);

    bool isEveryComponentReachableFromOrigin(
        const std::vector<std::vector<long>> &components,
        const std::vector<std::vector<bool>> &wrTable,
        long origin);
};