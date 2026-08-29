#include "ReadDimacs.h"
#include "ResParser.h"

#include "CodingStringTable.h"

#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "Utility.h"

using namespace std;
using namespace Instances;

string ReadDimacs::readFile(const string &filename)
{
    ifstream file(filename, ios::binary | ios::ate);

    if (!file)
        throw runtime_error("Cannot open file: " + filename);

    const streamsize size = file.tellg();

    if (size < 0)
        throw runtime_error("Cannot determine file size: " + filename);

    string content;
    content.reserve(static_cast<size_t>(size));

    file.seekg(0, ios::beg);

    content.assign(
        istreambuf_iterator<char>(file),
        istreambuf_iterator<char>());

    return content;
}

vector<long> ReadDimacs::getVariables(const string &content)
{
    vector<long> result;

    istringstream stream(content);
    string line;
    bool firstLine = true;

    while (getline(stream, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }

        if (line.empty() || line[0] != 'v')
            continue;

        istringstream lineStream(line);
        char v;
        long literal;

        lineStream >> v;

        while (lineStream >> literal && literal != 0)
        {
            result.push_back(literal);
        }
    }

    return result;
}

std::pair<std::string, vector<long>> ReadDimacs::parseSubEntry(const std::string &entry)
{
    std::stringstream ss(entry);
    std::string token;

    std::getline(ss, token, '_');
    std::string name = token;

    std::vector<long> values;

    while (std::getline(ss, token, '_'))
    {
        values.push_back(std::stol(token));
    }

    return {name, values};
}

RawPartCertificate ReadDimacs::parseVariables(
    const std::vector<long> &vars,
    const std::vector<std::string> &sub)
{
    RawPartCertificate cert;

    for (const long varIdx : vars)
    {
        const auto &tag = sub.at(abs(varIdx));
        const auto [name, args] = parseSubEntry(tag);
        const bool isSetToTrue = varIdx > 0;

        if (isSetToTrue)
        {
            if (name == CodingStringTable::final)
            {
                cert.acceptingStates.emplace_back(args.at(0));
            }
            else if (name == CodingStringTable::origin)
            {
                cert.origin = args.at(0);
            }
            else if (name == CodingStringTable::trans)
            {
                const long from = args.at(0);
                const long by = args.at(1);
                const long to = args.at(2);

                cert.trans.emplace_back(TransInstance{
                    .from = from,
                    .by = by,
                    .to = to});
            }
            else if (name == CodingStringTable::belongs)
            {
                cert.dagNodes.emplace_back(DagNodeInstance{
                    .nodeId = args.at(0),
                    .includedState = args.at(1)});
            }
            else if (name == CodingStringTable::cover)
            {
                cert.covers.emplace_back(CoversInstance{
                    .nodeId = args.at(0),
                    .pathId = args.at(1)});
            }
        }

        // read even if the variable is negated,
        // to take zero into the account.
        if (name == CodingStringTable::coveringPath)
        {
            cert.coveringPaths.emplace_back(CoveringPathInstance{
                .pathId = args.at(0),
                .nodeRank = args.at(1),
                .bitRank = args.at(2),
                .bitVal = isSetToTrue});
        }
    }

    return cert;
}

CertInfo ReadDimacs::getCertInfoFromSub(const std::vector<std::string> &sub)
{
    RawPartCertificate cert;

    long maxLetter = -1;
    long maxStateId = -1;
    long maxPathId = -1;

    for (const auto &entry : sub)
    {
        const auto [name, args] = parseSubEntry(entry);

        if (name == CodingStringTable::trans)
        {
            const long from = args.at(0);
            const long by = args.at(1);

            maxLetter = max(maxLetter, by);
            maxStateId = max(maxStateId, from);
        }
        else if (name == CodingStringTable::cover)
        {
            const long pathId = args.at(1);
            maxPathId = max(maxPathId, pathId);
        }
    }

    return CertInfo{
        .alphabetSize = maxLetter + 1,
        .targetStatesCount = maxStateId + 1,
        .orgPathsCount = maxPathId + 1};
}

RawPartCertificate ReadDimacs::readAndParseRawCertificate(const std::string &satResPath, const std::vector<std::string> &sub)
{
    const auto content = ReadDimacs::readFile(satResPath);
    const auto trueVars = ReadDimacs::getVariables(content);
    return ReadDimacs::parseVariables(trueVars, sub);
}
