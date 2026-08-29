#include "Checker.h"

#include "Utility.h"

#include <stdexcept>
#include <format>

using namespace std;

void Checker::check() const
{
    checkComponents();
    checkCovers();
    checkContinuations();
    checkPathsCovering();
}

void Checker::checkDfa() const
{
    const long acceptingStatesCount = cert_.guessed.acceptingStates.size();

    if (acceptingStatesCount == 0)
        throw invalid_argument("No accepting states in the DFA");

    const size_t statesCount = cert_.guessed.trans.size();
    const size_t alphabetSize = cert_.given.originalDfa.alphabetSize();

    for (size_t i = 0; i < statesCount; i++)
    {
        if (cert_.guessed.trans.at(i).size() != alphabetSize)
            throw invalid_argument(format(
                "DFA is not complete for a source state {0}",
                i));

        for (size_t letter = 0; letter < alphabetSize; letter++)
        {
            const long destination = cert_.guessed.trans.at(i).at(letter);

            if (destination < 0 || (size_t)destination >= statesCount)
                throw invalid_argument(format(
                    "Invalid transition from state {0} with letter {1} to state {2}",
                    i,
                    letter,
                    destination));
        }
    }

    for (long acceptingState : cert_.guessed.acceptingStates)
    {
        if (acceptingState < 0 || (size_t)acceptingState >= statesCount)
            throw invalid_argument(format(
                "Invalid accepting state {0}",
                acceptingState));
    }
}

void Checker::checkComponents() const
{
    vector<long> unassignedStates = validateAssignedComponents();
    validateUnassignedStates(unassignedStates);
}

void Checker::checkCovers() const
{
    const long nodesCount = cert_.guessed.dagNodes.size();
    for (long nodeId = 0; nodeId < nodesCount; nodeId++)
    {
        for (long pathId : cert_.guessed.covers.at(nodeId))
        {
            if (!pathCanBelongToComponent(pathId, nodeId))
                throw invalid_argument(format("Path {0} cannot belong to a component {1}", pathId, nodeId));
        }
    }
}

void Checker::checkContinuations() const
{
    const long nodesCount = dagGraph_.size();
    for (long src = 0; src < nodesCount; src++)
    {
        for (long dest : dagGraph_.at(src))
        {
            for (long pathIdsrc : cert_.guessed.covers.at(src)) // can be done much faster but there is no bottleneck right now.
            {
                bool foundCont = false;
                for (long pathIddest : cert_.guessed.covers.at(dest))
                {
                    if (isPrefix(pathIdsrc, pathIddest))
                    {
                        foundCont = true;
                        break;
                    }
                }
                if (!foundCont)
                    throw invalid_argument(format("In node {0} path {1}, does not have a continuatio in node {2}",
                                                  src, pathIdsrc, dest));
            }
        }
    }
}

void Checker::checkPathsCovering() const
{
    const size_t pathsCount = cert_.given.paths.size();

    if (cert_.guessed.coveringPaths.size() != pathsCount)
        throw invalid_argument("Number of covering paths does not match number of paths");

    for (size_t coveredPathId = 0; coveredPathId < pathsCount; coveredPathId++)
    {
        checkGuessedPathExists(cert_.guessed.coveringPaths.at(coveredPathId));
        checkPathCovered(coveredPathId, cert_.guessed.coveringPaths.at(coveredPathId));
    }
}