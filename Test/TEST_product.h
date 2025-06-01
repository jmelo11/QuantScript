#include <map>
#include <iostream>
#include "parser/parser.h"
#include "visitors/debugger.h"
#include "product/product.h"


namespace QuantScript {
	void test_product() {
		std::string s = "y = max(log(1+6-6),1) "
			"x = 1				"
			"if y<2 {			"
			" x = 2				"
			" }				";

		std::map<Date, std::string> mapping = { {1,s} };
		Product prod;
		prod.parseEvents(mapping.begin(), mapping.end());
		prod.indexVariables();
		Debugger d;
		prod.visit(d);
		std::cout << d.getString() << std::endl;
		std::unique_ptr<Scenario<double>> scenario = prod.buildScenario<double>();
		std::unique_ptr<Evaluator<double>> evaluator = prod.buildEvaluator<double>();
		prod.evaluate(*scenario, *evaluator);
		std::vector values = evaluator->varVals();
		std::vector var_names = prod.varNames();
		for (size_t i = 0; i < values.size(); i++) {
			std::cout << var_names[i] << ": " << values[i] << std::endl;
		};
	};
}