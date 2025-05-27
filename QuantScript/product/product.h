#pragma once
#include "nodes/nodes.h"
#include "visitors/varindexer.h"
#include "visitors/evaluator.h"
#include "visitors/solverevaluator.h"
#include "models/models.h"
#include "parser/parser.h"

#include <cstdint>
#include <functional>
#include <iostream>

namespace QuantScript
{
    class ObjFunc;

    class Product
    {
        std::vector<Date> myEventDates;
        std::vector<Event> myEvents;
        std::vector<std::string> myVariables;

    public:
        const std::vector<Date> &eventDates();
        void visit(Visitor &visitor);
        void indexVariables();
        template <class T>
        void evaluate(const Scenario<T> &scenario, Evaluator<T> &evaluator)
        {
            evaluator.setScenario(&scenario);
            for (size_t i = 0; i < myEvents.size(); i++)
            {
                evaluator.setCurrentEvent(i);
                for (auto &statement : myEvents[i])
                {
                    evaluator.visit(statement);
                };
            };
        };

        // Return variable names
        std::vector<std::string> varNames();
        // Evaluator Factory
        template <class T>
        std::unique_ptr<Evaluator<T>> buildEvaluator()
        {
            // Move
            return std::unique_ptr<Evaluator<T>>(new Evaluator<T>(myVariables.size()));
        };
        // Scenario factory
        template <class T>
        std::unique_ptr<Scenario<T>> buildScenario()
        {
            // Move
            return std::unique_ptr<Scenario<T>>(new Scenario<T>(myEventDates.size()));
        };
        // Solver factory
        template <class T>
        std::unique_ptr<SolverEvaluator<T>> buildSolver(std::string target, T value)
        {
            // Move
            return std::unique_ptr<SolverEvaluator<T>>(new SolverEvaluator<T>(target, value));
        };

        // Event Parser
        template <class EventIt>
        void parseEvents(EventIt begin, EventIt end)
        {
            for (EventIt evtIt = begin; evtIt != end; ++evtIt)
            {
                // Copy event date
                myEventDates.push_back(evtIt->first);
                // Parse event string
                myEvents.push_back(parse(evtIt->second));
            };
        };
        ;
    };

    // Aux class for solver method
    class ObjFunc
    {
        Product *myProduct;

    public:
        ObjFunc(Product *prod) : myProduct(prod) {};
        template <class T>
        double operator()(T const &x, const Scenario<T> &scenario, Evaluator<T> &evaluator, SolverEvaluator<T> &solver)
        {
            solver.update(x);
            myProduct->evaluate(scenario, evaluator);
            std::vector<T> varValues = evaluator.varVals();
            evaluator.init();
            return std::pow(varValues[solver.getTargetIndex()] - solver.getTargetValue(), 2);
        }
    };
}