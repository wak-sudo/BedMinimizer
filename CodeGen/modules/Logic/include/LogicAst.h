#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <utility>

namespace LogicAst
{
    class Node;

    using NodePtr = std::shared_ptr<Node>;

    class Var
    {
        std::string name_;

    public:
        Var(std::string name);
        const std::string &name() const;
    };

    class Not
    {
        NodePtr child_;

    public:
        Not(NodePtr child);
        const NodePtr &child() const;
    };

    class Binary
    {
    public:
        enum class Type
        {
            AND,
            OR,
            IMPLIES,
            IFF
        };

        Binary(Type type, NodePtr left, NodePtr right);

        Type type() const;
        const NodePtr &left() const;
        const NodePtr &right() const;

    private:
        Binary::Type type_;
        NodePtr left_;
        NodePtr right_;
    };

    class Node
    {
    public:
        using Data = std::variant<
            Var,
            Not,
            Binary>;

    private:
        Data data_;

    public:
        Node(Data d);
        const Data &data() const;
    };

    NodePtr AND(NodePtr left, NodePtr right);
    NodePtr OR(NodePtr left, NodePtr right);
    NodePtr IMPLIES(NodePtr left, NodePtr right);
    NodePtr IFF(NodePtr left, NodePtr right);

    NodePtr VAR(std::string name);
    NodePtr NOT(NodePtr child);

    NodePtr &operator|=(NodePtr &target, NodePtr provided);
    NodePtr &operator&=(NodePtr &target, NodePtr provided);
    
    NodePtr GenFreshVar();

    const std::string HELPER_VAR_PREFIX = "$helper_";

    std::string str(const NodePtr &node);
}