#pragma once
#include "visitor.h"

namespace QuantScript {
    class VarIndexer : public Visitor {
        std::map<std::string, unsigned> myVarMap;
    public:
        ~VarIndexer() {};
        void visitVar(NodeVar& node) override;
        std::vector<std::string> getVarNames() const;
    };
}