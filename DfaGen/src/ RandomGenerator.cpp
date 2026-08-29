#include "RandomGenerator.h"
#include "RandomDagGenerator.h"
#include "RandomDfaGenerator.h"

#include <random>
#include <stdexcept>

using namespace std;

DFA RandomGenerator::generate(long numberOfStates, long numberOfNodes, long depth, long alphabetSize)
{
    GeneratedDag dag;

    dag = RandomDagGenerator().generate(
        numberOfStates - 1, // sink omitted
        numberOfNodes,
        depth);

    auto dfa = RandomDfaGenerator(dag, alphabetSize).generate();
    return dfa;
}