#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "errName.hpp"

namespace drewno_mars{

static bool bodyNameAnalysis(std::list<StmtNode *> * stmtList, SymbolTable * symTab) {
	bool bodyNameAnalysisOk = true;
	for (StmtNode * node : *stmtList) {
		bodyNameAnalysisOk = node->nameAnalysis(symTab) && bodyNameAnalysisOk;
	}
	return bodyNameAnalysisOk;
}

//subclasses of ASTNode {{{1
bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	symTab->createScopeTable();
	bool res = true;
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	symTab->dropScopeTable();
	return res;
}
//1}}}

//subclasses of ExpNode {{{2
bool ExpNode::nameAnalysis(SymbolTable * symTab) {
	return true;
}
bool BinaryExpNode::nameAnalysis(SymbolTable * symTab) {
	return (myExp1->nameAnalysis(symTab) && myExp2->nameAnalysis(symTab));
}
bool CallExpNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCallee->nameAnalysis(symTab) && nameAnalysisOk;

	for (ExpNode * expNode : * myArgs){
		nameAnalysisOk = expNode->nameAnalysis(symTab) && nameAnalysisOk;
	}
	return nameAnalysisOk;
}
bool UnaryExpNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}
//2}}}

//subclasses of StmtNode {{{1
bool AssignStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myDst->nameAnalysis(symTab) && mySrc->nameAnalysis(symTab);
}
bool CallStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myCallExp->nameAnalysis(symTab);
}
bool ExitStmtNode::nameAnalysis(SymbolTable * symTab) {
	return true;
}
bool GiveStmtNode::nameAnalysis(SymbolTable * symTab) {
	return mySrc->nameAnalysis(symTab);
}
bool IfElseStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCond->nameAnalysis(symTab) && nameAnalysisOk;

	//name analysis on true branch
	symTab->createScopeTable();
	nameAnalysisOk = bodyNameAnalysis(myBodyTrue, symTab) && nameAnalysisOk;
	symTab->dropScopeTable();

	//name analysis on false branch
	symTab->createScopeTable();
	nameAnalysisOk = bodyNameAnalysis(myBodyFalse, symTab) && nameAnalysisOk;
	symTab->dropScopeTable();

	return nameAnalysisOk;
}
bool IfStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCond->nameAnalysis(symTab) && nameAnalysisOk;
	symTab->createScopeTable();
	nameAnalysisOk = bodyNameAnalysis(myBody, symTab) && nameAnalysisOk;
	symTab->dropScopeTable();
	return nameAnalysisOk;
}
bool PostDecStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myLoc->nameAnalysis(symTab);
}
bool PostIncStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myLoc->nameAnalysis(symTab);
}
bool ReturnStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}
bool TakeStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myDst->nameAnalysis(symTab);
}
bool WhileStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCond->nameAnalysis(symTab) && nameAnalysisOk;

	symTab->createScopeTable();
	nameAnalysisOk = bodyNameAnalysis(myBody, symTab) && nameAnalysisOk;
	symTab->dropScopeTable();
	return nameAnalysisOk;
}
//1}}}

//subclasses of TypeNode {{{1
bool IntTypeNode::nameAnalysis(SymbolTable* symTab){
	//Name analysis may never even recurse down to IntTypeNode,
	//but if it does, just return true to indicate that 
	//name analysis has not failed, and add nothing to the symbol table
	return true;
}
bool BoolTypeNode::nameAnalysis(SymbolTable* symTab){
	return true;
}
bool ClassTypeNode::nameAnalysis(SymbolTable* symTab){
	return myID->nameAnalysis(symTab);
}
bool PerfectTypeNode::nameAnalysis(SymbolTable* symTab){
	return mySub->nameAnalysis(symTab);
}
bool VoidTypeNode::nameAnalysis(SymbolTable* symTab){
	return true;
}
//1}}}

//subclasses of LocNode {{{1
bool IDNode::nameAnalysis(SymbolTable * symTab) {
	/*When an IDNode is discovered as part of a use or definition,
	check if that use or definition corresponds to a valid symbol and,
	if appropriate, builds a link between the IDNode and the symbol */
	SemSymbol * symbol = symTab->symbolTableLookUp(this->getName());
	if(symbol) {
		attachSymbol(symbol);
		return true;
	}
	Report::fatal(pos(), "Undeclared identifier");
	return false;
}
bool MemberFieldExpNode::nameAnalysis(SymbolTable * symTab) {
	return myBase->nameAnalysis(symTab) && myField->nameAnalysis(symTab);
}
//1}}}

//subclasses of DeclNode {{{1
bool DeclNode::nameAnalysis(SymbolTable * symTab) {
	return true;
}
bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	VarSymbol * newSymbol = new VarSymbol(this->getTypeNode());
	LookUpResult result = symTab->insert(myID->getName(), newSymbol);
	switch (result) {
		case SUCCESS: {
			myID->attachSymbol(newSymbol);
			break;
		}
		case FAIL: {
			nameAnalysisOk = false;
			break;
		}
		case INVALID_TYPE: {
			NameErr::badVarType(myID->pos());
			nameAnalysisOk = false;
			break;
		}
		case MULTIPLE_DECL_ID: {
			NameErr::multiDecl(myID->pos());
			nameAnalysisOk = false;
			break;
		}
		case INVALID_MULTIPLE_ID: {
			NameErr::badVarType(myID->pos());
			NameErr::multiDecl(myID->pos());
			nameAnalysisOk = false;
			break;
		}
		default: break;
	}
	return nameAnalysisOk;
}
bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	FnSymbol * newSymbol = new FnSymbol(this->getTypeNode());
	LookUpResult result = symTab->insert(myID->getName(), newSymbol);
	switch (result) {
		case SUCCESS: {
			myID->attachSymbol(newSymbol);
			break;
		}
		case FAIL: {
			nameAnalysisOk = false;
			break;
		}
		case INVALID_TYPE: {
			NameErr::badVarType(myID->pos());
			nameAnalysisOk = false;
			break;
		}
		case MULTIPLE_DECL_ID: {
			NameErr::multiDecl(myID->pos());
			nameAnalysisOk = false;
			break;
		}
		case INVALID_MULTIPLE_ID: {
			NameErr::badVarType(myID->pos());
			NameErr::multiDecl(myID->pos());
			nameAnalysisOk = false;
			break;
		}
		default: break;
	}
	symTab->createScopeTable();
	for (FormalDeclNode * formal : *getFormals()){
		nameAnalysisOk = formal->nameAnalysis(symTab) && nameAnalysisOk;
		newSymbol->insertParams(formal->getTypeNode());
	}
	nameAnalysisOk = bodyNameAnalysis(myBody, symTab);
	symTab->dropScopeTable();

	return nameAnalysisOk;
}
bool ClassDefnNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	// nameAnalysisOk = myID->nameAnalysis(symTab) && nameAnalysisOk;
	ClassSymbol * newSymbol = new ClassSymbol();
	newSymbol->setName(myID->getName());

	LookUpResult result = symTab->insert(myID->getName(), newSymbol);
	switch (result) {
		case SUCCESS: {
			myID->attachSymbol(newSymbol);
			break;
		}
		case FAIL: {
			nameAnalysisOk = false;
			break;
		}
		case INVALID_TYPE: {
			NameErr::badVarType(myID->pos());
			nameAnalysisOk = false;
			break;
		}
		case MULTIPLE_DECL_ID: {
			NameErr::multiDecl(myID->pos());
			nameAnalysisOk = false;
			break;
		}
		case INVALID_MULTIPLE_ID: {
			NameErr::badVarType(myID->pos());
			NameErr::multiDecl(myID->pos());
			nameAnalysisOk = false;
			break;
		}
	}
	symTab->createScopeTable();
	for (DeclNode * decl : * myMembers) {
		nameAnalysisOk = decl->nameAnalysis(symTab) && nameAnalysisOk;
	}
	symTab->dropScopeTable();
	return nameAnalysisOk;
}
//1}}}
}
