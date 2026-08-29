#include "AugSccGraph.h"
#include "AugSccGraphBuilder.h"
#include "Utility.h"

#include <functional>

using namespace std;

AugSccGraph::AugSccGraph(DFA aut) : AugSccGraphBasic(move(aut))
{
    pathIdToNodes_ = AugSccGraphBuilder::genPathIdToNodes(graph_);
}

AugSccGraph::AugSccGraph(AugSccGraphBasic &&base)
    : AugSccGraphBasic(std::move(base))
{
    pathIdToNodes_ = AugSccGraphBuilder::genPathIdToNodes(this->graph_);
}

const std::vector<std::vector<long>> &AugSccGraph::paths() const
{
    return pathIdToNodes_;
}

const std::vector<std::vector<long>> &AugSccGraph::sccNodes() const { return AugSccGraphBasic::sccNodes(); }
long AugSccGraph::nextState(long q, long a) const { return AugSccGraphBasic::nextState(q, a); }
long AugSccGraph::automataStatesCount() const { return AugSccGraphBasic::automataStatesCount(); }
long AugSccGraph::alphabetSize() const { return AugSccGraphBasic::alphabetSize(); }
long AugSccGraph::globalSinkId() const { return AugSccGraphBasic::globalSinkId(); }