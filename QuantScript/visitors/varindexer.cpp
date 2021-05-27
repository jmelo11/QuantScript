#include "varindexer.h"
namespace QuantScript {
    void VarIndexer::visitVar(NodeVar& node) {     
        std::map<std::string, unsigned>::iterator it = myVarMap.find(node.name);
        if (it == myVarMap.end()) {
            node.index = myVarMap[node.name] = myVarMap.size();
        }
        else {
            node.index = it->second;
        };
       
    };
    std::vector<std::string> VarIndexer::getVarNames() const {
        std::vector<std::string> varVector(myVarMap.size());
        for (auto const& [key, value] : myVarMap) {
            varVector[value] = key;
        };
        return varVector; 
    };
}