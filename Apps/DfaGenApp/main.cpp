#include <iostream>
#include <string>

#include "RandomGenerator.h"
#include "Serializer.h"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <dfa_states> <alphabet_size> <scc_nodes_count> <scc_depth>\n";
        return 1;
    }

    const long dfaStates = std::stoi(std::string(argv[1]));
    const long alphabetSize = std::stoi(std::string(argv[2]));
    const long sccNodesCount = std::stoi(std::string(argv[3]));
    const long sccDepth = std::stoi(std::string(argv[4]));

    const std::string dfaRes = "generatedDfa.json";

    const auto dfa = RandomGenerator::generate(dfaStates, sccNodesCount, sccDepth, alphabetSize);
    Serializer::saveToFile(dfaRes, dfa);

    return 0;
}