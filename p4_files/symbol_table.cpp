#include <sstream>
#include "symbol_table.hpp"
using namespace std;

namespace drewno_mars{

string VarSymbol::typeAnnotation() {
	stringstream ss;
	ss << "{" << this->getType() << "}";
	return ss.str();
}

string FnSymbol::typeAnnotation() {
	stringstream ss;
	ss << "{(";
	int currentIndex = 0;
	for(string t : paramTypes) {
		ss << t;
		currentIndex++;
		if(currentIndex < int(paramTypes.size())) ss << ",";
	}
	ss << ")" << "->" << this->getType() << "}";
	return ss.str();
}

void FnSymbol::insertParams(string t) {
	paramTypes.push_back(t);
}

ScopeTable::ScopeTable(){
	symbols = new HashMap<string, SemSymbol *>();
}

LookUpResult ScopeTable::addDecl(string id, SemSymbol * symbol) {
	bool invalidType = (symbol->getKind() == "var" && !(symbol->getType() == "int" || symbol->getType() == "bool" || symbol->getType() == "perfect" || symbol->getType() == "class"));
	auto existingSymbol = (symbols->find(id) != symbols->end()) ? symbols->find(id)->second : nullptr;
	if (invalidType && !existingSymbol) return INVALID_TYPE;
	else if(!invalidType && existingSymbol) return MULTIPLE_DECL_ID;
	else if(invalidType && existingSymbol) return INVALID_MULTIPLE_ID;
	else {
		symbols->insert( {id, symbol} );
		return SUCCESS;
	}
}

SemSymbol * ScopeTable::scopeTableLookUp(string id) {
	auto result = symbols->find(id);
	return ((result != symbols->end()) ? result->second : nullptr);
}

SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be 
	// entered into the front of the chain and upon exit the 
	// latest scope table will be removed from the front of 
	// the chain.
	scopeTableChain = new list<ScopeTable *>();
}

//a new scope table will be entered into the front of the chain
void SymbolTable::createScopeTable() {
	ScopeTable * scope = new ScopeTable();
	scopeTableChain->push_front(scope);
}

// latest scope table will be removed from the front of the chain.
void SymbolTable::dropScopeTable() {
	if(!scopeTableChain->empty()) {
		ScopeTable * scope = scopeTableChain->front();
		scopeTableChain->pop_front();
	}
}

ScopeTable * SymbolTable::topScope() {
	if(!scopeTableChain->empty()) return scopeTableChain->front();
	return nullptr;
}

SemSymbol * SymbolTable::symbolTableLookUp(std::string id) {
	for (ScopeTable * scope : *scopeTableChain) {
		SemSymbol * symbol = scope->scopeTableLookUp(id);
		if(symbol) return symbol;
	}
	return nullptr;
}

LookUpResult SymbolTable::insert(std::string id, SemSymbol * symbol) {
	if(!scopeTableChain->empty()) {
		return topScope()->addDecl(id, symbol);
	}
	return FAIL;
}

}
