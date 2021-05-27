#pragma once
#include <string>
#include <map>
#include "visitor.h"

namespace QuantScript {
    class DefinitionIndexer : public Visitor {
        std::map<std::string, double> myDefinitionMap;
    public:
        ~DefinitionIndexer() {};
        std::vector<std::string> getVarNames() const;
    };
}