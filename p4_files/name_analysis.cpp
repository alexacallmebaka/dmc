#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "errName.hpp"

namespace drewno_mars{

static bool bodyNameAnalysis(std::list<StmtNode *> * stmtList, SymbolTable * symTab) {
	bool bodyNameAnalysisOk = true;
	for (auto node : *stmtList) {
		bodyNameAnalysisOk = node->nameAnalysis(symTab) && bodyNameAnalysisOk;
	}
	return bodyNameAnalysisOk;
}

//TODO here is a subset of the nodes needed to do nameAnalysis, 
// you should add the rest to allow for a complete treatment
// of any AST

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

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	VarSymbol * newSymbol = new VarSymbol(this->getTypeNode()->typeStr());
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
			NameErr::badVarType(pos());
			nameAnalysisOk = false;
			break;
		}
		case MULTIPLE_DECL_ID: {
			NameErr::multiDecl(pos());
			nameAnalysisOk = false;
			break;
		}
		case INVALID_MULTIPLE_ID: {
			NameErr::multiDecl(pos());
			NameErr::badVarType(pos());
			nameAnalysisOk = false;
			break;
		}
		default: break;
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	FnSymbol * newSymbol = new FnSymbol(this->getTypeNode()->typeStr());
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
			NameErr::badVarType(pos());
			nameAnalysisOk = false;
			break;
		}
		case MULTIPLE_DECL_ID: {
			NameErr::multiDecl(pos());
			nameAnalysisOk = false;
			break;
		}
		case INVALID_MULTIPLE_ID: {
			NameErr::multiDecl(pos());
			NameErr::badVarType(pos());
			nameAnalysisOk = false;
			break;
		}
		default: break;
	}

	//Create a new scope context at top of stack
	symTab->createScopeTable();
	for (FormalDeclNode * formal : *getFormals()){
		nameAnalysisOk = formal->nameAnalysis(symTab) && nameAnalysisOk;
		newSymbol->insertParams(formal->getTypeNode()->typeStr());
	}
	// nameAnalysisOk = bodyNameAnalysis(myBody, symTab);
	symTab->dropScopeTable();

	return nameAnalysisOk;
}

bool IntTypeNode::nameAnalysis(SymbolTable* symTab){
	//Name analysis may never even recurse down to IntTypeNode,
	//but if it does, just return true to indicate that 
	//name analysis has not failed, and add nothing to the symbol table
	return true;
}

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

bool IfStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCond->nameAnalysis(symTab) && nameAnalysisOk;
	symTab->createScopeTable();
	nameAnalysisOk = bodyNameAnalysis(myBody, symTab) && nameAnalysisOk;
	symTab->dropScopeTable();
	return nameAnalysisOk;
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

bool WhileStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCond->nameAnalysis(symTab) && nameAnalysisOk;

	symTab->createScopeTable();
	nameAnalysisOk = bodyNameAnalysis(myBody, symTab) && nameAnalysisOk;
	symTab->dropScopeTable();
	return nameAnalysisOk;
}

}
