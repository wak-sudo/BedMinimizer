#pragma once

#include <vector>

class IPreCalc
{
public:
    virtual ~IPreCalc() {}
    
    virtual const std::vector<std::vector<long>>& sccNodes() const = 0;
    virtual const std::vector<std::vector<long>>& paths() const = 0;
    virtual long nextState(long q, long a) const = 0;
    virtual long automataStatesCount() const = 0;
    virtual long alphabetSize() const = 0;
    
};