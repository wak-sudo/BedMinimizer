#include "Utility.h"

#include <vector>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;

DFA Utility::minimizeDFA(const DFA &dfa)
{
    const long n = dfa.statesCount();
    const long alphabetSize = dfa.alphabetSize();
    const long origState = dfa.originalState();

    if (n == 0)
        return dfa;

    const auto &trans = dfa.transFun();

    std::vector<bool> reachable(n, false);
    std::queue<long> q;
    q.push(origState);
    reachable[origState] = true;

    while (!q.empty())
    {
        long u = q.front();
        q.pop();

        for (long a = 0; a < alphabetSize; ++a)
        {
            long v = trans[u][a];
            if (v != -1 && !reachable[v])
            {
                reachable[v] = true;
                q.push(v);
            }
        }
    }

    std::vector<long> F, nonF;
    for (long i = 0; i < n; ++i)
    {
        if (!reachable[i])
            continue;
        if (dfa.stateIsAccepting(i))
            F.push_back(i);
        else
            nonF.push_back(i);
    }

    std::vector<std::vector<long>> P;
    if (!F.empty())
        P.push_back(F);
    if (!nonF.empty())
        P.push_back(nonF);

    std::vector<std::vector<long>> W = P;

    std::vector<std::vector<std::vector<long>>> invTrans(alphabetSize, std::vector<std::vector<long>>(n));
    for (long u = 0; u < n; ++u)
    {
        if (!reachable[u])
            continue;
        for (long c = 0; c < alphabetSize; ++c)
        {
            long v = trans[u][c];
            if (v != -1 && reachable[v])
            {
                invTrans[c][v].push_back(u);
            }
        }
    }

    while (!W.empty())
    {
        auto A = W.back();
        W.pop_back();

        for (long c = 0; c < alphabetSize; ++c)
        {
            std::set<long> X_set;
            for (long p : A)
            {
                for (long u : invTrans[c][p])
                {
                    X_set.insert(u);
                }
            }
            if (X_set.empty())
                continue;

            std::vector<std::vector<long>> newP;
            for (const auto &Y : P)
            {
                std::vector<long> Y1, Y2;
                for (long state : Y)
                {
                    if (X_set.count(state))
                        Y1.push_back(state);
                    else
                        Y2.push_back(state);
                }

                if (!Y1.empty() && !Y2.empty())
                {
                    newP.push_back(Y1);
                    newP.push_back(Y2);

                    auto it = std::find(W.begin(), W.end(), Y);
                    if (it != W.end())
                    {
                        W.erase(it);
                        W.push_back(Y1);
                        W.push_back(Y2);
                    }
                    else
                    {
                        W.push_back(Y1.size() <= Y2.size() ? Y1 : Y2);
                    }
                }
                else
                {
                    newP.push_back(Y);
                }
            }
            P = std::move(newP);
        }
    }

    const long minStatesCount = P.size();
    std::vector<long> stateToClass(n, -1);
    long newOrigState = -1;

    for (long i = 0; i < minStatesCount; ++i)
    {
        for (long st : P[i])
        {
            stateToClass[st] = i;
            if (st == origState)
            {
                newOrigState = i;
            }
        }
    }

    std::vector<std::vector<long>> minTrans(minStatesCount, std::vector<long>(alphabetSize, -1));
    std::vector<long> minAccepting;

    for (long i = 0; i < minStatesCount; ++i)
    {
        long rep = P[i][0];
        if (dfa.stateIsAccepting(rep))
        {
            minAccepting.push_back(i);
        }

        for (long c = 0; c < alphabetSize; ++c)
        {
            long target = trans[rep][c];
            if (target != -1 && stateToClass[target] != -1)
            {
                minTrans[i][c] = stateToClass[target];
            }
        }
    }

    return DFA(minTrans, minAccepting, newOrigState);
}