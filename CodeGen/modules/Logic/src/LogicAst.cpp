#include "LogicAst.h"

#include <memory>
#include <string>
#include <variant>
#include <utility>
#include <stdexcept>

using namespace LogicAst;
using namespace std;

Node::Node(Data d)
    : data_(std::move(d))
{
}

Var::Var(std::string name)
    : name_(std::move(name))
{
}

Binary::Binary(Type type, NodePtr left, NodePtr right)
    : type_(type),
      left_(std::move(left)),
      right_(std::move(right))
{
}

Not::Not(NodePtr child)
    : child_(std::move(child))
{
}

const Node::Data &Node::data() const
{
    return data_;
}

const std::string &Var::name() const
{
    return name_;
}

const NodePtr &Not::child() const
{
    return child_;
}

Binary::Type Binary::type() const
{
    return type_;
}

const NodePtr &Binary::left() const
{
    return left_;
}

const NodePtr &Binary::right() const
{
    return right_;
}

NodePtr LogicAst::AND(NodePtr left, NodePtr right)
{
    return std::make_shared<Node>(Binary(Binary::Type::AND, std::move(left), std::move(right)));
}

NodePtr LogicAst::OR(NodePtr left, NodePtr right)
{
    return std::make_shared<Node>(Binary(Binary::Type::OR, std::move(left), std::move(right)));
}

NodePtr LogicAst::IMPLIES(NodePtr left, NodePtr right)
{
    return std::make_shared<Node>(Binary(Binary::Type::IMPLIES, std::move(left), std::move(right)));
}

NodePtr LogicAst::IFF(NodePtr left, NodePtr right)
{
    return std::make_shared<Node>(Binary(Binary::Type::IFF, std::move(left), std::move(right)));
}

NodePtr LogicAst::NOT(NodePtr child)
{
    return std::make_shared<Node>(Not(std::move(child)));
}

NodePtr &LogicAst::operator&=(NodePtr &lhs, NodePtr rhs)
{
    if (lhs == nullptr)
        lhs = std::move(rhs);
    else
        lhs = LogicAst::AND(std::move(lhs), std::move(rhs));
    return lhs;
}

NodePtr &LogicAst::operator|=(NodePtr &lhs, NodePtr rhs)
{
    if (lhs == nullptr)
        lhs = std::move(rhs);
    else
        lhs = LogicAst::OR(std::move(lhs), std::move(rhs));
    return lhs;
}

NodePtr LogicAst::VAR(std::string name)
{
    return std::make_shared<Node>(Var(std::move(name)));
}

NodePtr LogicAst::GenFreshVar()
{
    static long counter = 0;

    std::string s = HELPER_VAR_PREFIX + std::to_string(counter);

    counter++;

    return std::make_shared<Node>(Var(std::move(s)));
}

std::string LogicAst::str(const NodePtr &root)
{
    if (!root)
        return "null";

    return std::visit(
        [&](const auto &x) -> std::string
        {
            using T = decay_t<decltype(x)>;

            if constexpr (is_same_v<T, LogicAst::Var>)
            {
                return x.name();
            }
            else if constexpr (is_same_v<T, LogicAst::Not>)
            {
                return "-" + str(x.child());
            }
            else if constexpr (is_same_v<T, LogicAst::Binary>)
            {
                const auto leftStr = str(x.left());
                const auto rightStr = str(x.right());
                switch (x.type())
                {
                case LogicAst::Binary::Type::AND:
                    return "(" + leftStr + " & " + rightStr + ")";
                case LogicAst::Binary::Type::OR:
                    return "(" + leftStr + " | " + rightStr + ")";
                case LogicAst::Binary::Type::IMPLIES:
                    return "(" + leftStr + " => " + rightStr + ")";
                case LogicAst::Binary::Type::IFF:
                    return "(" + leftStr + " <=> " + rightStr + ")";
                }
            }
            throw invalid_argument("Unimplemented branch in LogicAst str!");
        },
        root->data());
}