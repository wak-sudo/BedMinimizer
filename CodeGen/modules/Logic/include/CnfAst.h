#pragma once

#include <string>
#include <vector>

namespace Cnf
{
    class Literal
    {
        std::string name_;
        bool negated_;

    public:
        Literal(std::string name, bool negated);
        Literal(std::string name);

        const std::string &name() const;
        bool negated() const;
        std::string str() const;
    };

    Literal VAR(std::string name);
    Literal NOT(Literal l);

    bool operator==(const Literal &l1, const Literal &l2);
    bool operator<(const Literal &l1, const Literal &l2);

    class Clause
    {
        std::vector<Literal> literals_;

    public:
        Clause() = default;
        Clause(Literal literal);
        Clause(std::vector<Literal> literals);

        void addLiteral(Literal literal);
        std::vector<Literal> &literals();
        const std::vector<Literal> &literals() const;
        void clear();
        std::string str() const;
    };

    Clause OR(std::vector<Literal> literals);
    Clause OR(Literal l1, Literal l2);
    Clause OR(Clause c, Literal l);

    Clause &operator|=(Clause &target, Literal provided);
    Clause &operator|=(Clause &target, Clause provided);
    Clause operator|(Clause c1, Clause c2);

    bool operator==(const Clause &c1, const Clause &c2);
    bool operator<(const Clause &c1, const Clause &c2);

    class Formula
    {
        std::vector<Clause> clauses_;

    public:
        Formula() = default;
        Formula(std::vector<Clause> clauses);
        Formula(Clause clause);

        void addClause(Clause clause);
        std::vector<Clause> &clauses();
        const std::vector<Clause> &clauses() const;
        std::string str() const;
    };

    Formula AND(Clause c1, Clause c2);
    Formula AND(std::vector<Clause> clauses);

    Formula &operator&=(Formula &target, Formula provided);
    Formula &operator&=(Formula &f, Clause c);
    Formula &operator&=(Formula &target, Literal provided);
    Formula operator&(Formula target, Formula provided);

    bool operator==(const Formula &f1, const Formula &f2);
}