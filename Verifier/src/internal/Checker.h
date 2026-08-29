#pragma once

#include "Certificate.h"
#include "InputInstance.h"

#include <memory>

class Checker
{
private:
    // provided:
    Certificate cert_;

    // generated:
    std::vector<bool> canReachFinal_;
    std::vector<std::vector<bool>> dfaReachibilityByWord_;
    std::vector<std::vector<long>> dagGraph_;

    void checkComponents() const;

    void checkDfa() const;

    void checkCovers() const;

    void checkPathsCovering() const;

    void checkContinuations() const;

    bool isPrefix(const long, const long) const;

    std::vector<long> validateAssignedComponents() const;

    void validateUnassignedStates(
        const std::vector<long> &unassignedStates) const;

    bool pathCanBelongToComponent(const long pathId, const long targetComponentId) const;

    bool checkPathCovered(
        const long coveredPathId,
        const std::vector<long> &coveringPath) const;

    bool checkGuessedPathExists(const std::vector<long> &coveringPath) const;

    static std::vector<std::vector<long>> genEdges(
        const std::vector<std::vector<long>> &components,
        const std::vector<std::vector<bool>> &dfaReachibilityByWord);

    long getLastNodeId(long pathId) const;

public:
    Checker(Certificate cert);
    void check() const;
};