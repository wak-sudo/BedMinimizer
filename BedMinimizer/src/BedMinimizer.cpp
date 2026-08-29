#include "BedMinimizer.h"

#include "Serializer.h"
#include "Verify.h"
#include "AugSccGraph.h"
#include "CodeGen.h"
#include "ResParser.h"
#include "InputInstance.h"
#include "SolverResult.h"
#include "Certificate.h"

#include <memory>
#include <iostream>

using namespace std;

void BedMin::generateCoding(
    const std::string &dfaFilePath,
    unsigned long targetSize,
    const std::string &codingDestFilePath,
    const std::string &instanceDestFilePath)
{
    const auto dfa = Serializer::deserializeDfaFromFile(dfaFilePath);
    const auto graphPtr = std::make_shared<AugSccGraph>(AugSccGraph(dfa));

    const auto sub = CodeGen::genDimacsCodingToFile(graphPtr, targetSize, codingDestFilePath);

    const auto ctx = InputInstance(graphPtr->getOriginalDfa(), graphPtr->getComponents(), graphPtr->paths(), sub);

    Serializer::saveToFile(instanceDestFilePath, ctx);
}

void BedMin::createCertificate(
    const std::string &instFilePath, const std::string &satResFilePath, const std::string &destCertFilePath, const std::string &destDfaFilePath)
{
    const auto inp = Serializer::deserializeInputInstanceFromFile(instFilePath);
    const auto res = ResParser::parseSolverResultsFromFile(satResFilePath, inp.sub.data());

    DFA dfa(res.trans, res.acceptingStates, res.origin);

    const auto cert = Certificate{
        .guessed = res,
        .given = inp};

    Serializer::saveToFile(destCertFilePath, cert);
    Serializer::saveToFile(destDfaFilePath, dfa);
}

std::optional<std::string> BedMin::verify(const std::string &certFilePath)
{
    const auto cert = Serializer::deserializeCertificateFromFile(certFilePath);
    return Verify::checkCertificate(cert);
}