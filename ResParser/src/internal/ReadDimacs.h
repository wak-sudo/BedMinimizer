#pragma once

#include <unordered_map>
#include <string>

#include "CertInfo.h"
#include "RawCertificate.h"

namespace ReadDimacs
{
    std::pair<std::string, std::vector<long>> parseSubEntry(const std::string &entry);
    std::string readFile(const std::string &filename);
    std::vector<long> getVariables(const std::string &output);
    RawPartCertificate parseVariables(const std::vector<long> &trueVars, const std::vector<std::string> &sub);
    RawPartCertificate readAndParseRawCertificate(const std::string &satResPath, const std::vector<std::string> &sub);
    CertInfo getCertInfoFromSub(const std::vector<std::string> &sub);
};