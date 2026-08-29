#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "CnfAst.h"

using namespace std;
using namespace Cnf;

Clause::Clause(Cnf::Literal literal)
{
    literals_.push_back(std::move(literal));
}

Clause::Clause(std::vector<Literal> literals) 
    : literals_(std::move(literals))
{
}

void Clause::addLiteral(Literal literal)
{
    literals_.push_back(std::move(literal));
}

vector<Literal> &Clause::literals()
{
    return literals_;
}

const vector<Literal> &Clause::literals() const
{
    return literals_;
}

void Clause::clear()
{
    literals_.clear();
}

Clause Cnf::OR(std::vector<Literal> literals)
{
    return Clause(std::move(literals));
}

Clause Cnf::OR(Literal l1, Literal l2)
{
    std::vector<Literal> literals;
    literals.reserve(2);
    literals.push_back(std::move(l1));
    literals.push_back(std::move(l2));

    return Clause(std::move(literals));
}

Clause &Cnf::operator|=(Clause &target, Literal provided)
{
    target.addLiteral(std::move(provided));
    return target;
}

Clause &Cnf::operator|=(Clause &target, Clause provided)
{
    target.literals().insert(
        target.literals().end(),
        std::make_move_iterator(provided.literals().begin()),
        std::make_move_iterator(provided.literals().end())
    );
    return target;
}

bool Cnf::operator==(const Clause &c1, const Clause &c2)
{
    const auto &c1Arr = c1.literals();
    const auto &c2Arr = c2.literals();

    if (c1Arr.size() != c2Arr.size())
        return false;

    std::vector<Literal> s1 = c1Arr;
    std::vector<Literal> s2 = c2Arr;

    std::sort(s1.begin(), s1.end());
    std::sort(s2.begin(), s2.end());

    return s1 == s2;
}

string Cnf::Clause::str() const
{
    if (literals_.empty())
        return "()";

    string s = "(";
    bool first = true;

    for (const auto &literal : literals_)
    {
        if (!first)
            s += " | ";

        s += literal.str();
        first = false;
    }

    s += ")";

    return s;
}

bool Cnf::operator<(const Clause &c1, const Clause &c2)
{
    auto s1 = c1.literals();
    auto s2 = c2.literals();

    std::sort(s1.begin(), s1.end());
    std::sort(s2.begin(), s2.end());

    return s1 < s2;
}

Clause Cnf::operator|(Clause c1, Clause c2)
{
    c1 |= std::move(c2);
    return c1;
}

Clause Cnf::OR(Clause c, Literal l)
{
    c.addLiteral(std::move(l));
    return c;
}