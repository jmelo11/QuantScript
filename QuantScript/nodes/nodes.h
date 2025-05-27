#pragma once

#include <memory>
#include <vector>
#include <string>
#include <cmath>

namespace QuantScript
{
    struct Node;
    class Visitor;
    class ConstVisitor;

    using Date = int;
    using ExpressionTree = std::unique_ptr<Node>;
    using Statement = ExpressionTree;
    using Event = std::vector<Statement>;

    // base
    struct Node
    {
        std::vector<ExpressionTree> arguments;
        virtual ~Node() {}
        virtual void acceptVisitor(Visitor &visitor) = 0;
        virtual void acceptVisitor(ConstVisitor &visitor) const = 0;
    };
    // arithmetic
    struct NodeUplus : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeUminus : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeAdd : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeSubtract : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeMult : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeDiv : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };

    // advanced
    struct NodePow : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeLog : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeSqrt : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeMax : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeMin : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };

    // logic
    struct NodeAssign : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeEqual : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeDifferent : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeSuperior : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeSupEqual : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeInferior : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeInfEqual : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeAnd : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeOr : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };

    struct NodeIf : public Node
    {
        int firstElse;
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };

    // Constants and variables
    struct NodeSpot : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeConst : public Node
    {
        double value; // Constant, so wont need templating.
        NodeConst(const double v) : value(v) {};
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };
    struct NodeVar : public Node
    {
        const std::string name;
        unsigned index;
        NodeVar(std::string n) : name(n) {};
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };

    struct NodePays : public Node
    {
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };

    template <typename NodeType, typename... Args>
    std::unique_ptr<NodeType> make_node(Args &&...args)
    {
        return std::unique_ptr<NodeType>(new NodeType(std::forward<Args>(args)...));
    };
    template <typename NodeType, typename... Args>
    std::unique_ptr<Node> make_base_node(Args &&...args)
    {
        return std::unique_ptr<Node>(new NodeType(std::forward<Args>(args)...));
    };
    template <class NodeType>
    ExpressionTree buildBinary(ExpressionTree &lhs, ExpressionTree &rhs)
    {
        auto top = make_base_node<NodeType>();
        top->arguments.resize(2);
        //	Take ownership of lhs and rhs
        top->arguments[0] = std::move(lhs);
        top->arguments[1] = std::move(rhs);
        //	Return
        return top;
    }

    // custom nodes
    // better name?
    struct NodeSolver : public Node
    {
        double value;
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };

    struct NodeDefinition : public Node
    {
        const std::string name;
        unsigned index;
        NodeDefinition(std::string n) : name(n) {};
        void acceptVisitor(Visitor &visitor) override;
        void acceptVisitor(ConstVisitor &visitor) const override;
    };

}
