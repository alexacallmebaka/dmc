#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "types.hpp"
#include "name_analysis.hpp"
#include "type_analysis.hpp"

namespace drewno_mars{

TypeAnalysis * TypeAnalysis::build(NameAnalysis * nameAnalysis){ //{{{1
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
} //1}}}

void ProgramNode::typeAnalysis(TypeAnalysis * ta){ //{{{1

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
} //1}}}

void FnDeclNode::typeAnalysis(TypeAnalysis * ta){ //{{{1

	//get current function type.
	const SemSymbol * nameSymbol = myID->getSymbol();
	const DataType * nameType = nameSymbol->getDataType();
	const FnType * symAsFn = nameType->asFn();

	ta->setCurrentFnType(symAsFn);

	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta);
	}
	//we need to add the body type to know which type this fucntion return
} //1}}}

void ReturnStmtNode::typeAnalysis(TypeAnalysis * ta) {//{{{1
  const DataType * expType;
  if ( myExp ) {
    myExp->typeAnalysis(ta);                                                      
    expType = ta->nodeType(myExp);
  } else { //if nullptr, that means an empty return so we set type of expr to void.
		expType = BasicType::produce(VOID);
  }
	if (expType->asError()){
		ta->nodeType(this, ErrorType::produce());
	}
  const DataType * currentFnRetType = ta->getCurrentFnType()->getReturnType();
  	//nothing to do if types match.
  if ( expType ==  currentFnRetType ) { 
    return;
  	//if expr is void AND types don't match, it means we are trying to return void from a non-void function.
  } else if ( expType->isVoid() ) {
		//since myExpr is void, return position of return statement.
		//this is consistent with the oracle.
		ta->errRetEmpty(this->pos());
  //if current fn type is void, but types dont match we must be trying to return a value from a void function.
  } else if ( currentFnRetType->isVoid() ) {
		ta->extraRetValue(myExp->pos());
  //in all other cases, the return types simply don't match.
  } else {
		ta->errRetWrong(myExp->pos());
  }
}//1}}}

void CallExpNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
  
  //todo:
  //check formals against actuals
  //check type this exp as fn ret type
 //if lists are not same size, err out
 
  SemSymbol * nameSymbol = myCallee->getSymbol();
  const DataType * nameType = nameSymbol->getDataType();
  const FnType * symAsFn = nameType->asFn();
  
  if ( !( symAsFn ) ) {
    ta->errCallee(myCallee->pos());   
    return;
  }

  const TypeList * formalsList = symAsFn->getFormalTypes();

  if( myArgs->size() != formalsList->count() ) {
    ta->errArgCount(myCallee->pos());
  }

//work in progress
// auto formalsItr = formalsList->start();
//
// for ( auto arg : myArgs ) {
//     
//   arg->typeAnalysis(ta);
//  
//   const DataType * argType = ta->nodeType(arg);
//
//   //if one of the args errors out, the whole thing is an error.
//   if (argType->asError()) {
//     
//     ta->nodeType(this, ErrorType::produce());
//     continue;
//	}
//
//  if ( argType != *formalsItr ) {
//  
//     ta->nodeType(this, ErrorType::produce());
//     ta->errArgMatch(arg->pos);
//MagicNode
//  }
//
//  formalsItr++;
//
// }  

} //1}}}

void StmtNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	TODO("Implement me in the subclass");
} //1}}}

void GiveStmtNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
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
  } else if (srcType->isClass()) {
    ta->errOutputClass(mySrc->pos());
  } else if (srcType->isVoid()) {
    ta->errOutputVoid(mySrc->pos());
  }
} //1}}}

// take operations
void TakeStmtNode::typeAnalysis(TypeAnalysis * ta) { //{{{1
	myDst->typeAnalysis(ta);

	const DataType * dstType = ta->nodeType(myDst);

	if (dstType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (dstType->isPerfect()) {
		ta->errAssignNonLVal(myDst->pos());
		return;
	}
	if ((dstType->isBool() || dstType->isInt())) {
		return;
	}
	if (dstType->asFn()) {
    ta->errReadFn(myDst->pos());
  } else if (dstType->asClass()) {
    ta->errReadClass(myDst->pos());
  }
} //1}}}

void AssignStmtNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	bool isValid = true;
	//Do typeAnalysis on the subexpressions
	myDst->typeAnalysis(ta);
	mySrc->typeAnalysis(ta);

	const DataType * tgtType = ta->nodeType(myDst);
	const DataType * srcType = ta->nodeType(mySrc);

	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself. It 
	// sort of serves as a way to cast the subtype
	if (srcType->asError() || srcType->asError()) {
		isValid = false;
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (srcType->asFn() || srcType->isClass()) {
		ta->errAssignOpd(mySrc->pos());
		isValid = false;
	}
	if (tgtType->isPerfect()) {
		ta->errAssignNonLVal(mySrc->pos());
		isValid = false;
	}
	if (isValid && (tgtType->getString() != srcType->getString())) {
		ta->errAssignOpr(this->pos());
		isValid = false;
	}
	if (isValid) {
		ta->nodeType(this, tgtType);
	}
	ta->nodeType(this, BasicType::produce(VOID));
} //1}}}

void ExpNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	TODO("Override me in the subclass");
} //1}}}

void DeclNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	TODO("Override me in the subclass");
} //1}}}

void VarDeclNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	// VarDecls always pass type analysis, since they 
	// are never used in an expression. You may choose
	// to type them void (like this), as discussed in class
	ta->nodeType(this, BasicType::produce(VOID));
} //1}}}

void ClassDefnNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	for (auto stmt : *myMembers){
		stmt->typeAnalysis(ta);
	}
	ta->nodeType(this, BasicType::produce(CLASS));
} //1}}}

void IDNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	// IDs never fail type analysis and always
	// yield the type of their symbol (which
	// depends on their definition)
	ta->nodeType(this, this->getSymbol()->getDataType());
} //1}}}

void IntLitNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(INT));
} //1}}}

void TrueNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	ta->nodeType(this, BasicType::produce(BOOL));
} //1}}}

void FalseNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	ta->nodeType(this, BasicType::produce(BOOL));
} //1}}}

void StrLitNode::typeAnalysis(TypeAnalysis * ta){ //{{{1
	ta->nodeType(this, BasicType::produce(STRING));
} //1}}}

/*Logical operators and conditions: Binary - AndNode, OrNode, Unary - NotNode
-> All operands are bool. The result type is bool in legal cases, ERROR otherwise.
{{{1*/

void logicalOpsTypeAnalysis(BinaryExpNode * node, TypeAnalysis * ta) {
	ExpNode * myExp1 = node->getExp1();
	ExpNode * myExp2 = node->getExp2();
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	bool isValid = true;
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(node, ErrorType::produce());
		isValid = false;
		return;
	}

	if (!(type1->isBool())) {
		ta->errLogicOpd(myExp1->pos());
		isValid = false;
	}
	if (!(type2->isBool())) {
		ta->errLogicOpd(myExp2->pos());
		isValid = false;
	}
	if (isValid) {
		ta->nodeType(node, BasicType::produce(BOOL));
	} else {
		ta->nodeType(node, ErrorType::produce());
	}
}
void AndNode::typeAnalysis(TypeAnalysis * ta) {
	logicalOpsTypeAnalysis(this, ta);
}

void OrNode::typeAnalysis(TypeAnalysis * ta) {
	logicalOpsTypeAnalysis(this, ta);
}
void NotNode::typeAnalysis(TypeAnalysis * ta) {
	myExp->typeAnalysis(ta);
	const DataType * expType = ta->nodeType(myExp);

	if (expType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!(expType->isInt())) {
		ta->errLogicOpd(myExp->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
} //1}}}

/* Arithmetic operations: Binary - PlusNode, MinusNode, TimesNode, DivideNode, Unary - NegNode, postincrement, postdecrement
-> Operands are both int - the result type is int. In all illegal cases, the result type is ERROR.
{{{1*/
void arithmeticOpsTypeAnalysis(BinaryExpNode * node, TypeAnalysis * ta) {
	ExpNode * myExp1 = node->getExp1();
	ExpNode * myExp2 = node->getExp2();
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	bool isValid = true;
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(node, ErrorType::produce());
		isValid = false;
		return;
	}

	if (!(type1->isInt())) {
		ta->errMathOpd(myExp1->pos());
		isValid = false;
	}
	if (!(type2->isInt())) {
		ta->errMathOpd(myExp2->pos());
		isValid = false;
	}
	if (isValid) {
		ta->nodeType(node, BasicType::produce(INT));
	} else {

		ta->nodeType(node, ErrorType::produce());
	}
}

void PlusNode::typeAnalysis(TypeAnalysis * ta) {
	arithmeticOpsTypeAnalysis(this, ta);
}

void MinusNode::typeAnalysis(TypeAnalysis * ta) {
	arithmeticOpsTypeAnalysis(this, ta);
}

void TimesNode::typeAnalysis(TypeAnalysis * ta) {
	arithmeticOpsTypeAnalysis(this, ta);
}

void DivideNode::typeAnalysis(TypeAnalysis * ta) {
	arithmeticOpsTypeAnalysis(this, ta);
}

void NegNode::typeAnalysis(TypeAnalysis * ta) {
	myExp->typeAnalysis(ta);
	const DataType * expType = ta->nodeType(myExp);

	if (expType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!(expType->isInt())) {
		ta->errMathOpd(myExp->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
}
void PostIncStmtNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * expType = myLoc->getSymbol()->getDataType();
	if (expType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!expType->isInt()) {
		ta->errMathOpd(myLoc->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
}
void PostDecStmtNode::typeAnalysis(TypeAnalysis * ta) {
	const DataType * expType = myLoc->getSymbol()->getDataType();
	if (expType->asError()) {
		ta->nodeType(this, ErrorType::produce());
		return;
	}
	if (!expType->isInt()) {
		ta->errMathOpd(myLoc->pos());
		ta->nodeType(this, ErrorType::produce());
		return;
	}
}

/* Relational operations: LessNode, LessEqNode, GreaterNode, GreaterEqNode
-> Both operands are int. The result type is bool in legal cases, ERROR otherwise.
*/
void relationalOpsTypeAnalysis(BinaryExpNode * node, TypeAnalysis * ta) {
	ExpNode * myExp1 = node->getExp1();
	ExpNode * myExp2 = node->getExp2();
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	bool isValid = true;
	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);

	if (type1->asError() || type2->asError()) {
		ta->nodeType(node, ErrorType::produce());
		isValid = false;
		return;
	}

	if (!(type1->isInt())) {
		ta->errRelOpd(myExp1->pos());
		isValid = false;
	}
	if (!(type2->isInt())) {
		ta->errRelOpd(myExp2->pos());
		isValid = false;
	}
	if (isValid) {
		ta->nodeType(node, BasicType::produce(BOOL));
	} else {
		ta->nodeType(node, ErrorType::produce());
	}
}
void LessNode::typeAnalysis(TypeAnalysis * ta) {
	relationalOpsTypeAnalysis(this, ta);
}
void LessEqNode::typeAnalysis(TypeAnalysis * ta) {
	relationalOpsTypeAnalysis(this, ta);
}
void GreaterNode::typeAnalysis(TypeAnalysis * ta) {
	relationalOpsTypeAnalysis(this, ta);
}
void GreaterEqNode::typeAnalysis(TypeAnalysis * ta) {
	relationalOpsTypeAnalysis(this, ta);
}


/* Equality operations: EqualsNode, NotEqualsNode
-> Both operands are of the same primitive type
Neither operands are of the same class type and Neither operand is a function type
result type is bool
*/
void equalityOpsTypeAnalysis(BinaryExpNode * node, TypeAnalysis * ta) {
	bool isValid = true;
	ExpNode * myExp1 = node->getExp1();
	ExpNode * myExp2 = node->getExp2();

	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	const DataType * type1 = ta->nodeType(myExp1);
	const DataType * type2 = ta->nodeType(myExp2);
	if (type1->asError() || type2->asError()) {
		ta->nodeType(node, ErrorType::produce());
		return;
	}
	if (!(type1->isBool() || type1->isInt())) {
		ta->errEqOpd(myExp1->pos());
		isValid = false;
	}
	if (!(type2->isBool() || type2->isInt())) {
		ta->errEqOpd(myExp2->pos());
		isValid = false;
	}
	if (isValid && (type1->getString() != type2->getString())) {
		ta->errEqOpr(node->pos());
		bool isValid = false;
	}
	if (isValid) {
		ta->nodeType(node, BasicType::produce(BOOL));
	} else {
		ta->nodeType(node, ErrorType::produce());
	}
}
void EqualsNode::typeAnalysis(TypeAnalysis * ta) {
	equalityOpsTypeAnalysis(this, ta);
}
void NotEqualsNode::typeAnalysis(TypeAnalysis * ta) {
	equalityOpsTypeAnalysis(this, ta);
} //1}}}


/*Member access operations: dereferance
The index operand is a field name of the base class type
The result type is the type of the field in legal cases, and ERROR otherwise.
*/

//Bool condition
void IfStmtNode::typeAnalysis(TypeAnalysis * ta) {
	myCond->typeAnalysis(ta);
	if (!ta->nodeType(myCond)->isBool()) {
		ta->errCond(myCond->pos());
	}
	for (StmtNode * node : *myBody) {
		node->typeAnalysis(ta);
	}
}
void IfElseStmtNode::typeAnalysis(TypeAnalysis * ta) {
	myCond->typeAnalysis(ta);
	if (!ta->nodeType(myCond)->isBool()) {
		ta->errCond(myCond->pos());
	}
	for (StmtNode * node : *myBodyTrue) {
		node->typeAnalysis(ta);
	}
		for (StmtNode * node : *myBodyFalse) {
		node->typeAnalysis(ta);
	}
}
void WhileStmtNode::typeAnalysis(TypeAnalysis * ta) {
	myCond->typeAnalysis(ta);
	if (!ta->nodeType(myCond)->isBool()) {
		ta->errCond(myCond->pos());
	}
	for (StmtNode * node : *myBody) {
		node->typeAnalysis(ta);
	}
}
}
