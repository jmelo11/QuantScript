#include "product/product.h"
#include "parser/parser.h"

namespace QuantScript {
    const std::vector<Date>& Product::eventDates() {
        return myEventDates; 
    };
    void Product::visit(Visitor& visitor) {
        for (auto& e : myEvents) {
            for (auto& s : e) {
                s->acceptVisitor(visitor);
            };
        };
    };
    void Product::indexVariables() {
        VarIndexer indexer;
        visit(indexer);
        myVariables = indexer.getVarNames();
    };
    std::vector<std::string> Product::varNames() {
        return myVariables;
    };
}