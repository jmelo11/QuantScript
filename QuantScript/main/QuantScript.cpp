

#include "visitors/debugger.h"
#include "product/product.h"
#include "parser/parser.h"
#include "models/models.h"
#include "aad/aad.h"
#include <vector>
#include <map>
#include <iostream>
#include "others/gaussians.h"

using namespace QuantScript;
using namespace std;

template<class T>
void test_product() {
	string s = "X = 3";
	map<Date, string> mapping = { {0,s} };
	Product prod;
	prod.parseEvents(mapping.begin(), mapping.end());
	prod.indexVariables();
	Debugger d;
	prod.visit(d);
	cout << d.getString() << endl;
	unique_ptr<Scenario<T>> scenario = prod.buildScenario<T>();
	unique_ptr<Evaluator<T>> evaluator = prod.buildEvaluator<T>();
	prod.evaluate(*scenario, *evaluator);
	vector<T> values = evaluator->varVals();
	vector<string> var_names = prod.varNames();
	for (size_t i = 0; i < values.size(); i++) {
		cout << var_names[i] << ": " << values[i]<< endl;
	};
};
void test_solver() {
	//Script
	string s =  "x = SOLVE()			"
				"y = 100				"
				"VP =  (x - y)			";

	//Create product
	map<Date, string> mapping = { {1,s} };
	Product prod;
	prod.parseEvents(mapping.begin(), mapping.end());
	prod.indexVariables();
	
	//evaluators
	unique_ptr<Scenario<double>> scenario = prod.buildScenario<double>();
	unique_ptr<Evaluator<double>> evaluator = prod.buildEvaluator<double>();
	unique_ptr<SolverEvaluator<double>> solver = prod.buildSolver<double>("VP", 0);
	
	prod.solve(*scenario, *evaluator, *solver); //->eval+solve

	//solve for variables

	//display results
        vector<double> values = evaluator->varVals();
        vector<string> var_names = prod.varNames();
	for (size_t i = 0; i < values.size(); i++) {
		cout << var_names[i] << ": " << values[i] << endl;
	};
	
};
void test_macro_replacer() {
	std::map < std::string, std::string> macro = { {"STRIKE","100" }, {"CALL(S,K)","MAX(S-K,0)"} };
	string script = "P = CALL(SOLVE(1,2,STRIKE),5)";
	script = macroReplacer(macro, script);
	cout << script << endl;	
}
template <class T>
void simpleBsScriptVal(const Date& today,T spot, T vol, T rate, const std::map<Date, std::string>& events,
					   const unsigned numSim, vector<string>& varNames, vector<T>& varVals) {
	if (events.begin()->first < today)
		throw std::runtime_error("Events in the past are disallowed");
	// Initialize product
	Product prd;
	prd.parseEvents(events.begin(), events.end());
	prd.indexVariables();


	// Build evaluator and scenarios
	unique_ptr<Scenario<T>> scen = prd.buildScenario<T>();
	unique_ptr<Evaluator<T>> eval = prd.buildEvaluator<T>();
	// Initialize model
	BasicRanGen random;
	SimpleBlackScholes<T> model(today, spot, vol, rate);
	ScriptSimulator<T> simulator(model, random);
	simulator.initForScripting(prd.eventDates());
	// Initialize results
	varNames = prd.varNames();
	varVals.resize(varNames.size(), 0.0);
	// Loop over simulations
	
	for (size_t i = 0; i < numSim; ++i)
	{
		//must be initialized in order to init AADNumbers of output varibles
		eval->init();
		// Generate next scenario into scen
		simulator.nextScenario(*scen);
		// Evaluate product
		prd.evaluate(*scen, *eval);
		// Update results

		for (size_t v = 0; v < varVals.size(); ++v)
		{
			varVals[v] += (eval->varVals()[v]) / numSim;
		}
		
	}
}

template <class T>
int evaluation_test()
{
	// Inputs
	map <string, string> macros = { {"STRIKE", "650"} };
	map<Date, string> events = { {360,"opt pays max(SPOT()-650,0)" } };
	
	Date today = 0;
	T spot = 700, vol = 0.3, rate = 0.06;
	unsigned numSim = 1;
	
	// Main call
	vector<string> varNames;
	vector<T> varVals;
	try
	{
		simpleBsScriptVal<T>(today, spot, vol,rate,
						  events, numSim, varNames, varVals);
	}
	catch (const runtime_error& rte)
	{
		cout << rte.what();
		return 1;
	}
	for (size_t i = 0; i < varNames.size(); ++i)
	{
		cout << varNames[i] << " = " << varVals[i] << endl;
		

	}
	
	vector<double> sens = calculateAdjoints(varVals[0]);
	cout << "spot sens (delta): " << sens[spot.idx] << endl;
	cout << "rate sens (rho): " << sens[rate.idx] << endl;
	cout << "vol sens (vega): " << sens[vol.idx] << endl;

	
	return 0;
}
void test_aad_numbers(const double a, const double b) {
	std::cout.precision(5);
	AADNumber x = a;
	AADNumber y = b;
	bool sumBool = x + y == a+b;
	bool subBool = x - y == a-b;
	bool multBool = x * y == a*b;
	bool divBool = x / y == a/b;

	bool unarySumBool = ++x == a+1;
	bool unarySubBool = --x == a;

	x += 1;
	bool eqSumBool = x == a+1;
	x -= 1;
	bool eqSubBool = x == a;
	x /= 2;
	bool eqDivBool = x == a/2;
	x *= 2;
	bool eqMultBool = x == a;
	
	cout << "SUM: X+Y == " << a + b << "? " << boolalpha << sumBool << endl;
	cout << "SUB: X-Y == " << a - b << "? " << boolalpha << subBool << endl;
	cout << "MULT: X*Y == "<< a * b << "? "<< boolalpha << multBool << endl;
	cout << "DIV: X/Y == " << a / b << "? "<< boolalpha << divBool << endl;
	cout << "UNARYSUM: ++X == " << a + 1 << " ? " << boolalpha << unarySumBool << endl;
	cout << "UNARYSUB: --X == " << a << " ? " << boolalpha << unarySumBool << endl;

	cout << "EQSUM: X+=1 == " << a + 1 << " ? " << boolalpha << eqSumBool << endl;
	cout << "EQSUB: X-=1 == " << a << " ? " << boolalpha << eqSubBool << endl;
	cout << "EQMULT: X*=2 == " << a << " ? " << boolalpha << eqMultBool << endl;
	cout << "EQDIV: X/=2 == " << a / 2 << " ? " << boolalpha << eqDivBool << endl;

	//functions
	x = sqrt(x);
	bool sqrtBool = x == std::sqrt(a);
	x = pow(x, 2);
	bool powBool = fabs(x-a) <= EPS;
	y = exp(y);
	bool expBool = fabs(y-std::exp(b)) <= EPS;
	y = log(y);
	bool logBool = fabs(y - b) <= EPS;
	
	cout << "SQRT: SQRT(X) == " << sqrt(a) << " ? " << boolalpha << sqrtBool << endl;
	cout << "POW: X^2 == " << a << " ? " << boolalpha << powBool << endl;
	cout << "EXP: EXP(X) == " << exp(a) << " ? " << boolalpha << expBool << endl;
	cout << "LOG: LOG(X) == " << a << " ? " << boolalpha << logBool << endl;

}


template <class T>
inline T blackScholes(const T spot, const T rate, const T yield, const T vol, const T strike, const T mat){
	auto df = exp(-rate * mat), fwd = spot * exp((rate - yield) * mat), std = vol * sqrt(mat);
	auto d = log(fwd / strike) / std;
	auto d1 = d + 0.5 * std, d2 = d - 0.5 * std;
	auto p1 = normalCdf(d1), p2 = normalCdf(d2);
	return df * (fwd * p1 - strike * p2);
}
inline void differentiateBlackScholes(){
	// initializes and records inputs
	AADNumber spot = 700, rate = 0.06, yield = 0.0, vol = 0.3, strike = 650, mat = 1;
	// evaluates and records operations
	auto result = blackScholes(spot, rate, yield, vol, strike, mat);
	cout << "Value = " << result.value << endl;   //  5.03705

	//  propagate adjoints
	vector<double> adjoints = calculateAdjoints(result);

	//  show derivatives
	cout << "Derivative to spot (delta) = "
		<< adjoints[spot.idx] << endl;
	//  0.309
	cout << "Derivative to rate (rho) = "
		<< adjoints[rate.idx] << endl;
	//  51.772
	cout << "Derivative to dividend yield = "
		<< adjoints[yield.idx] << endl;
	//  -61.846
	cout << "Derivative to volatility (vega) = "
		<< adjoints[vol.idx] << endl;
	//  46.980
	cout << "Derivative to strike (-digital) = "
		<< adjoints[strike.idx] << endl;
	//  -0.235
	cout << "Derivative to maturity (-theta) = "
		<< adjoints[mat.idx] << endl;
	//  1.321

	//  clear
	tape.clear();
}
int main() {
	test_product<AADNumber>();
}
