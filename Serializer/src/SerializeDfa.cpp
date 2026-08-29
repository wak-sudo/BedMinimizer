#include "Serializer.h"
#include "SerializerPrivate.h"

#include <fstream>
#include <vector>

using namespace std;

nlohmann::json Serializer::serialize(const DFA &dfa)
{
    nlohmann::json j;

    const auto &trans = dfa.transFun();
    const long alphabetSize = trans.empty() ? 0 : trans[0].size();

    j["states_count"] = trans.size();
    j["alphabet_size"] = alphabetSize;
    j["initial_state"] = dfa.originalState();
    j["accept_states"] = dfa.acceptingStates();
    j["transitions"] = trans;

    return j;
}

void Serializer::saveToFile(const std::string &filePath, const DFA &dfa)
{
    std::ofstream file(filePath);
    if (!file)
        throw std::runtime_error("Cannot open file: " + filePath);

    file << serialize(dfa).dump(4);
}

DFA SerializerPrivate::deserializeDFA(const nlohmann::json &jDfa)
{
    const long initialState = jDfa.at("initial_state").get<long>();
    const auto acceptStates = jDfa.at("accept_states").get<std::vector<long>>();
    const auto trans = jDfa.at("transitions").get<std::vector<std::vector<long>>>();

    return DFA(trans, acceptStates, initialState);
}

DFA Serializer::deserializeDfaFromFile(const std::string &filePath)
{
    const nlohmann::json j = SerializerPrivate::readFile(filePath);
    return SerializerPrivate::deserializeDFA(j);
}

void Serializer::saveDfaAsDot(const std::string &filePath, const DFA &dfa)
{
    std::ofstream file(filePath);

    if (!file)
        throw std::runtime_error("Cannot open file: " + filePath);

    file << "digraph DFA {\n";
    file << "    rankdir=LR;\n";

    for (long state = 0; state < dfa.statesCount(); state++)
    {
        file << "    " << state;

        if (state == dfa.originalState())
            file << " [shape=circle, style=bold";
        else
            file << " [shape=circle";

        if (dfa.stateIsAccepting(state))
            file << ", peripheries=2";

        file << "];\n";
    }

    file << "    start [shape=point];\n";
    file << "    start -> " << dfa.originalState() << ";\n";


    const auto &transFun = dfa.transFun();

    for (long state = 0; state < dfa.statesCount(); state++)
    {
        for (long letter = 0; letter < dfa.alphabetSize(); letter++)
        {
            const long target = transFun.at(state).at(letter);

            file << "    "
                 << state
                 << " -> "
                 << target
                 << " [label=\""
                 << letter
                 << "\"];\n";
        }
    }

    file << "}\n";
}