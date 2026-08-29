#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "CnfAst.h"

using namespace std;
using namespace Cnf;

Formula::Formula(vector<Clause> clauses) 
    : clauses_(move(clauses))
{
}

Formula::Formula(Clause clause)
{
    clauses_.push_back(move(clause));
}

void Formula::addClause(Clause clause)
{
    clauses_.push_back(move(clause));
}

vector<Clause> &Formula::clauses()
{
    return clauses_;
}

const vector<Clause> &Formula::clauses() const
{
    return clauses_;
}

Formula Cnf::AND(Clause c1, Clause c2)
{
    vector<Clause> clauses;
    clauses.reserve(2);
    clauses.push_back(move(c1));
    clauses.push_back(move(c2));

    return Formula(move(clauses));
}

Formula Cnf::AND(vector<Clause> clauses)
{
    return Formula(move(clauses));
}

Formula &Cnf::operator&=(Formula &target, Formula provided)
{
    target.clauses().insert(
        target.clauses().end(),
        make_move_iterator(provided.clauses().begin()),
        make_move_iterator(provided.clauses().end())
    );

    return target;
}

Formula &Cnf::operator&=(Formula &f, Clause c)
{
    f.addClause(move(c));
    return f;
}

Formula &Cnf::operator&=(Formula &target, Literal provided)
{
    target.addClause(Clause(move(provided)));
    return target;
}

Formula Cnf::operator&(Formula target, Formula provided)
{
    target &= move(provided);
    return target;
}

bool Cnf::operator==(const Formula &f1, const Formula &f2)
{
    const auto &f1Arr = f1.clauses();
    const auto &f2Arr = f2.clauses();
    if (f1Arr.size() != f2Arr.size())
        return false;

    vector<Clause> s1 = f1Arr;
    vector<Clause> s2 = f2Arr;

    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());

    return s1 == s2;
}

string Cnf::Formula::str() const
{
    if (clauses_.empty())
        return "[]";

    string s = "[";
    bool first = true;

    for (const auto &clause : clauses_)
    {
        if (!first)
            s += " & ";

        s += clause.str();
        first = false;
    }

    s += "]";
    return s;
}