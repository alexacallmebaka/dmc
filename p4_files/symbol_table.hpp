#ifndef DREWNO_MARS_SYMBOL_TABLE_HPP
#define DREWNO_MARS_SYMBOL_TABLE_HPP
#include <string>
#include <unordered_map>
#include <list>

using namespace std;

//Use an alias template so that we can use
// "HashMap" and it means "unordered_map"
template <typename K, typename V>
using HashMap = unordered_map<K, V>;

using namespace std;

namespace drewno_mars{

//A semantic symbol, which represents a single
// variable, function, etc. Semantic symbols 
// exist for the lifetime of a scope in the 
// symbol table. 
class SemSymbol {
	// each semantic symbol should track
	// (i.e. the kind of the symbol (either a variable or function)
	// and functions to get/set those fields
	protected:
		string kind;
		string type;
	public:
		SemSymbol() {}
		SemSymbol(string k, string t) : kind(k), type(t) {}
		string getKind() const { return kind; }
		string getType() const { return type; }
		void setKind(string k) { this->kind = k; }
		void setType(string t) { this->type = t; }
		virtual string typeAnnotation() = 0;
};

class VarSymbol : public SemSymbol{
	public:
		VarSymbol(string t) : SemSymbol("var", t) {}
		std::string typeAnnotation();
};

class FnSymbol : public SemSymbol{
	private:
		std::list<string> paramTypes;
	public:
		FnSymbol(string t) : SemSymbol("fn", t) {}
		FnSymbol(string t, std::list<string>& l) : SemSymbol("fn", t), paramTypes(l) {}
		std::string typeAnnotation();
		void insertParams(string t);
};

//A single scope. The symbol table is broken down into a 
// chain of scope tables, and each scope table holds 
// semantic symbols for a single scope. For example,
// the globals scope will be represented by a ScopeTable,
// and the contents of each function can be represented by
// a ScopeTable.
enum LookUpResult {SUCCESS, FAIL, MULTIPLE_DECL_ID, INVALID_TYPE, INVALID_MULTIPLE_ID};

class ScopeTable {
	public:
		ScopeTable();
		//TODO: add functions for looking up symbols
		// and/or returning information to indicate
		// that the symbol does not exist within the
		// current scope.

		//add declaration to the scope
		LookUpResult addDecl(string id, SemSymbol * symbol);

		//returning information to indicate
		//that the symbol does not exist within the
		//current scope
		SemSymbol * scopeTableLookUp(string id);
	private:
		HashMap<string, SemSymbol *> * symbols;
};

class SymbolTable{
	public:
		SymbolTable();
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc.

		//create a new ScopeTable when a new scope is entered
		void createScopeTable();

		//drop a ScopeTable when a scope is exited
		void dropScopeTable();

		ScopeTable * topScope();

		// Takes an id name and attempts to find that id in the symbol table
		// returns the symbol that should be pointed to
		// returns nullptr if not found
		SemSymbol * symbolTableLookUp(std::string id);
	
		// wrapper around the ScopeTable implementation
		// adds pair to the scope at the front (top) of stack
		LookUpResult insert(std::string id, SemSymbol * symbol);

	private:
		list<ScopeTable *> * scopeTableChain;
};

	
}

#endif
