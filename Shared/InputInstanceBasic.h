#pragma once

#include <vector>

#include "DFA.h"

struct InputInstanceBasic
{
    DFA originalDfa;
    std::vector<std::vector<long>> components; // Information on how the components are numbered
    std::vector<std::vector<long>> paths;      // Information on how the paths are numbered

    InputInstanceBasic(
        DFA originalDfaArg,
        std::vector<std::vector<long>> componentsArg,
        std::vector<std::vector<long>> pathsArg) :
        originalDfa(std::move(originalDfaArg)), 
        components(std::move(componentsArg)),
        paths(std::move(pathsArg)) {}
};