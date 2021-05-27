#include "visitors/visitor.h"

namespace QuantScript {
    //VISITOR
    void NodeUplus::acceptVisitor(Visitor& visitor) {visitor.visitUplus(*this);}; 
    void NodeUminus::acceptVisitor(Visitor& visitor) {visitor.visitUminus(*this);}; 
    void NodeAdd::acceptVisitor(Visitor& visitor) {visitor.visitAdd(*this);}; 
    void NodeSubtract::acceptVisitor(Visitor& visitor) {visitor.visitSubtract(*this);}; 
    void NodeMult::acceptVisitor(Visitor& visitor) {visitor.visitMult(*this);}; 
    void NodeDiv::acceptVisitor(Visitor& visitor) {visitor.visitDiv(*this);}; 

    //advanced
    void NodePow::acceptVisitor(Visitor& visitor) {visitor.visitPow(*this);}; 
    void NodeLog::acceptVisitor(Visitor& visitor) {visitor.visitLog(*this);}; 
    void NodeSqrt::acceptVisitor(Visitor& visitor) {visitor.visitSqrt(*this);}; 
    void NodeMax::acceptVisitor(Visitor& visitor) {visitor.visitMax(*this);}; 
    void NodeMin::acceptVisitor(Visitor& visitor) {visitor.visitMin(*this);}; 

    //logic
    void NodeAssign::acceptVisitor(Visitor& visitor) {visitor.visitAssign(*this);}; 
    void NodeEqual::acceptVisitor(Visitor& visitor) {visitor.visitEqual(*this);}; 
    void NodeDifferent::acceptVisitor(Visitor& visitor) {visitor.visitDifferent(*this);}; 
    void NodeSuperior::acceptVisitor(Visitor& visitor) {visitor.visitSuperior(*this);}; 
    void NodeSupEqual::acceptVisitor(Visitor& visitor) {visitor.visitSupEqual(*this);}; 
    void NodeInferior::acceptVisitor(Visitor& visitor) {visitor.visitInferior(*this);}; 
    void NodeInfEqual::acceptVisitor(Visitor& visitor) {visitor.visitInfEqual(*this);}; 
    void NodeAnd::acceptVisitor(Visitor& visitor) {visitor.visitAnd(*this);}; 
    void NodeOr::acceptVisitor(Visitor& visitor) {visitor.visitOr(*this);}; 


    //Constants and variables
    void NodeIf::acceptVisitor(Visitor& visitor) { visitor.visitIf(*this); };
    void NodeSpot::acceptVisitor(Visitor& visitor) { visitor.visitSpot(*this); };
    void NodeConst::acceptVisitor(Visitor& visitor) { visitor.visitConst(*this); };
    void NodeVar::acceptVisitor(Visitor& visitor) { visitor.visitVar(*this); };
    

    //CONST VISITOR
    void NodeUplus::acceptVisitor(ConstVisitor& visitor) const { visitor.visitUplus(*this); };
    void NodeUminus::acceptVisitor(ConstVisitor& visitor) const { visitor.visitUminus(*this); };
    void NodeAdd::acceptVisitor(ConstVisitor& visitor) const { visitor.visitAdd(*this); };
    void NodeSubtract::acceptVisitor(ConstVisitor& visitor) const { visitor.visitSubtract(*this); };
    void NodeMult::acceptVisitor(ConstVisitor& visitor) const { visitor.visitMult(*this); };
    void NodeDiv::acceptVisitor(ConstVisitor& visitor) const { visitor.visitDiv(*this); };

    //advanced
    void NodePow::acceptVisitor(ConstVisitor& visitor) const { visitor.visitPow(*this); };
    void NodeLog::acceptVisitor(ConstVisitor& visitor) const { visitor.visitLog(*this); };
    void NodeSqrt::acceptVisitor(ConstVisitor& visitor) const { visitor.visitSqrt(*this); };
    void NodeMax::acceptVisitor(ConstVisitor& visitor) const { visitor.visitMax(*this); };
    void NodeMin::acceptVisitor(ConstVisitor& visitor) const { visitor.visitMin(*this); };

    //logic
    void NodeAssign::acceptVisitor(ConstVisitor& visitor) const { visitor.visitAssign(*this); };
    void NodeEqual::acceptVisitor(ConstVisitor& visitor) const { visitor.visitEqual(*this); };
    void NodeDifferent::acceptVisitor(ConstVisitor& visitor) const { visitor.visitDifferent(*this); };
    void NodeSuperior::acceptVisitor(ConstVisitor& visitor) const { visitor.visitSuperior(*this); };
    void NodeSupEqual::acceptVisitor(ConstVisitor& visitor) const { visitor.visitSupEqual(*this); };
    void NodeInferior::acceptVisitor(ConstVisitor& visitor) const { visitor.visitInferior(*this); };
    void NodeInfEqual::acceptVisitor(ConstVisitor& visitor) const { visitor.visitInfEqual(*this); };
    void NodeAnd::acceptVisitor(ConstVisitor& visitor) const { visitor.visitAnd(*this); };
    void NodeOr::acceptVisitor(ConstVisitor& visitor) const { visitor.visitOr(*this); };


    //Constants and variables
    void NodeIf::acceptVisitor(ConstVisitor& visitor) const { visitor.visitIf(*this); };
    void NodeSpot::acceptVisitor(ConstVisitor& visitor) const { visitor.visitSpot(*this); };
    void NodeConst::acceptVisitor(ConstVisitor& visitor) const { visitor.visitConst(*this); };
    void NodeVar::acceptVisitor(ConstVisitor& visitor) const { visitor.visitVar(*this); };
    
    void NodePays::acceptVisitor(Visitor& visitor){
        visitor.visitPays(*this);
    }
    void NodePays::acceptVisitor(ConstVisitor& visitor) const
    {
        visitor.visitPays(*this);
    }


    //new features   
    void NodeSolver::acceptVisitor(Visitor& visitor) { visitor.visitSolver(*this); };
    void NodeSolver::acceptVisitor(ConstVisitor& visitor) const { visitor.visitSolver(*this); };
    void NodeDefinition::acceptVisitor(Visitor& visitor) { visitor.visitDefinition(*this); };
    void NodeDefinition::acceptVisitor(ConstVisitor& visitor) const { visitor.visitDefinition(*this); };

}