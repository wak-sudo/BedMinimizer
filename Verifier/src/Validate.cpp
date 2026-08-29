#include "Checker.h"

#include "Utility.h"

#include <format>

using namespace std;

std::vector<long> Checker::validateAssignedComponents() const
{
    const long statesCount = canReachFinal_.size();

    vector<bool> stateAlreadyChecked(statesCount, false);

    for (size_t componentId = 0; componentId < cert_.guessed.dagNodes.size(); componentId++)
    {
        const auto &component = cert_.guessed.dagNodes.at(componentId);

        if (component.empty())
            throw invalid_argument(format("Component {0} is empty.", componentId));

        const long compSize = component.size();

        for (long i = 0; i < compSize; i++)
        {
            const long fstVertex = component.at(i);

            if (fstVertex < 0 || fstVertex >= statesCount)
                throw invalid_argument(format("Invalid state {0} in component {1}.", fstVertex, componentId));

            if (stateAlreadyChecked.at(fstVertex))
                throw invalid_argument(format("State {0} is assigned to multiple components.", fstVertex));

            stateAlreadyChecked.at(fstVertex) = true;

            if (!canReachFinal_.at(fstVertex))
            {
                throw invalid_argument(format(
                    "In component {0}, from the state {1} it is impossible to reach an accepting state.",
                    componentId,
                    fstVertex));
            }

            for (long j = i + 1; j < compSize; j++)
            {
                const long sndVertex = component.at(j);

                if (sndVertex < 0 || sndVertex >= statesCount)
                    throw invalid_argument(format("Invalid state {0} in component {1}.", sndVertex, componentId));

                if (!dfaReachibilityByWord_.at(fstVertex).at(sndVertex) ||
                    !dfaReachibilityByWord_.at(sndVertex).at(fstVertex))
                {
                    throw invalid_argument(format(
                        "In component {0}, states {1} and {2} are not mutually reachable.",
                        componentId,
                        fstVertex,
                        sndVertex));
                }
            }
        }
    }

    vector<long> unassignedStates;
    unassignedStates.reserve(statesCount);

    for (long i = 0; i < statesCount; ++i)
    {
        if (!stateAlreadyChecked.at(i))
            unassignedStates.push_back(i);
    }

    return unassignedStates;
}

void Checker::validateUnassignedStates(const vector<long> &unassignedStates) const
{
    const long statesCount = dfaReachibilityByWord_.size();

    for (long stateId : unassignedStates)
    {
        if (stateId < 0 || stateId >= statesCount)
            throw invalid_argument(format("Invalid unassigned state {0}.", stateId));

        if (!canReachFinal_.at(stateId))
            continue;

        for (long otherState = 0; otherState < statesCount; otherState++)
        {
            if (dfaReachibilityByWord_.at(stateId).at(otherState) &&
                dfaReachibilityByWord_.at(otherState).at(stateId))
            {
                throw invalid_argument(format(
                    "State {0} is mutually reachable with {1} and can reach an accepting state, but is assigned to no component.",
                    stateId,
                    otherState));
            }
        }
    }
}