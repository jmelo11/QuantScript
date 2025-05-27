#pragma once
#include "visitor.h"
#include "models/models.h"
#include <cmath>

#define EPS 1.0e-15
namespace QuantScript {

	template<class T>
	class Evaluator : public ConstVisitor {
		std::vector<T> myVariables;
        std::vector<T> myDefinitions;
		quickStack<bool> myBStack;
		quickStack<T> myDStack; 
        bool myLhsVar = false;
        T* myLhsVarAddr;

        const Scenario<T>* myScenario;
        size_t myCurrentEvent;

	public:
        
		~Evaluator() {};
        Evaluator(size_t nVar) : myVariables(nVar) {};
        Evaluator(const Evaluator& rhs) : myVariables(rhs.myVariables) {}
        Evaluator& operator=(const Evaluator& rhs)
        {
            if (this == &rhs) return *this;
            myVariables = rhs.myVariables;
            return *this;
        }
        Evaluator(Evaluator&& rhs) : myVariables(move(rhs.myVariables)) {}
        Evaluator& operator=(Evaluator&& rhs)
        {
            myVariables = move(rhs.myVariables);
            return *this;
        }
        // (Re-)initialize before evaluation in each scenario
        void init()
        {
            for (auto& varIt : myVariables) varIt = 0.0;
            // Stacks should be empty, if this is not the case we empty them
            // without affecting capacity for added performance
            while (!myDStack.empty()) myDStack.pop();
            while (!myBStack.empty()) myBStack.pop();
            myLhsVar = false;
            myLhsVarAddr = nullptr;
        }

        std::vector<T> varVals() {
            return myVariables;
        };
      
        void reverseVisitArguments(const Node& node) {
            //visit all arguments, right to left
            for (auto it = node.arguments.rbegin(); it != node.arguments.rend(); ++it)
                (*it)->acceptVisitor(*this);
        };

        void setScenario(const Scenario<T>* scenario) {
            myScenario = scenario;
        };
        void setCurrentEvent(size_t currentEvent) {
            myCurrentEvent = currentEvent;
        };

        //Aux
        std::pair < T, T> pop2() {
            std::pair<T, T> res;
            res.first = myDStack.top();
            myDStack.pop();
            res.second = myDStack.top();
            myDStack.pop();
            return res;
        };
        std::pair<bool, bool> pop2b() {
            std::pair<bool, bool> res;
            res.first = myBStack.top();
            myBStack.pop();
            res.second = myBStack.top();
            myBStack.pop();
            return res;
        };

        //Nodes
        void visitUplus(const NodeUplus& node) {
            reverseVisitArguments(node);
        };
        void visitUminus(const NodeUminus& node) {
            reverseVisitArguments(node);
            myDStack.top() *= -1;
        };
        void visitAdd(const NodeAdd& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myDStack.push(res.first + res.second);
        };
        void visitSubtract(const NodeSubtract& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myDStack.push(res.first - res.second);
        };
        void visitMult(const NodeMult& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myDStack.push(res.first * res.second);
        };
        void visitDiv(const NodeDiv& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myDStack.push(res.first / res.second);
        };

        //Advanced
        void visitPow(const NodePow& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myDStack.push(pow(res.first, res.second));
        };
        void visitLog(const NodeLog& node) {
            reverseVisitArguments(node);
            auto res = log(myDStack.top());
            myDStack.pop();
            myDStack.push(res);
        };
        void visitSqrt(const NodeSqrt& node) {
            reverseVisitArguments(node);
            auto res = sqrt(myDStack.top());
            myDStack.pop();
            myDStack.push(res);
        };
        void visitMax(const NodeMax& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myDStack.push(std::max(res.first, res.second));
        };
        void visitMin(const NodeMin& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myDStack.push(std::min(res.first, res.second));
        };

        //Logic
        void visitAssign(const NodeAssign& node) {

            myLhsVar = true;
            node.arguments[0]->acceptVisitor(*this); //left

            myLhsVar = false;
            node.arguments[1]->acceptVisitor(*this); //right
            *myLhsVarAddr = myDStack.top();
            myDStack.pop();
        };
        void visitEqual(const NodeEqual& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myBStack.push(fabs(res.first - res.second) < EPS);
        };
        void visitDifferent(const NodeDifferent& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myBStack.push(fabs(res.first - res.second) > EPS);
        };
        void visitSuperior(const NodeSuperior& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myBStack.push(res.first > res.second + EPS);
        };
        void visitSupEqual(const NodeSupEqual& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myBStack.push(res.first > res.second - EPS);
        };
        void visitInferior(const NodeInferior& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myBStack.push(res.first < res.second - EPS);
        };
        void visitInfEqual(const NodeInfEqual& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myBStack.push(res.first < res.second + EPS);
        };
        void visitAnd(const NodeAnd& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myBStack.push(res.first && res.second);
        };
        void visitOr(const NodeOr& node) {
            reverseVisitArguments(node);
            auto res = pop2();
            myBStack.push(res.first || res.second);
        };

        void visitIf(const NodeIf& node) {
            // Visit the condition
            node.arguments[0]->acceptVisitor(*this);
            // Pick the result
            const bool isTrue = myBStack.top();
            myBStack.pop();
            // Evaluate the relevant statements
            if (isTrue)
            {
                const auto lastTrue =
                    node.firstElse == -1 ?
                    node.arguments.size() - 1 :
                    node.firstElse - 1;
                for (auto i = 1; i <= lastTrue; ++i)
                {
                    node.arguments[i]->acceptVisitor(*this);
                };
            }
            else if (node.firstElse != -1)
            {
                for (auto i = node.firstElse; i <= node.arguments.size() - 1; ++i)
                {
                    node.arguments[i]->acceptVisitor(*this);
                }
            };
        };
        void visitSpot(const NodeSpot& node) {
            myDStack.push((*myScenario)[myCurrentEvent].spot);
        };
        void visitConst(const NodeConst& node) {
            myDStack.push(node.value);
        };
        void visitVar(const NodeVar& node) {
            if (myLhsVar) {
                // Record address in myLhsVarAdr
                myLhsVarAddr = &myVariables[node.index];
            }
            else // Read
            {
                // Push value onto the stack
                myDStack.push(myVariables[node.index]);
            };
        };        
        void visitPays(const NodePays& node) {
            myLhsVar = true;
            node.arguments[0]->acceptVisitor(*this);
            myLhsVar = false;
            // Visit the RHS expression
            node.arguments[1]->acceptVisitor(*this);
            // Write result into variable
            *myLhsVarAddr += myDStack.top() / (*myScenario)[myCurrentEvent].numeraire;
            myDStack.pop();
        }

        //Custom
        void visitSolver(const NodeSolver& node) {
            myDStack.push(node.value);
        };
        void visitDefinition(const NodeDefinition& node) {
            myDStack.push(myDefinitions[node.index]);
        };
	};
}