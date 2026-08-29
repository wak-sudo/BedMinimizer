#include "Serializer.h"
#include "SerializerPrivate.h"

#include "InputInstance.h"

#include <fstream>
#include <vector>

#include <iostream>

using namespace std;

nlohmann::json Serializer::serialize(const InputInstance &instance)
{
    nlohmann::json j = SerializerPrivate::serialize((InputInstanceBasic)(instance));

    j["substitution"] = instance.sub.data();

    return j;
}

void Serializer::saveToFile(
    const std::string &filePath,
    const InputInstance &instance)
{
    std::ofstream file(filePath);

    if (!file)
        throw std::runtime_error("Cannot open file: " + filePath);

    file << serialize(instance).dump(4);
}

InputInstance SerializerPrivate::deserializeInputInstance(const nlohmann::json &jInstance)
{
    InputInstanceBasic basicInstance = deserializeInputInstanceBasic(jInstance);
    const auto sub = jInstance.at("substitution").get<vector<string>>();

    return InputInstance{
        basicInstance.originalDfa,
        basicInstance.components,
        basicInstance.paths,
        Substitution(sub)};
}

InputInstance Serializer::deserializeInputInstanceFromFile(const std::string &filePath)
{
    const nlohmann::json j = SerializerPrivate::readFile(filePath);
    return SerializerPrivate::deserializeInputInstance(j);
}