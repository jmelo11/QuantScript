#pragma once

#include <regex>
#include <algorithm>
#include <map>

#include "nodes/nodes.h"

namespace QuantScript
{

	struct script_error : public std::runtime_error
	{
		script_error(const char msg[]) : std::runtime_error(msg) {}
	};

	std::vector<std::string> tokenize(const std::string &str);
	Event parse(const std::string &eventString);
	std::string macroReplacer(const std::map<std::string, std::string> &macroMap, const std::string &eventString);
	std::string macroVarReplacer(const std::vector<std::string> &macroArgs, const std::vector<std::string> &scriptArgs, const std::string &funcString);
	std::vector<std::string> getMacroFuncArgs(const std::string &funcString, const std::regex &parametersRegex);
	std::vector<std::string> scriptVarSplit(const std::string &s, const std::string &funcSignature);

	template <class TokIt>
	class Parser
	{
	public:
		// Statement = ExprTree = unique_ptr<Node>
		static Statement parseStatement(TokIt &cur, const TokIt end)
		{
			// Check for instructions of type 1, so far only ’if’
			if (*cur == "IF")
				return parseIf(cur, end);
			// Parse cur as a variable
			auto lhs = parseVar(cur);
			// Check for end
			if (cur == end)
				throw script_error("Unexpected end of statement");
			// Check for instructions of type 2, so far only assignment
			if (*cur == "PAYS")
				return parsePays(cur, end, lhs);
			if (*cur == "=")
				return parseAssign(cur, end, lhs);
			// No instruction, error
			throw script_error("Statement without an instruction");
			return Statement();
		};
		static ExpressionTree parsePays(TokIt &cur, const TokIt end, ExpressionTree &lhs)
		{
			++cur;
			// Check for end
			if (cur == end)
				throw script_error("Unexpected end of statement");
			// Parse the RHS
			auto rhs = parseExpr(cur, end);
			// Build and return the top node
			return buildBinary<NodePays>(lhs, rhs);
		}
                static ExpressionTree parseIf(TokIt &cur, const TokIt end)
                {
                        // Advance to token immediately following "if"
                        ++cur;
                        // Check for end
                        if (cur == end)
                                throw script_error("'If' is not followed by condition");
                        // Parse the condition
                        auto cond = parseCondElem(cur, end);

                        if (cur == end)
                                throw script_error("'If' has no body");

                        bool useBraces = false;
                        if (*cur == "THEN")
                        {
                                ++cur;
                        }
                        else if (*cur == "{")
                        {
                                useBraces = true;
                                ++cur;
                        }
                        else
                                throw script_error("'If' is not followed by 'then' or '{'");

                        std::vector<Statement> stats;
                        while (cur != end && *cur != "ELSE" && *cur != (useBraces ? "}" : "ENDIF"))
                                stats.push_back(parseStatement(cur, end));

                        if (cur == end)
                                throw script_error("If block not terminated");

                        std::vector<Statement> elseStats;
                        int elseIdx = -1;

                        if (*cur == "ELSE")
                        {
                                ++cur;
                                bool elseBraces = false;
                                if (useBraces)
                                {
                                        if (cur == end || *cur != "{")
                                                throw script_error("Else block must start with '{'");
                                        elseBraces = true;
                                        ++cur;
                                }
                                while (cur != end && *cur != (elseBraces ? "}" : "ENDIF"))
                                        elseStats.push_back(parseStatement(cur, end));
                                if (cur == end)
                                        throw script_error("Else block not terminated");
                                if (elseBraces)
                                        ++cur; // over '}'
                                else
                                        ; // ENDIF consumed later
                                elseIdx = stats.size() + 1;
                        }

                        auto top = make_node<NodeIf>();
                        top->arguments.resize(1 + stats.size() + elseStats.size());
                        top->arguments[0] = std::move(cond);

                        for (size_t i = 0; i < stats.size(); ++i)
                                top->arguments[i + 1] = std::move(stats[i]);

                        for (size_t i = 0; i < elseStats.size(); ++i)
                                top->arguments[i + elseIdx] = std::move(elseStats[i]);

                        top->firstElse = elseIdx;

                        if (*cur == "ENDIF")
                        {
                                ++cur;
                        }
                        else if (*cur == "}")
                        {
                                ++cur;
                                if (elseIdx != -1 && cur != end && *cur == "ENDIF")
                                        ++cur; // support mixed '}' before ENDIF if else had no braces
                        }
                        else
                        {
                                // if we reached here, termination token is part of else parsing
                        }
                        return std::move(top);
                };
		static ExpressionTree parseVar(TokIt &cur)
		{
			// Check that the variable name starts with a letter
			if ((*cur)[0] < 'A' || (*cur)[0] > 'Z')
				throw script_error((std::string("Variable name ") + *cur + " is invalid").c_str());
			// Build the var node
			auto top = make_node<NodeVar>(*cur);
			// Advance over var and return
			++cur;
			return std::move(top);
			// Explicit std::move is necessary
			// because we return a base class pointer
		};
		static ExpressionTree parseAssign(TokIt &cur, const TokIt end, ExpressionTree &lhs)
		{
			// Advance to token immediately following "="
			++cur;
			// Check for end
			if (cur == end)
				throw script_error("Unexpected end of statement");
			// Parse the RHS
			auto rhs = parseExpr(cur, end);
			// Build and return the top node
			return buildBinary<NodeAssign>(lhs, rhs);
		};

		static ExpressionTree parseCond(TokIt &cur,
										const TokIt end)
		{
			// First exhaust all L2 (and) and above (elem)
			// conditions on the lhs
			auto lhs = parseCondL2(cur, end);
			// Do we have an ’or’?
			while (cur != end && *cur == "OR")
			{
				// Advance cur over ’or’ and parse the rhs
				++cur;
				// Should not stop straight after ’or’
				if (cur == end)
					throw script_error("Unexpected end of statement");
				// Exhaust all L2 (and) and above (elem)
				// conditions on the rhs
				auto rhs = parseCondL2(cur, end);
				// Build node and assign lhs and rhs as its arguments,
				// store in lhs
				lhs = buildBinary<NodeOr>(lhs, rhs);
			}
			// No more ’or’, and L2 and above were exhausted,
			// hence condition is complete
			return lhs;
		};

		static ExpressionTree parseCondElem(TokIt &cur, const TokIt end)
		{
			// Parse the LHS expression
			auto lhs = parseExpr(cur, end);
			// Check for end
			if (cur == end)
				throw script_error("Unexpected end of statement");
			// Advance to token immediately following the comparator
			std::string comparator = *cur;
			++cur;
			// Check for end
			if (cur == end)
				throw script_error("Unexpected end of statement");
			// Parse the RHS
			auto rhs = parseExpr(cur, end);
			// Build the top node, set its arguments and return
			if (comparator == "=")
				return buildBinary<NodeEqual>(lhs, rhs);
			else if (comparator == "!=")
				return buildBinary<NodeDifferent>(lhs, rhs);
			else if (comparator == "<")
				return buildBinary<NodeInferior>(lhs, rhs);
			else if (comparator == ">")
				return buildBinary<NodeSuperior>(lhs, rhs);
			else if (comparator == "<=")
				return buildBinary<NodeInfEqual>(lhs, rhs);
			else if (comparator == ">=")
				return buildBinary<NodeSupEqual>(lhs, rhs);
			else
				throw script_error("Elementary condition has no valid comparator");
		};
		template <char OpChar, char ClChar>
		static TokIt findMatch(TokIt cur, const TokIt end)
		{
			unsigned opens = 1;
			++cur;
			while (cur != end && opens > 0)
			{
				opens += ((*cur)[0] == OpChar) - ((*cur)[0] == ClChar);
				++cur;
			}
			if (cur == end && opens > 0)
				throw script_error((std::string("Opening ") + OpChar + " has not matching closing " + ClChar).c_str());
			return --cur;
		};
		static ExpressionTree parseCondL2(TokIt &cur, const TokIt end)
		{
			// First parse the leftmost elem condition
			auto lhs = parseCondElem(cur, end);
			// Do we have an ’and’?
			while (cur != end && *cur == "AND")
			{
				// Advance cur over ’and’ and parse the rhs
				++cur;
				// Should not stop straight after ’and’
				if (cur == end)
					throw script_error("Unexpected end of statement");
				// Parse the rhs elem condition
				auto rhs = parseCondElem(cur, end);
				// Build node and assign lhs and rhs as its arguments,
				// store in lhs
				lhs = buildBinary<NodeAnd>(lhs, rhs);
			}
			// No more ’and’,
			// so L2 and above were exhausted,
			// return to check for an or
			return lhs;
		};
		static ExpressionTree parseCondParentheses(TokIt &cur, const TokIt end)
		{
			ExpressionTree tree;
			// Do we have an opening ’(’?
			if (*cur == "(")
			{
				// Find match
				TokIt closeIt = findMatch<"(", ")">(cur, end);
				// Parse the parenthesed condition,
				// including nested parentheses,
				// by recursively calling the parent parseCond
				tree = parseCond(++cur, closeIt);
				// Advance cur after matching )
				cur = ++closeIt;
			}
			else
			{
				// No (, so leftmost we std::move one level up
				tree = parseCondElem(cur, end);
			}
			return tree;
		};
		static ExpressionTree parseVarConstFunc(TokIt &cur, const TokIt end)
		{
			// First check for constants,
			// if the char is a digit or a dot,
			// then we have a number
			if ((*cur)[0] == '.' || ((*cur)[0] >= '0' && (*cur)[0] <= '9'))
			{
				return parseConst(cur);
			}

			// Check for functions,
			// including those for accessing simulated data
			ExpressionTree top;
			unsigned minArg = 0;
			unsigned maxArg = 0;

			if (*cur == "SPOT")
			{
				top = make_base_node<NodeSpot>();
				minArg = maxArg = 0;
			}
			else if (*cur == "SOLVE")
			{
				top = make_base_node<NodeSolver>();
				minArg = maxArg = 0;
			}
			else if (*cur == "LOG")
			{
				top = make_base_node<NodeLog>();
				minArg = maxArg = 1;
			}
			else if (*cur == "SQRT")
			{
				top = make_base_node<NodeSqrt>();
				minArg = maxArg = 1;
			}
			else if (*cur == "MIN")
			{
				top = make_base_node<NodeMin>();
				minArg = 2;
				maxArg = 100;
			}
			else if (*cur == "MAX")
			{
				top = make_base_node<NodeMax>();
				minArg = 2;
				maxArg = 100;
			}
			if (top)
			{
				std::string func = *cur;
				++cur;
				// Matched a function, parse its arguments and check
				top->arguments = parseFuncArg(cur, end);
				if (top->arguments.size() < minArg || top->arguments.size() > maxArg)
					throw script_error((std::string("Function ") + func + ": wrong number of arguments").c_str());
				// Return
				return top;
			}
			// When everything else fails,
			// we have a variable
			return Parser<TokIt>::parseVar(cur);
		};

		static ExpressionTree parseConst(TokIt &cur)
		{
			// Convert to double
			double v;
			try
			{
				v = std::stod(*cur);
			}
			catch (const std::exception &)
			{
				throw script_error((*cur + " is not a number").c_str());
			}
			// Build the const node
			auto top = make_node<NodeConst>(v);
			// Advance over var and return
			++cur;
			return std::move(top);
			// Explicit move is necessary
			// because we return a base class pointer
		};
		static std::vector<ExpressionTree> parseFuncArg(TokIt &cur, const TokIt end)
		{
			// Check that we have a ’(’ and something after that
			if ((*cur)[0] != '(')
				throw script_error("No opening ( following function name");
			// Find matching ’)’
			TokIt closeIt = findMatch<'(', ')'>(cur, end);
			// Parse expressions between parentheses
			std::vector<ExpressionTree> args;
			++cur; // Over ’(’
			while (cur != closeIt)
			{
				args.push_back(parseExpr(cur, end));
				if ((*cur)[0] == ',')
					++cur;
				else if (cur != closeIt)
					throw script_error("Arguments must be separated by commas");
			};
			// Advance and return
			cur = ++closeIt;
			return args;
		};
		using ParseFunc = ExpressionTree (*)(TokIt &, const TokIt);
		template <ParseFunc FuncOnMatch, ParseFunc FuncOnNoMatch>
		static ExpressionTree parseParentheses(TokIt &cur, const TokIt end)
		{
			ExpressionTree tree;
			// Do we have an opening ’(’?
			if (*cur == "(")
			{
				// Find match
				TokIt closeIt = findMatch<'(', ')'>(cur, end);
				// Parse the parenthesed condition/expression,
				// including nested parentheses,
				// by recursively calling the parent parseCond/parseExpr
				tree = FuncOnMatch(++cur, closeIt);
				// Advance cur after matching )
				cur = ++closeIt;
			}
			else
			{
				// No (, so leftmost we move one level up
				tree = FuncOnNoMatch(cur, end);
			}
			return tree;
		};

		static ExpressionTree parseExpr(TokIt &cur, const TokIt end)
		{
			// First exhaust all L2 (’*’ and ’/’)
			// and above expressions on the lhs
			auto lhs = parseExprL2(cur, end);
			// Do we have a match?
			while (cur != end && ((*cur)[0] == '+' || (*cur)[0] == '-'))
			{
				// Record operator and advance
				char op = (*cur)[0];
				++cur;
				// Should not stop straight after operator
				if (cur == end)
					throw script_error("Unexpected end of statement");
				// Exhaust all L2 (’*’ and ’/’)
				// and above expressions on the rhs
				auto rhs = Parser<TokIt>::parseExprL2(cur, end);
				// Build node and assign lhs and rhs as its arguments,
				// store in lhs
				lhs = op == '+' ? buildBinary<NodeAdd>(lhs, rhs) : buildBinary<NodeSubtract>(lhs, rhs);
			}
			// No more match, return lhs
			return lhs;
		};
		static ExpressionTree parseExprL2(TokIt &cur, const TokIt end)
		{
			// First exhaust all L3 (’^’)
			// and above expressions on the lhs
			auto lhs = parseExprL3(cur, end);
			// Do we have a match?
			while (cur != end && ((*cur)[0] == '*' || (*cur)[0] == '/'))
			{
				// Record operator and advance
				char op = (*cur)[0];
				++cur;
				// Should not stop straight after operator
				if (cur == end)
					throw script_error("Unexpected end of statement");
				// Exhaust all L3 (’^’) and above expressions on the rhs

				auto rhs = Parser<TokIt>::parseExprL3(cur, end);
				// Build node and assign lhs and rhs as its arguments,
				// store in lhs
				lhs = op == '*' ? buildBinary<NodeMult>(lhs, rhs) : buildBinary<NodeDiv>(lhs, rhs);
			}
			// No more match, return lhs
			return lhs;
		};
		static ExpressionTree parseExprL3(TokIt &cur, const TokIt end)
		{
			// First exhaust all L4 (unaries)
			// and above expressions on the lhs
			auto lhs = parseExprL4(cur, end);
			// Do we have a match?
			while (cur != end && (*cur)[0] == '^')
			{
				// Advance
				++cur;
				// Should not stop straight after operator
				if (cur == end)
					throw script_error("Unexpected end of statement");
				// Exhaust all L4 (unaries)
				// and above expressions on the rhs
				auto rhs = Parser<TokIt>::parseExprL4(cur, end);
				// Build node and assign lhs and rhs as its arguments,
				// store in lhs
				lhs = buildBinary<NodePow>(lhs, rhs);
			}
			// No more match, return lhs
			return lhs;
		};
		static ExpressionTree parseExprL4(TokIt &cur, const TokIt end)
		{
			// Here we check for a match first
			if (cur != end && ((*cur)[0] == '+' || (*cur)[0] == '-'))
			{
				// Record operator and advance
				char op = (*cur)[0];
				++cur;
				// Should not stop straight after operator
				if (cur == end)
					throw script_error("Unexpected end of statement");
				// Parse rhs, call recursively
				// to support multiple unaries in a row
				auto rhs = parseExprL4(cur, end);
				// Build node and assign rhs as its (only) argument
				auto top = op == '+' ? make_base_node<NodeUplus>() : make_base_node<NodeUminus>();
				top->arguments.resize(1);
				// Take ownership of rhs
				top->arguments[0] = move(rhs);
				// Return the top node
				return top;
			}
			// No more match,
			// we pass on to the L5 (parentheses) parser
			return parseParentheses<parseExpr, parseVarConstFunc>(cur, end);
		};
	};
};