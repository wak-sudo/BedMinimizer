#pragma once

#include <unordered_map>
#include <memory>

#include "IPreCalc.h"
#include "Substitution.h"

namespace CodeGen
{
    Substitution genDimacsCodingToFile(std::shared_ptr<const IPreCalc> precalcObj, unsigned long, const std::string &filePath);
    Substitution genDimacsCodingToFileContains(std::shared_ptr<const IPreCalc> precalcObj, unsigned long m, const std::string &filePath);
    Substitution genDimacsCodingToFileContained(std::shared_ptr<const IPreCalc> precalcObj, unsigned long m, const std::string &filePath);
    std::pair<std::string, Substitution> genDimacsCodingToStr(std::shared_ptr<const IPreCalc> precalcObj, unsigned long targetSize);
}
