#pragma once

namespace Instances
{
    struct CoveringPathInstance
    {
        long pathId;
        long nodeRank;
        long bitRank;
        long bitVal;
    };
    
    struct DagNodeInstance
    {
        long nodeId;
        long includedState;
    };

    struct TransInstance
    {
        long from;
        long by;
        long to;
    };

    struct CoversInstance
    {
        long nodeId;
        long pathId;
    };
}
