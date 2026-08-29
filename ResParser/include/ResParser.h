#pragma once

#include <string>
#include <vector>

#include "SolverResult.h"

namespace ResParser
{
   SolverResult parseSolverResultsFromFile(const std::string &satResPath, const std::vector<std::string> &sub);
}