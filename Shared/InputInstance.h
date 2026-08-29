#pragma once

#include <vector>

#include "InputInstanceBasic.h"
#include "Substitution.h"

struct InputInstance : public InputInstanceBasic
{
    Substitution sub;

    InputInstance(
        DFA originalDfaArg,
        std::vector<std::vector<long>> componentsArg,
        std::vector<std::vector<long>> pathsArg,
        Substitution subArg) : InputInstanceBasic(std::move(originalDfaArg), std::move(componentsArg), std::move(pathsArg)), sub(std::move(subArg)) {}
};