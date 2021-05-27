#pragma once

#include <vector>
#include <random>
#include <memory>
#include <stdexcept>
#include <map>
#include "nodes/nodes.h"

namespace QuantScript {
	template <class T>
	struct SimulationData{
		T spot;
		T numeraire;
	};

	template <class T>
	using Scenario = std::vector<SimulationData<T>>;

	struct randomgen_error : public std::runtime_error
	{
		randomgen_error(const char msg[]) : std::runtime_error(msg) {}
	};

	class RandomGen	{
		public:
			// Initialise for a given dimension
			virtual void init(const size_t dim) = 0;
			virtual void genNextNormVec() = 0;
			// Access Gaussian vector byRef
			virtual const std::vector<double>& getNorm() const = 0;
			// Clone
			virtual std::unique_ptr<RandomGen> clone() const = 0;
			// Skip ahead (for parallel Monte-Carlo)
			virtual void skipAhead(const long skip){
				throw randomgen_error("Concrete random generator cannot be used for parallel simulations");
			}
	};

	class BasicRanGen : public RandomGen
	{
		std::default_random_engine myEngine;
		std::normal_distribution<> myDist;
		size_t myDim;
		std::vector<double> myNormVec;
	public:
		BasicRanGen(const unsigned seed = 0){
			myEngine = seed > 0 ?
				std::default_random_engine(seed) :
				std::default_random_engine();
			myDist = std::normal_distribution<>();
		}
		void init(const size_t dim) override
		{
			myDim = dim;
			myNormVec.resize(dim);
		}
		void genNextNormVec() override {
			for (size_t i = 0; i < myDim; ++i){
				myNormVec[i] = myDist(myEngine);
			}
		};
		const std::vector<double>& getNorm() const override {
			return myNormVec;
		};
		// Clone
		std::unique_ptr<RandomGen> clone() const override
		{
			return std::unique_ptr<RandomGen>(new BasicRanGen(*this));
		}
	};

	template <class T>
	struct Model
	{
		// Clone
		virtual std::unique_ptr<Model<T>> clone() const = 0;
		// Initialize simulation dates
		virtual void initSimDates(const std::vector<Date>& simDates) = 0;
		// Number of Gaussian numbers required for one path
		virtual size_t dim() const = 0;
		// Apply the model SDE
		virtual void applySDE(const std::vector<double>& G, std::vector<T>& spots, std::vector<T>& numeraire) const = 0;
	};

	template <class T>
	class SimpleBlackScholes : public Model<T>{
		Date myToday;
		T mySpot;
		T myVol;
		T myRate;
		T myDrift;
		bool myTime0; // If today is among simul dates
		std::vector<double> myTimes;
		std::vector<double> myDt;
		std::vector<double> mySqrtDt;
	private:
		void calcDf(std::vector<T>& dfs) const
		{
			for (size_t i = 0; i < myTimes.size(); ++i)
				// Despite the name of the variable, this is not the discount factor
				// but its inverse, hence, the numeraire
				dfs[i] = exp(myRate * myTimes[i]);
		}
	public:
		// Construct with T0, S0, vol and rate
		SimpleBlackScholes(const Date& today, T spot, T vol, T rate): 
			myToday(today), mySpot(spot), myVol(vol), myRate(rate), myDrift(0.5 * vol * vol){}
		// clone
		virtual std::unique_ptr<Model<T>> clone() const override{
			return std::unique_ptr<Model<T>>(new SimpleBlackScholes(*this));
		}
		// Parameter accessors, read only
		const T& spot() { return mySpot; }
		const T& rate() { return myRate; }
		const T& vol() { return myVol; }
		// Initialize simulation dates
		void initSimDates(const std::vector<Date>& simDates) override{
			myTime0 = simDates[0] == myToday;
			// Fill array of times
			for (auto dateIt = simDates.begin();
				dateIt != simDates.end();
				++dateIt)
			{
				myTimes.push_back(double(*dateIt - myToday) / 360);
			}
			myDt.resize(myTimes.size());
			myDt[0] = myTimes[0];
			for (size_t i = 1; i < myTimes.size(); ++i)
			{
				myDt[i] = myTimes[i] - myTimes[i - 1];
			}
			mySqrtDt.resize(myTimes.size());
			for (size_t i = 0; i < myTimes.size(); ++i) {
				mySqrtDt[i] = sqrt(myDt[i]);
			}
		}
		size_t dim() const override { return myTimes.size() - myTime0; }
		// Simulate one path
		void applySDE(const std::vector<double>& G, std::vector<T>& spots, std::vector<T>& numeraires) const override {
			calcDf(numeraires);
			// Apply the SDE
			size_t step = 0;
			// First step
			spots[0] = myTime0 ?
				mySpot :
				mySpot * exp((myRate-myDrift) * myDt[0] + myVol * mySqrtDt[0] * G[step++]);
			// All steps
			for (size_t i = 1; i < myTimes.size(); ++i)
			{
				spots[i] = spots[i - 1]* exp((myRate-myDrift) * myDt[i] + myVol * mySqrtDt[i] * G[step++]);
			}
		}
	};

	template <class T>
	class MonteCarloSimulator
	{
		RandomGen& myRandomGen;
		Model<T>& myModel;
	public:
		MonteCarloSimulator(Model<T>& model, RandomGen& ranGen) : myRandomGen(ranGen), myModel(model) {}
		void init(const std::vector<Date>& simDates){
			myModel.initSimDates(simDates);
			myRandomGen.init(myModel.dim());
		}
		void simulateOnePath(std::vector<T>& spots, std::vector<T>& numeraire){
			myRandomGen.genNextNormVec();
			myModel.applySDE(myRandomGen.getNorm(), spots, numeraire);
		}
	};
	
	template <class T>
	struct ScriptModelApi{
		virtual void initForScripting(const std::vector<Date>& eventDates) = 0;
		virtual void nextScenario(Scenario<T>& s) = 0;
	};

	template <class T>
	class ScriptSimulator : public MonteCarloSimulator<T>, public ScriptModelApi<T>{
		std::vector<T> myTempSpots;
		std::vector<T> myTempNumeraires;
	public:
		ScriptSimulator(Model<T>& model, RandomGen& ranGen): MonteCarloSimulator<T>(model, ranGen) {}
		void initForScripting(const std::vector<Date>& eventDates) override{
			MonteCarloSimulator<T>::init(eventDates);
			myTempSpots.resize(eventDates.size());
			myTempNumeraires.resize(eventDates.size());
		}
		void nextScenario(Scenario<T>& s) override{
			MonteCarloSimulator<T>::simulateOnePath(myTempSpots, myTempNumeraires);
			// Note the inefficiency
			for (size_t i = 0; i < s.size(); ++i) {
				s[i].spot = myTempSpots[i];
				s[i].numeraire = myTempNumeraires[i];
			}
		}
	};
}