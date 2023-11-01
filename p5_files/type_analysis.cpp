#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "types.hpp"
#include "name_analysis.hpp"
#include "type_analysis.hpp"

namespace drewno_mars{

TypeAnalysis * TypeAnalysis::build(NameAnalysis * nameAnalysis){
	//To emphasize that type analysis depends on name analysis
	// being complete, a name analysis must be supplied for 
	// type analysis to be performed.
	TypeAnalysis * typeAnalysis = new TypeAnalysis();
	auto ast = nameAnalysis->ast;	
	typeAnalysis->ast = ast;

	ast->typeAnalysis(typeAnalysis);
	if (typeAnalysis->hasError){
		return nullptr;
	}

	return typeAnalysis;

}

void ProgramNode::typeAnalysis(TypeAnalysis * ta){

	//pass the TypeAnalysis down throughout
	// the entire tree, getting the types for
	// each element in turn and adding them
	// to the ta object's hashMap
	for (auto global : *myGlobals){
		global->typeAnalysis(ta);
	}

	//The type of the program node will never
	// be needed. We can just set it to VOID
	//(Alternatively, we could make our type 
	// be error if the DeclListNode is an error)
	ta->nodeType(this, BasicType::produce(VOID));
}

void FnDeclNode::typeAnalysis(TypeAnalysis * ta){

	//HINT: you might want to change the signature for
	// typeAnalysis on FnBodyNode to take a second
	// argument which is the type of the current 
	// function. This will help you to know at a 
	// return statement whether the return type matches
	// the current function

	//Note: this function may need extra code
	//we can use this code for FnDecl?
	// IDNode * myId = this->ID();
	// SemSymbol * nameSymbol = myID->getSymbol();
	// const DataType * nameType = nameSymbol->getDataType();
	// const FnType * symAsFn = nameType->asFn();

	// ta->setCurrentFnType(symAsFn);

	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta);
	}
}

void StmtNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Implement me in the subclass");
}


void GiveStmtNode::typeAnalysis(TypeAnalysis * ta){
  mySrc->typeAnalysis(ta);
  const DataType * srcType = ta->nodeType(mySrc);
	if (srcType->asError()){
		ta->nodeType(this, ErrorType::produce());
	}
  if ( srcType->isBool() || srcType->isInt() || srcType->isString() ) {
    //give statement doesn't have a type, so we just return.
    return;
  }
 //cant test these until we implement more stuff... but i think this works 
  if (srcType->asFn()) {
    ta->errOutputFn(mySrc->pos());
  } else if (srcType->asClass()) {
    ta->errOutputClass(this->pos());
  } else if (srcType->isVoid()) {
    ta->errOutputVoid(mySrc->pos());
  }

}

/*Assignment access: AssignStmtNode*/
void AssignStmtNode::typeAnalysis(TypeAnalysis * ta){
	//TODO: Note that this function is incomplete. 
	// and needs additional code

	//Do typeAnalysis on the subexpressions
	myDst->typeAnalysis(ta);
	mySrc->typeAnalysis(ta);

	const DataType * tgtType = ta->nodeType(myDst);
	const DataType * srcType = ta->nodeType(mySrc);


	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself. It 
	// sort of serves as a way to cast the subtype
	if (tgtType->asError() || srcType->asError()){
		ta->nodeType(this, ErrorType::produce());
	}


	//While incomplete, this gives you one case for 
	// assignment: if the types are exactly the same
	// it is usually ok to do the assignment. One
	// exception is that if both types are function
	// names, it should fail type analysis
	if (tgtType == srcType){
		ta->nodeType(this, tgtType);
		return;
	}
	
	//Some functions are already defined for you to
	// report type errors. Note that these functions
	// also tell the typeAnalysis object that the
	// analysis has failed, meaning that main.cpp
	// will print "Type check failed" at the end
	ta->errAssignOpr(this->pos());


	// Here, we set the type of the assignment
	// to void to indicate no error was found.
	// This step is optional, since you'll never
	// use the type of a statement
	ta->nodeType(this, BasicType::produce(VOID));

}

void ExpNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void DeclNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void VarDeclNode::typeAnalysis(TypeAnalysis * ta){
	// VarDecls always pass type analysis, since they 
	// are never used in an expression. You may choose
	// to type them void (like this), as discussed in class
	ta->nodeType(this, BasicType::produce(VOID));
}


void IDNode::typeAnalysis(TypeAnalysis * ta){
	// IDs never fail type analysis and always
	// yield the type of their symbol (which
	// depends on their definition)
	ta->nodeType(this, this->getSymbol()->getDataType());
}

void IntLitNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(INT));
}

void TrueNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(BOOL));
}

void FalseNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(BOOL));
}

void StrLitNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, BasicType::produce(STRING));
}

/*Logical operators and conditions: Binary - AndNode, OrNode, Unary - NotNode
-> All operands are bool. The result type is bool in legal cases, ERROR otherwise.
*/
void AndNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isBool() && type2->isBool())) {
		ta->errLogicOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

void OrNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isBool() && type2->isBool())) {
		ta->errLogicOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}
void NotNode::typeAnalysis(TypeAnalysis * ta) {
	myExp->typeAnalysis(ta);
	const DataType * expType = ta->nodeType(myExp);

	if (expType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!(expType->isBool())) {
		ta->errLogicOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}

/* Arithmetic operations: Binary - PlusNode, MinusNode, TimesNode, DivideNode, Unary - NegNode, postincrement, postdecrement
-> Operands are both int - the result type is int. In all illegal cases, the result type is ERROR.
*/
void PlusNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isInt() && type2->isInt())) {
		ta->errMathOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void MinusNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isInt() && type2->isInt())) {
		ta->errMathOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void TimesNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isInt() && type2->isInt())) {
		ta->errMathOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void DivideNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isInt() && type2->isInt())) {
		ta->errMathOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void NegNode::typeAnalysis(TypeAnalysis * ta) {
	myExp->typeAnalysis(ta);
	const DataType * expType = ta->nodeType(myExp);

	if (expType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!(expType->isInt())) {
		ta->errMathOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
}

void PostIncStmtNode::typeAnalysis(TypeAnalysis * ta) {
	// myLoc->typeAnalysis(ta);
	const DataType * expType = myLoc->getSymbol()->getDataType();
	if (expType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!expType->isInt()) {
		ta->errMathOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
}
void PostDecStmtNode::typeAnalysis(TypeAnalysis * ta) {
	// myLoc->typeAnalysis(ta);
	const DataType * expType = myLoc->getSymbol()->getDataType();
	if (expType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!expType->isInt()) {
		ta->errMathOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
}


/* Relational operations: LessNode, LessEqNode, GreaterNode, GreaterEqNode
-> Both operands are int. The result type is bool in legal cases, ERROR otherwise.
*/
void LessNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isInt() && type2->isInt())) {
		ta->errRelOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}
void LessEqNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isInt() && type2->isInt())) {
		ta->errRelOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}
void GreaterNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isInt() && type2->isInt())) {
		ta->errRelOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}
void GreaterEqNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!(type1->isInt() && type2->isInt())) {
		ta->errRelOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}


/* Equality operations: EqualsNode, NotEqualsNode
-> Both operands are of the same primitive type
Neither operands are of the same class type and Neither operand is a function type
result type is bool
*/
void EqualsNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (type1 != type2) {
		ta->errEqOpr(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if ((type1->isClass() || type1->isVoid() || type2->isClass() || type2->isVoid())) {
		ta->errEqOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}
void NotEqualsNode::typeAnalysis(TypeAnalysis * ta) {
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (type1 != type2) {
		ta->errEqOpr(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if ((type1->isClass() || type1->isVoid() || type2->isClass() || type2->isVoid())) {
		ta->errEqOpd(pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
}


/*Member access operations: dereferance
The index operand is a field name of the base class type
The result type is the type of the field in legal cases, and ERROR otherwise.
*/

}
