#pragma once
#include "visitor.h"
namespace QuantScript {
    class Debugger : public Visitor {
    private:
        std::string myEvaluation;
        quickStack<std::string> myStack;
        Event::reverse_iterator it;
    public:
        ~Debugger();
        void debug(Node& node, std::string name);

        std::string getString() const;

        void visit(Node& node);
        void visitUplus(NodeUplus& node) override;
        void visitUminus(NodeUminus& node) override;
        void visitAdd(NodeAdd& node) override;
        void visitSubtract(NodeSubtract& node) override;
        void visitMult(NodeMult& node) override;
        void visitDiv(NodeDiv& node) override;

        //Advanced
        void visitPow(NodePow& node) override;
        void visitLog(NodeLog& node) override;
        void visitSqrt(NodeSqrt& node) override;
        void visitMax(NodeMax& node) override;
        void visitMin(NodeMin& node) override;

        //Logic
        void visitIf(NodeIf& node) override;
        void visitAssign(NodeAssign& node) override;
        void visitEqual(NodeEqual& node) override;
        void visitDifferent(NodeDifferent& node) override;
        void visitSuperior(NodeSuperior& node) override;
        void visitSupEqual(NodeSupEqual& node) override;
        void visitInferior(NodeInferior& node) override;
        void visitInfEqual(NodeInfEqual& node) override;
        void visitAnd(NodeAnd& node) override;
        void visitOr(NodeOr& node) override;

        void visitSpot(NodeSpot& node) override;
        void visitVar(NodeVar& node) override;
        void visitConst(NodeConst& node) override;

        //Custom
        void visitSolver(NodeSolver& node) override;
        void visitDefinition(NodeDefinition& node) override;
    };
}