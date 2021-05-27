#include "visitor.h"

namespace QuantScript {
    Visitor::~Visitor() {};
    void Visitor::visit(ExpressionTree& expTree) {
        expTree->acceptVisitor(*this);
    };
    void Visitor::visitArguments(Node& node) {
        //visit all arguments, left to right
        for (auto& e : node.arguments) {
            e->acceptVisitor(*this);
        };
    };
    void Visitor::visitUplus(NodeUplus& node) { visitArguments(node); };
    void Visitor::visitUminus(NodeUminus& node) { visitArguments(node); };
    void Visitor::visitAdd(NodeAdd& node) { visitArguments(node); };
    void Visitor::visitSubtract(NodeSubtract& node) { visitArguments(node); };
    void Visitor::visitMult(NodeMult& node) { visitArguments(node); };
    void Visitor::visitDiv(NodeDiv& node) { visitArguments(node); };

    //Advanced
    void Visitor::visitPow(NodePow& node) { visitArguments(node); };
    void Visitor::visitLog(NodeLog& node) { visitArguments(node); };
    void Visitor::visitSqrt(NodeSqrt& node) { visitArguments(node); };
    void Visitor::visitMax(NodeMax& node) { visitArguments(node); };
    void Visitor::visitMin(NodeMin& node) { visitArguments(node); };

    //Logic
    void Visitor::visitAssign(NodeAssign& node) { visitArguments(node); };
    void Visitor::visitEqual(NodeEqual& node) { visitArguments(node); };
    void Visitor::visitDifferent(NodeDifferent& node) { visitArguments(node); };
    void Visitor::visitSuperior(NodeSuperior& node) { visitArguments(node); };
    void Visitor::visitSupEqual(NodeSupEqual& node) { visitArguments(node); };
    void Visitor::visitInferior(NodeInferior& node) { visitArguments(node); };
    void Visitor::visitInfEqual(NodeInfEqual& node) { visitArguments(node); };
    void Visitor::visitAnd(NodeAnd& node) { visitArguments(node); };
    void Visitor::visitOr(NodeOr& node) { visitArguments(node); };

    void Visitor::visitIf(NodeIf& node) { visitArguments(node); };
    void Visitor::visitSpot(NodeSpot& node) { visitArguments(node); };
    void Visitor::visitConst(NodeConst& node) { visitArguments(node); };
    void Visitor::visitVar(NodeVar& node) { visitArguments(node); };
    void Visitor::visitPays(NodePays& node) { visitArguments(node); };
    //custom
    void Visitor::visitSolver(NodeSolver& node) { visitArguments(node); };
    void Visitor::visitDefinition(NodeDefinition& node) { visitArguments(node); };


    ConstVisitor::~ConstVisitor() {};
    void ConstVisitor::visit(const ExpressionTree& expTree) {
        expTree->acceptVisitor(*this);
    };
    void ConstVisitor::visitArguments(const Node& node) {
        //visit all arguments, left to right
        for (auto& e : node.arguments) {
            e->acceptVisitor(*this);
        };
    };
    void ConstVisitor::visitUplus(const NodeUplus& node) { visitArguments(node); };
    void ConstVisitor::visitUminus(const NodeUminus& node) { visitArguments(node); };
    void ConstVisitor::visitAdd(const NodeAdd& node) { visitArguments(node); };
    void ConstVisitor::visitSubtract(const NodeSubtract& node) { visitArguments(node); };
    void ConstVisitor::visitMult(const NodeMult& node) { visitArguments(node); };
    void ConstVisitor::visitDiv(const NodeDiv& node) { visitArguments(node); };

    //Advanced
    void ConstVisitor::visitPow(const NodePow& node) { visitArguments(node); };
    void ConstVisitor::visitLog(const NodeLog& node) { visitArguments(node); };
    void ConstVisitor::visitSqrt(const NodeSqrt& node) { visitArguments(node); };
    void ConstVisitor::visitMax(const NodeMax& node) { visitArguments(node); };
    void ConstVisitor::visitMin(const NodeMin& node) { visitArguments(node); };

    //Logic
    void ConstVisitor::visitAssign(const NodeAssign& node) { visitArguments(node); };
    void ConstVisitor::visitEqual(const NodeEqual& node) { visitArguments(node); };
    void ConstVisitor::visitDifferent(const NodeDifferent& node) { visitArguments(node); };
    void ConstVisitor::visitSuperior(const NodeSuperior& node) { visitArguments(node); };
    void ConstVisitor::visitSupEqual(const NodeSupEqual& node) { visitArguments(node); };
    void ConstVisitor::visitInferior(const NodeInferior& node) { visitArguments(node); };
    void ConstVisitor::visitInfEqual(const NodeInfEqual& node) { visitArguments(node); };
    void ConstVisitor::visitAnd(const NodeAnd& node) { visitArguments(node); };
    void ConstVisitor::visitOr(const NodeOr& node) { visitArguments(node); };

    void ConstVisitor::visitIf(const NodeIf& node) { visitArguments(node); };
    void ConstVisitor::visitSpot(const NodeSpot& node) { visitArguments(node); };
    void ConstVisitor::visitConst(const NodeConst& node) { visitArguments(node); };
    void ConstVisitor::visitVar(const NodeVar& node) { visitArguments(node); };

    void ConstVisitor::visitPays(const NodePays& node) { visitArguments(node); };

    //custom
    void ConstVisitor::visitSolver(const NodeSolver& node) { visitArguments(node); };
    void ConstVisitor::visitDefinition(const NodeDefinition& node) { visitArguments(node); };
   }; 