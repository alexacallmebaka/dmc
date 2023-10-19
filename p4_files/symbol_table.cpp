#include "symbol_table.hpp"
using namespace std;

namespace drewno_mars{

void VarSymbol::typeAnnotation(std::ostream& out, int indent) {
	out << "{";
	this->getType()->unparse(out, 0);
	out << "}";
}

void FnSymbol::typeAnnotation(std::ostream& out, int indent) {
	out << "{(";
	int currentIndex = 0;
	for(TypeNode* t : paramTypes) {
		t->unparse(out, 0);
		currentIndex++;
		if(currentIndex < int(paramTypes.size())) out << ",";
	}
	out << ")" << "->";
	this->getType()->unparse(out, 0);
	out << "}";
}

void ClassSymbol::typeAnnotation(std::ostream& out, int indent) {
	this->getType()->unparse(out, 0);
}

void FnSymbol::insertParams(TypeNode* t) {
	paramTypes.push_back(t);
}

ScopeTable::ScopeTable(){
	symbols = new HashMap<string, SemSymbol *>();
}

LookUpResult ScopeTable::addDecl(string id, SemSymbol * symbol) {
	bool invalidType = (symbol->getKind() == "var" && !(symbol->getType()->typeStr() == "int" || symbol->getType()->typeStr() == "bool" || symbol->getType()->typeStr() == "perfect" || symbol->getType()->typeStr() == "class"));
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
