#include "PartialDFA.h"
#include "Serializer.h"
#include "SerializerPrivate.h"

#include <fstream>
#include <vector>

using namespace std;

InputInstanceBasic SerializerPrivate::deserializeInputInstanceBasic(
    const nlohmann::json &jInputBasic)
{
    return InputInstanceBasic{
        deserializeDFA(jInputBasic.at("original_dfa")),
        jInputBasic.at("components").get<std::vector<std::vector<long>>>(),
        jInputBasic.at("paths").get<std::vector<std::vector<long>>>()};
}

nlohmann::json SerializerPrivate::serialize(const InputInstanceBasic &instance)
{
    return nlohmann::json{
        {"original_dfa", Serializer::serialize(instance.originalDfa)},
        {"components", instance.components},
        {"paths", instance.paths}};
}