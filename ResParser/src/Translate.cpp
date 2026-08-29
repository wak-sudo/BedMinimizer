#include "Translate.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_map>
#include <set>
#include "Utility.h"

using namespace std;
using namespace Instances;

unordered_map<long, long> Translate::getTransRelabeling(
    const vector<TransInstance> &trans)
{
    long counter = 0;
    unordered_map<long, long> relabeling;

    relabeling.reserve(2 * trans.size());

    for (const auto &ti : trans)
    {
        if (relabeling.emplace(ti.from, counter).second)
            ++counter;

        if (relabeling.emplace(ti.to, counter).second)
            ++counter;
    }

    return relabeling;
}

std::unordered_map<long, long> Translate::getDagRelabeling(const std::vector<DagNodeInstance> &nodes)
{
    long counter = 0;
    unordered_map<long, long> relabeling;
    relabeling.reserve(nodes.size());

    for (const auto &node : nodes)
    {
        if (relabeling.emplace(node.nodeId, counter).second)
            ++counter;
    }

    return relabeling;
}

vector<vector<long>> Translate::relableTrans()
{
    const long statesCount = transRelabeling_.size();

    vector<vector<long>> res(statesCount, vector<long>(info_.alphabetSize));

    for (const auto &ti : cert_.trans)
    {
        const long from = transRelabeling_.at(ti.from);
        const long by = ti.by;
        const long to = transRelabeling_.at(ti.to);

        res.at(from).at(by) = to;
    }

    return res;
}

long Translate::relableOrigin()
{
    return transRelabeling_.at(cert_.origin);
}

vector<long> Translate::relableAcceptingStates()
{
    vector<long> res;
    res.reserve(cert_.acceptingStates.size());

    for (const auto state : cert_.acceptingStates)
    {
        res.push_back(transRelabeling_.at(state));
    }

    return res;
}

vector<vector<long>> Translate::relableDag()
{
    const size_t dagNodesCount = dagsRelabeling_.size();

    vector<vector<long>> res(dagNodesCount);

    for (const auto &entry : cert_.dagNodes)
    {
        res.at(dagsRelabeling_.at(entry.nodeId))
            .push_back(transRelabeling_.at(entry.includedState));
    }

    return res;
}

unsigned long long Translate::bitsToNumber(
    const vector<bool> &bits)
{
    unsigned long long value = 0;
    const size_t size = bits.size();

    for (size_t i = 0; i < size; ++i)
    {
        if (bits.at(i))
            value |= (1ULL << i);
    }

    return value;
}

vector<vector<long>> Translate::mergeCoveringPath()
{
    vector<vector<long>> res(info_.orgPathsCount);

    vector<long> maxSize(info_.orgPathsCount, -1);

    set<long> test;

    for (const auto &instance : cert_.coveringPaths)
    {
        test.insert(instance.pathId);
        maxSize.at(instance.pathId) = max(maxSize.at(instance.pathId), instance.nodeRank);
    }

    for (long i = 0; i < info_.orgPathsCount; i++)
    {
        res.at(i).resize(maxSize.at(i) + 1);
    }

    for (const auto &instance : cert_.coveringPaths)
    {
        if (instance.bitVal)
        {
            res.at(instance.pathId).at(instance.nodeRank) |= (1ULL << instance.bitRank);
        }
    }

    return res;
}

std::vector<std::vector<long>> Translate::relableCovers()
{
    std::vector<std::vector<long>> res(dagsRelabeling_.size());

    for (const auto &entry : cert_.covers)
    {
        res.at(dagsRelabeling_.at(entry.nodeId)).push_back(entry.pathId);
    }

    return res;
}

std::vector<std::vector<long>> Translate::relableCoveringPath(std::vector<std::vector<long>> paths)
{
    vector<vector<long>> res(info_.orgPathsCount);
    for (long i = 0; i < info_.orgPathsCount; i++)
        res.at(i).reserve(paths.at(i).size());

    for (long pathId = 0; pathId < info_.orgPathsCount; pathId++)
    {
        for (long node : paths.at(pathId))
        {
            res.at(pathId).push_back(dagsRelabeling_.at(node));
        }
            
    }
    return res;
}

SolverResult Translate::pruneRawCertificate()
{
    SolverResult result;
    result.trans = relableTrans();
    result.origin = relableOrigin();
    result.acceptingStates = relableAcceptingStates();
    result.dagNodes = relableDag();
    result.covers = relableCovers();
    result.coveringPaths = relableCoveringPath(mergeCoveringPath());
    return result;
}

Translate::Translate(RawPartCertificate cert, CertInfo info) : cert_(std::move(cert)), info_(std::move(info))
{
    transRelabeling_ = getTransRelabeling(cert_.trans);
    dagsRelabeling_ = getDagRelabeling(cert_.dagNodes);
}