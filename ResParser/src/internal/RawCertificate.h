#pragma once

#include <vector>

#include "Instances.h"

struct RawPartCertificate
{
    std::vector<Instances::TransInstance> trans;
    long origin;
    std::vector<long> acceptingStates;
    std::vector<Instances::DagNodeInstance> dagNodes;
    std::vector<Instances::CoversInstance> covers;
    std::vector<Instances::CoveringPathInstance> coveringPaths;
};
