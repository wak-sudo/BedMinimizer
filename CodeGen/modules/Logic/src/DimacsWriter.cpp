
#include "DimacsWriter.h"
#include "Tseytin.h"

#include <cmath>
#include <unordered_set>
#include <fstream>
#include <ostream>
#include <sstream>

using namespace std;

unordered_map<string, long> DimacsWriter::GenIdMap(const Cnf::Formula &f)
{
    unordered_map<string, long> varToId;
    long counter = 1;

    const auto &clauses = f.clauses();

    for (const auto &c : clauses)
    {
        const auto &literals = c.literals();
        for (const auto &l : literals)
        {
            const string &name = l.name();
            if (!varToId.contains(name))
            {
                varToId[name] = counter;
                counter++;
            }
        }
    }

    return varToId;
}

std::unordered_map<std::string, long> DimacsWriter::ToStream(ostream &stream, const Cnf::Formula &f)
{
    const auto varToId = GenIdMap(f);
    ToStream(stream, varToId, f);
    return varToId;
}

void DimacsWriter::ToStream(ostream &stream, const unordered_map<string, long>& varToId, const Cnf::Formula &f)
{
    const auto &clauses = f.clauses();
    const long clauseCount = clauses.size();
    const long variablesCount = varToId.size();

    stream << "p cnf " << variablesCount << " " << clauseCount << "\n";

    for (const auto &c : clauses)
    {
        const auto &literals = c.literals();
        for (const auto &l : literals)
        {
            if (l.negated())
                stream << "-";
            stream << varToId.at(l.name()) << " ";
        }
        stream << "0\n";
    }
}

std::unordered_map<std::string, long> DimacsWriter::ToFile(const string &filePath, const Cnf::Formula &f)
{
    ofstream of(filePath);
    if (!of.good())
        throw invalid_argument("Could not open a file: " + filePath);

    constexpr size_t STREAM_BUFF_SIZE = 1024 * 1024; // 1 MiB
    std::vector<char> buffer(STREAM_BUFF_SIZE);
    of.rdbuf()->pubsetbuf(buffer.data(), buffer.size());

    const auto varToId = ToStream(of, f);

    of.flush();

    return varToId;
}

pair<std::string, std::unordered_map<std::string, long>> DimacsWriter::ToString(const Cnf::Formula &f)
{
    const auto varToId = GenIdMap(f);

    std::ostringstream oss;

    ToStream(oss, varToId, f);

    return make_pair(oss.str(), std::move(varToId));
}

std::vector<std::string> DimacsWriter::ReversMap(const std::unordered_map<std::string, long>& varToId)
{
    const long varsCount = varToId.size();
    vector<string> res(varsCount+1); // as 0 is reserved.
    for(const auto& entry : varToId)
    {
        const long idx = entry.second;
        const string& text = entry.first;
        res.at(idx) = text;
    }
    return res;
}