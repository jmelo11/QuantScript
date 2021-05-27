#pragma once
#include "visitors/visitor.h"
#include "visitors/evaluator.h"


namespace QuantScript {
	// Aux function
	template<class S>
	int getIndex(std::vector<S> v, S K) {
		auto it = std::find(v.begin(), v.end(), K);

		// If element was found
		if (it != v.end())
		{

			// calculating the index
			// of K
			int index = it - v.begin();
			return index;
		}
		else {
			// If the element is not
			// present in the vector
			return -1;
		}
	};

	template <class T>
	class SolverEvaluator : public Visitor {
		std::string targetVariable;
		T targetValue;
		T* solverValue;
		int targetIndex;

	public:		
		SolverEvaluator(std::string target, T value): targetVariable(target), targetValue(value) {};
		~SolverEvaluator() {};

		void init(std::vector<std::string> varNames) {
			targetIndex = getIndex(varNames, targetVariable);
		};
		void update(T value) {
			*solverValue = value;
		};
		void visitSolver(NodeSolver& node) {
			solverValue = &node.value;
		};

		//setters, getters
		void setTarget(std::string target, T value) {
			targetVariable = target;
			targetValue = value;
		};
		std::string getTargetVariable() {
			return targetVariable;
		};
		T getTargetValue() {
			return targetValue;
		};
		int getTargetIndex() {
			return targetIndex;
		};
	
	};

	
}