#include <string>
#include <utility>

#include "CnfAst.h"

using namespace std;
using namespace Cnf;

Literal::Literal(string name, bool negated) 
    : name_(move(name)), negated_(negated)
{
}

Literal::Literal(string name) 
    : name_(move(name)), negated_(false)
{
}

const string &Literal::name() const
{
    return name_;
}

bool Literal::negated() const
{
    return negated_;
}

Literal Cnf::VAR(string name)
{
    return Literal(move(name));
}

Literal Cnf::NOT(Literal l)
{
    return Literal(l.name(), !l.negated());
}

bool Cnf::operator==(const Literal &l1, const Literal &l2)
{
    return l1.negated() == l2.negated() && l1.name() == l2.name();
}

string Cnf::Literal::str() const
{
    if (negated_)
        return "-" + name_;
    else
        return name_;
}

bool Cnf::operator<(const Literal &l1, const Literal &l2)
{
    if (l1.name() != l2.name())
        return l1.name() < l2.name();

    return !l1.negated() && l2.negated();
}