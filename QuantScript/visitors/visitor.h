#pragma once
#include "nodes/nodes.h"
#include <stack>
#include <map>

namespace QuantScript
{
    template <class T>
    using quickStack = std::stack<T>;
    class Visitor
    {
    protected:
        Visitor() {};
        void visitArguments(Node &node);

    public:
        virtual ~Visitor();
        virtual void visit(ExpressionTree &expTree);
        virtual void visitUplus(NodeUplus &node);
        virtual void visitUminus(NodeUminus &node);
        virtual void visitAdd(NodeAdd &node);
        virtual void visitSubtract(NodeSubtract &node);
        virtual void visitMult(NodeMult &node);
        virtual void visitDiv(NodeDiv &node);

        // Advanced
        virtual void visitPow(NodePow &node);
        virtual void visitLog(NodeLog &node);
        virtual void visitSqrt(NodeSqrt &node);
        virtual void visitMax(NodeMax &node);
        virtual void visitMin(NodeMin &node);

        // Logic
        virtual void visitAssign(NodeAssign &node);
        virtual void visitEqual(NodeEqual &node);
        virtual void visitDifferent(NodeDifferent &node);
        virtual void visitSuperior(NodeSuperior &node);
        virtual void visitSupEqual(NodeSupEqual &node);
        virtual void visitInferior(NodeInferior &node);
        virtual void visitInfEqual(NodeInfEqual &node);
        virtual void visitAnd(NodeAnd &node);
        virtual void visitOr(NodeOr &node);

        virtual void visitIf(NodeIf &node);
        virtual void visitSpot(NodeSpot &node);
        virtual void visitConst(NodeConst &node);
        virtual void visitVar(NodeVar &node);
        virtual void visitPays(NodePays &node);

        // Custom
        virtual void visitSolver(NodeSolver &node);
        virtual void visitDefinition(NodeDefinition &node);
    };

    class ConstVisitor
    {
    protected:
        ConstVisitor() {};
        void visitArguments(const Node &node);

    public:
        virtual ~ConstVisitor();
        virtual void visit(const ExpressionTree &expTree);
        virtual void visitUplus(const NodeUplus &node);
        virtual void visitUminus(const NodeUminus &node);
        virtual void visitAdd(const NodeAdd &node);
        virtual void visitSubtract(const NodeSubtract &node);
        virtual void visitMult(const NodeMult &node);
        virtual void visitDiv(const NodeDiv &node);

        // Advanced
        virtual void visitPow(const NodePow &node);
        virtual void visitLog(const NodeLog &node);
        virtual void visitSqrt(const NodeSqrt &node);
        virtual void visitMax(const NodeMax &node);
        virtual void visitMin(const NodeMin &node);

        // Logic
        virtual void visitAssign(const NodeAssign &node);
        virtual void visitEqual(const NodeEqual &node);
        virtual void visitDifferent(const NodeDifferent &node);
        virtual void visitSuperior(const NodeSuperior &node);
        virtual void visitSupEqual(const NodeSupEqual &node);
        virtual void visitInferior(const NodeInferior &node);
        virtual void visitInfEqual(const NodeInfEqual &node);
        virtual void visitAnd(const NodeAnd &node);
        virtual void visitOr(const NodeOr &node);

        virtual void visitIf(const NodeIf &node);
        virtual void visitSpot(const NodeSpot &node);
        virtual void visitConst(const NodeConst &node);
        virtual void visitVar(const NodeVar &node);
        virtual void visitPays(const NodePays &node);

        // Custom
        virtual void visitSolver(const NodeSolver &node);
        virtual void visitDefinition(const NodeDefinition &node);
    };
}