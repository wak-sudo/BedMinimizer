#pragma once

#include "CnfAst.h"

#include <unordered_map>
#include <ostream>

class DimacsWriter
{
private:
    static std::unordered_map<std::string, long> GenIdMap(const Cnf::Formula &f);
    static std::unordered_map<std::string, long> ToStream(std::ostream &stream, const Cnf::Formula &f);
    static void ToStream(std::ostream &stream, const std::unordered_map<std::string, long> &varToId, const Cnf::Formula &f);

public:
    static std::unordered_map<std::string, long> ToFile(const std::string &filePath, const Cnf::Formula &f);
    static std::pair<std::string, std::unordered_map<std::string, long>> ToString(const Cnf::Formula &f);
    static std::vector<std::string> ReversMap(const std::unordered_map<std::string, long> &varToId);
};
