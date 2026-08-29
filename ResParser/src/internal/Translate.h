#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "Instances.h"
#include "RawCertificate.h"
#include "CertInfo.h"
#include "Certificate.h"

class Translate
{
private:
    RawPartCertificate cert_;
    CertInfo info_;
    std::unordered_map<long, long> transRelabeling_;
    std::unordered_map<long, long> dagsRelabeling_;

    static std::unordered_map<long, long> getTransRelabeling(const std::vector<Instances::TransInstance> &trans);

    static std::unordered_map<long, long> getDagRelabeling(const std::vector<Instances::DagNodeInstance> &nodes);

    std::vector<std::vector<long>> relableTrans();

    long relableOrigin();

    std::vector<long> relableAcceptingStates();

    std::vector<std::vector<long>> relableDag();
    
    std::vector<std::vector<long>> relableCovers();

    std::vector<std::vector<long>> relableCoveringPath(std::vector<std::vector<long>> paths);

    std::vector<std::vector<long>> mergeCoveringPath();

    static unsigned long long bitsToNumber(const std::vector<bool> &bits);

public:
    Translate(RawPartCertificate cert, CertInfo info);
    SolverResult pruneRawCertificate();
};