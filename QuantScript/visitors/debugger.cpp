#include "debugger.h"

namespace QuantScript
{
    Debugger::~Debugger() {};
    void Debugger::visit(Node &node) {};
    void Debugger::debug(Node &node, std::string nodeId)
    {
        for (auto it = node.arguments.rbegin(); it != node.arguments.rend(); it++)
        {
            (*it)->acceptVisitor(*this);
        };
        std::string str(myEvaluation + nodeId);
        if (!node.arguments.empty())
        {
            str += "(";
            // First argument, pushed last
            str += myStack.top();
            myStack.pop();
            if (node.arguments.size() > 1)
            {
                str += myEvaluation + ",";
            };
            // Args 2 to n-1
            for (size_t i = 1; i < node.arguments.size() - 1; ++i)
            {
                str += myStack.top() + myEvaluation + ",";
                myStack.pop();
            }
            if (node.arguments.size() > 1)
            {
                // Last argument, pushed first
                str += myStack.top();
                myStack.pop();
            }
            // Close �)�
            str += myEvaluation + ")";
        };
        myStack.push(std::move(str));
    };
    std::string Debugger::getString() const { return myStack.top(); };

    void Debugger::visitUplus(NodeUplus &node) { debug(node, "UPLUS"); };
    void Debugger::visitUminus(NodeUminus &node) { debug(node, "UMINUS"); };
    void Debugger::visitAdd(NodeAdd &node) { debug(node, "ADD"); };
    void Debugger::visitSubtract(NodeSubtract &node) { debug(node, "SUBTRACT"); };
    void Debugger::visitMult(NodeMult &node) { debug(node, "MULT"); };
    void Debugger::visitDiv(NodeDiv &node) { debug(node, "DIV"); };

    // Advanced
    void Debugger::visitPow(NodePow &node) { debug(node, "POW"); };

    void Debugger::visitLog(NodeLog &node) { debug(node, "LOG"); };
    void Debugger::visitSqrt(NodeSqrt &node) { debug(node, "SQRT"); };
    void Debugger::visitMax(NodeMax &node) { debug(node, "MAX"); };
    void Debugger::visitMin(NodeMin &node) { debug(node, "MIN"); };

    // Logic
    void Debugger::visitIf(NodeIf &node) { debug(node, std::string("IF[") + std::to_string(node.firstElse) + "]"); };
    void Debugger::visitAssign(NodeAssign &node) { debug(node, "ASSIGN"); };
    void Debugger::visitEqual(NodeEqual &node) { debug(node, "EQUAL"); };
    void Debugger::visitDifferent(NodeDifferent &node) { debug(node, "DIFFERENT"); };

    void Debugger::visitSuperior(NodeSuperior &node) { debug(node, "SUP"); };
    void Debugger::visitSupEqual(NodeSupEqual &node) { debug(node, "SUPEQUAL"); };
    void Debugger::visitInferior(NodeInferior &node) { debug(node, "INF"); };
    void Debugger::visitInfEqual(NodeInfEqual &node) { debug(node, "INFEQUAL"); };
    void Debugger::visitAnd(NodeAnd &node) { debug(node, "AND"); };
    void Debugger::visitOr(NodeOr &node) { debug(node, "OR"); };

    void Debugger::visitSpot(NodeSpot &node)
    {
        debug(node, "SPOT");
    };
    void Debugger::visitVar(NodeVar &node)
    {
        debug(node, "VAR[" + node.name + "," + std::to_string(node.index) + "]");
    };
    void Debugger::visitConst(NodeConst &node)
    {
        debug(node, "CONST[" + std::to_string(node.value) + "]");
    };

    // Custom
    void Debugger::visitSolver(NodeSolver &node)
    {
        debug(node, "SOLVER");
    };
    void Debugger::visitDefinition(NodeDefinition &node)
    {
        debug(node, "DEF[" + node.name + "," + std::to_string(node.index) + "]");
    };
}