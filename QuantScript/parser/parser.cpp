#include "parser/parser.h"
#include <assert.h>

namespace QuantScript {
	std::vector<std::string> tokenize(const std::string& str)
	{
		// Regex matching tokens of interest
                static const std::regex r("[\\w.]+|[/-]|,|[\\(\\)\\{\\}\\+\\*\\^]|!=|>=|<=|[<>=]");

		// Result, with max possible size reserved
		std::vector<std::string> v;
		v.reserve(str.size());
		// Loop over matches
		for (std::sregex_iterator it(str.begin(), str.end(), r), end; it != end; ++it)
		{
			// Copy match into results
			v.push_back((*it)[0]);
			// Uppercase
			transform(v.back().begin(), v.back().end(), v.back().begin(), ::toupper);
		};
		// C++11 move semantics means no copy
		return v;
	};
	Event parse(const std::string& eventString) {
		Event e;
		auto tokens = tokenize(eventString);
		auto it = tokens.begin();
		while (it != tokens.end())
		{
			e.push_back(Parser<decltype(it)>::parseStatement(it, tokens.end()));
		}
		return e;
	};

	//Macro replacement
	std::string macroReplacer(const std::map<std::string, std::string>& macroMap, const std::string& eventString) {
		std::string str = eventString;
		bool isFunc;
		static const std::regex funcSignatureRegex("\\w+(?=\\()");
		static const std::regex parametersRegex("(\\w+|\w+\\(\\))(?=[\\),])");
		std::string funcSignature, replacedMacro;
			
		std::vector<std::string> macroFuncArgs, scriptFuncArgs;

		std::string incompleteFuncRegex = "\\(.*\\)+";
		std::smatch sm;
		for (auto& macro : macroMap) {
			//1 check if is func or const
			isFunc = std::regex_search(macro.first, sm, funcSignatureRegex);
			if (isFunc) {		
				funcSignature = sm.str();
				macroFuncArgs = getMacroFuncArgs(macro.first, parametersRegex); //might be scriptFuncArgs?
				std::regex completeFuncRegex(funcSignature + incompleteFuncRegex);
				while (std::regex_search(str, sm, completeFuncRegex)) {
					scriptFuncArgs = scriptVarSplit(sm.str(), funcSignature);
					if (scriptFuncArgs.size() != macroFuncArgs.size()) {
						throw script_error("Script Macro call does not match its definition (Check Macro def or script call).");
					}
					else {
						replacedMacro = macroVarReplacer(macroFuncArgs, scriptFuncArgs, macro.second);						
						str = std::regex_replace(str, completeFuncRegex, replacedMacro);
					}
					//str = sm.suffix().str();
				}
			}
			else {
				str = std::regex_replace(str, std::regex(macro.first), macro.second);
			}

		}
		return str;
	};
	std::vector<std::string> getMacroFuncArgs(const std::string& funcString, const std::regex& parametersRegex) {
		std::string args = funcString;
		std::vector<std::string> results;
		std::smatch matches;
		while (std::regex_search(args, matches, parametersRegex)) {
			results.push_back(matches.str(1));
			args = matches.suffix().str();
		}
		return results;
	}
	std::string macroVarReplacer(const std::vector<std::string>& macroArgs, const std::vector<std::string>& scriptArgs, const std::string& funcString) {
		std::string result = funcString;
		std::regex re;
		assert(macroArgs.size() == scriptArgs.size());
		for (size_t i = 0; i < macroArgs.size(); i++) {
			re = std::regex(macroArgs[i]+"(?!\\w)");
			result = std::regex_replace(result, re, scriptArgs[i]);
		}
		return result;
	};
	std::vector<std::string> scriptVarSplit(const std::string& s, const std::string& funcSignature) {
		std::string str = std::regex_replace(s, std::regex(funcSignature), "");
		std::vector<std::string> result;
		std::string sb = "";
		int parenCount = 0;
		for (int i = 1; i < str.length() - 1; i++) { // go from 1 to length -1 to discard the surrounding ()
			char c = str[i];
			if (c == '(') parenCount++;
			else if (c == ')') parenCount--;
			if (parenCount == 0 && c == ',') {
				result.push_back(sb);
				sb = ""; // clear string builder
			}
			else {
				sb += c;
			}
		}
		result.push_back(sb);
		return result;
	}
};