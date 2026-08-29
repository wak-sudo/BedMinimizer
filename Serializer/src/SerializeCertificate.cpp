#include "Serializer.h"
#include "SerializerPrivate.h"

using namespace std;

#include <fstream>

Certificate Serializer::deserializeCertificateFromFile(
    const std::string &filePath)
{
    nlohmann::json j = SerializerPrivate::readFile(filePath);

    return Certificate{
        .guessed = SerializerPrivate::deserializeSolverResult(j.at("guessed")),
        .given = SerializerPrivate::deserializeInputInstanceBasic(j.at("given"))};
}

nlohmann::json Serializer::serialize(
    const Certificate &certificate)
{
    return nlohmann::json{
        {"guessed", SerializerPrivate::serialize(certificate.guessed)},
        {"given", SerializerPrivate::serialize(certificate.given)}};
}

void Serializer::saveToFile(
    const std::string &filePath,
    const Certificate &certificate)
{
    std::ofstream file(filePath);

    if (!file)
        throw std::runtime_error(
            "Cannot open file: " + filePath);

    file << serialize(certificate).dump(4);
}