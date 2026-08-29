#pragma once

#include "DFA.h"
#include "InputInstance.h"
#include "SolverResult.h"
#include "Certificate.h"
#include "Substitution.h"

#include <nlohmann/json.hpp>

namespace Serializer
{
    DFA deserializeDfaFromFile(const std::string &filePath);
    nlohmann::json serialize(const DFA &dfa);
    void saveToFile(const std::string &filePath, const DFA &dfa);
    void saveDfaAsDot(const std::string &filePath, const DFA &dfa);

    Certificate deserializeCertificateFromFile(const std::string &filePath);
    nlohmann::json serialize(const Certificate &certificate);
    void saveToFile(const std::string &filePath, const Certificate &certificate);

    nlohmann::json serialize(const InputInstance &instance);
    void saveToFile(const std::string &filePath, const InputInstance &instance);
    InputInstance deserializeInputInstanceFromFile(const std::string &filePath);
}