#pragma once

#include <nlohmann/json.hpp>
#include "DFA.h"
#include "Substitution.h"
#include "InputInstance.h"
#include "PartialDFA.h"
#include "SolverResult.h"

#include <map>
#include <string>

namespace SerializerPrivate
{
    nlohmann::json readFile(const std::string &filePath);
    DFA deserializeDFA(const nlohmann::json &jDfa);
   
    InputInstance deserializeInputInstance(const nlohmann::json &jInstance);

    InputInstanceBasic deserializeInputInstanceBasic(const nlohmann::json& jInstanceBasic);
    nlohmann::json serialize(const InputInstanceBasic & instance);

    SolverResult deserializeSolverResult(const nlohmann::json &jSolverRes);
    nlohmann::json serialize(const SolverResult & solverRes);
}