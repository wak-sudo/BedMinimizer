#include "ResParser.h"
#include "Translate.h"
#include "ReadDimacs.h"

SolverResult ResParser::parseSolverResultsFromFile(const std::string &satResPath, const std::vector<std::string> &sub)
{
    const auto rw = ReadDimacs::readAndParseRawCertificate(satResPath, sub);
    const auto info = ReadDimacs::getCertInfoFromSub(sub);
    return Translate(std::move(rw), std::move(info)).pruneRawCertificate();
}