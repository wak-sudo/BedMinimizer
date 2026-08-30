#include "RandomDfaGenerator.h"
#include "RandomGeneratorExp.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cassert>

#include "Utility.h"

using namespace std;

RandomDfaGenerator::RandomDfaGenerator(GeneratedDag dag, long alphabetSize)
    : dag_(dag), alphabetSize_(alphabetSize)
{
    if (alphabetSize < 2)
        throw invalid_argument("Alphabet size must be at least 2.");
    std::random_device rd;
    rng_ = std::mt19937(rd());

    offsets_ = calculateOffsets(dag_.statesPerNode);
    freeLetter_ = alphabetSize - 1;
    baseNumberOfStates_ = accumulate(
        dag_.statesPerNode.begin(),
        dag_.statesPerNode.end(),
        0L);
    globalSink_ = baseNumberOfStates_;
}

DFA RandomDfaGenerator::generate()
{
    auto localTransArr = generateLocalTrans();

    vector<vector<long>> transFun(baseNumberOfStates_, vector<long>(alphabetSize_));

    // Copy standard transitions
    // from local automata.
    for (size_t v = 0; v < localTransArr.size(); v++)
    {
        const auto &localTrans = localTransArr.at(v);

        for (long state = 0; state < dag_.statesPerNode.at(v); state++)
        {
            for (long letter = 0; letter < alphabetSize_ - 1; letter++)
            {
                transFun.at(offsets_.at(v) + state).at(letter) =
                    offsets_.at(v) +
                    localTrans.at(state).at(letter);
            }
        }
    }

    setTransitions(transFun);

    // Add the sink
    transFun.push_back(vector<long>(alphabetSize_, globalSink_));

    vector<long> acceptingStates;

    setAcceptingStates(acceptingStates);

    const long initialState = generateInitialState();

    shuffleLetters(transFun);

    return DFA(transFun, acceptingStates, initialState);
}

void RandomDfaGenerator::shuffleLetters(vector<vector<long>> &trans)
{
    const size_t statesCount = trans.size();
    for (size_t state = 0; state < statesCount; state++)
    {
        vector<long> destinations = trans[state];
        shuffle(destinations.begin(), destinations.end(), rng_);
        trans[state] = destinations;
    }
}

vector<vector<vector<long>>> RandomDfaGenerator::generateLocalTrans()
{
    vector<vector<vector<long>>> result;
    result.reserve(dag_.statesPerNode.size());

    const long localAlphabetSize = alphabetSize_ - 1;
    constexpr long maxAttempts = 10000;

    for (long states : dag_.statesPerNode)
    {
        long attempts = 0;

        while (attempts < maxAttempts)
        {
            auto trans = randomTransFun(states, localAlphabetSize);
            attempts++;

            if (Utility::isSingleScc(trans))
            {
                result.push_back(move(trans));
                break;
            }
        }

        if (attempts == maxAttempts)
        {
            throw RandomGeneratorExp(
                "Failed to generate a strongly connected DFA after " +
                to_string(maxAttempts) +
                " attempts.");
        }
    }

    return result;
}

vector<long> RandomDfaGenerator::calculateOffsets(
    const vector<long> &statesPerNode)
{
    vector<long> offsets(statesPerNode.size());

    long offset = 0;

    for (size_t v = 0; v < statesPerNode.size(); v++)
    {
        offsets.at(v) = offset;
        offset += statesPerNode.at(v);
    }

    return offsets;
}

void RandomDfaGenerator::connectRandom(vector<vector<long>> &trans, long src, long dest)
{
    uniform_int_distribution<long> letterDist(0, alphabetSize_ - 1);
    const long randomLetter = letterDist(rng_);
    if (randomLetter == freeLetter_)
        trans.at(src).at(freeLetter_) = dest;
    else
    {
        trans.at(src).at(freeLetter_) = trans.at(src).at(randomLetter);
        trans.at(src).at(randomLetter) = dest;
    }
}

void RandomDfaGenerator::setTransitions(vector<vector<long>> &transFun)
{
    const size_t nodesCount = dag_.edges.size();
    for (size_t v = 0; v < nodesCount; v++)
    {
        const size_t statesCount = dag_.statesPerNode.at(v);
        const auto &children = dag_.edges.at(v);

        vector<long> stateOrder(statesCount);
        iota(stateOrder.begin(), stateOrder.end(), 0);
        shuffle(stateOrder.begin(), stateOrder.end(), rng_);

        // There must be a connection from a unqiue state to each child
        for (size_t i = 0; i < children.size(); i++)
        {
            const long source = stateOrder.at(i);
            const long child = children.at(i);

            uniform_int_distribution<long> targetDist(0, dag_.statesPerNode.at(child) - 1);

            transFun.at(offsets_.at(v) + source).at(freeLetter_) = offsets_.at(child) + targetDist(rng_);
        }

        // there must be a connection to the sink
        const long fromState = stateOrder.at(children.size());
        transFun.at(offsets_.at(v) + fromState).at(freeLetter_) = globalSink_;

        for (size_t i = children.size() + 1; i < statesCount; i++)
        {
            const long source = stateOrder.at(i);
            setRandomDestination(transFun, v, source);
        }
    }
}

void RandomDfaGenerator::setRandomDestination(
    vector<vector<long>> &transFun,
    long node,
    long source)
{
    const auto &children = dag_.edges.at(node);

    uniform_int_distribution<long> innerTarget(0, dag_.statesPerNode.at(node) - 1);
    uniform_real_distribution<double> choiceDist(0.0, 1.0);
    const long srcState = offsets_.at(node) + source;
    const double r = choiceDist(rng_);
    long destination;

    // no children
    if (children.empty())
    {
        if (r < 0.5) // connect inside
            destination = offsets_.at(node) + innerTarget(rng_);
        else // connect outside
            destination = globalSink_;
    }
    else
    {
        uniform_int_distribution<long> childDist(0, children.size() - 1);
        if (r < 0.50) // 50% trans. to an innter state, 25% to a child, 25% to the sink.
        {
            destination = offsets_.at(node) + innerTarget(rng_);
        }
        else if (r < 0.75)
        {
            const long child = children.at(childDist(rng_));

            uniform_int_distribution<long> childStatesDist(0, dag_.statesPerNode.at(child) - 1);

            destination = offsets_.at(child) + childStatesDist(rng_);
        }
        else
        {
            destination = globalSink_;
        }
    }

    transFun.at(srcState).at(freeLetter_) = destination;
}

void RandomDfaGenerator::setAcceptingStates(
    vector<long> &acceptingStates)
{
    bernoulli_distribution acceptingDist(0.5);

    for (size_t v = 0; v < dag_.edges.size(); v++)
    {
        const long states = dag_.statesPerNode.at(v);
        const bool isLeaf = dag_.edges.at(v).empty();

        bool hasAcceptingState = false;

        for (long state = 0; state < states; state++)
        {
            if (acceptingDist(rng_))
            {
                acceptingStates.push_back(offsets_.at(v) + state);
                hasAcceptingState = true;
            }
        }

        // Every leaf must have an accepting state.
        if (isLeaf && !hasAcceptingState)
        {
            uniform_int_distribution<long> dist(0, states - 1);

            acceptingStates.push_back(offsets_.at(v) + dist(rng_));
        }
    }
}

long RandomDfaGenerator::generateInitialState()
{
    uniform_int_distribution<long> dist(0, dag_.statesPerNode.at(0) - 1);

    return offsets_.at(0) + dist(rng_);
}

vector<vector<long>> RandomDfaGenerator::randomTransFun(long n, long alphabetSize)
{
    vector<vector<long>> trans(n, vector<long>(alphabetSize));

    uniform_int_distribution<long> stateDist(0, n - 1);

    for (long q = 0; q < n; q++)
    {
        for (long a = 0; a < alphabetSize; a++)
        {
            trans[q][a] = stateDist(rng_);
        }
    }

    return trans;
}