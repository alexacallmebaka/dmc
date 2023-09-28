#include "ast.hpp"

namespace drewno_mars{

/*
doIndent is declared static, which means that it can 
only be called in this file (its symbol is not exported).
*/
static void doIndent(std::ostream& out, int indent){
	for (int k = 0 ; k < indent; k++){ out << "\t"; }
}

/*
In this code, the intention is that functions are grouped 
into files by purpose, rather than by class.
If you're used to having all of the functions of a class 
defined in the same file, this style may be a bit disorienting,
though it is legal. Thus, we can have
ProgramNode::unparse, which is the unparse method of ProgramNodes
defined in the same file as DeclNode::unparse, the unparse method
of DeclNodes.
*/


void ProgramNode::unparse(std::ostream& out, int indent){
	/* Oh, hey it's a for-each loop in C++!
	   The loop iterates over each element in a collection
	   without that gross i++ nonsense. 
	 */
	for (auto global : *myGlobals){
		/* The auto keyword tells the compiler
		   to (try to) figure out what the
		   type of a variable should be from 
		   context. here, since we're iterating
		   over a list of DeclNode *s, it's 
		   pretty clear that global is of 
		   type DeclNode *.
		*/
		global->unparse(out, indent);
	}
}

void VarDeclNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	this->myID->unparse(out, 0);
	out << " : ";
	this->myType->unparse(out, 0);
  if (this->myVal) {
    out << " = ";
    this->myVal->unparse(out,0);
  }
	out << ";\n";
}

void IDNode::unparse(std::ostream& out, int indent){
	out << this->name;
}

void PerfectTypeNode::unparse(std::ostream& out, int indent){
	doIndent(out, indent);
	out << "perfect ";
  this->myType->unparse(out, 0);
}

void IntTypeNode::unparse(std::ostream& out, int indent){
	out << "int";
}

void BoolTypeNode::unparse(std::ostream& out, int indent){
	out << "bool";
}

void VoidTypeNode::unparse(std::ostream& out, int indent){
	out << "void";
}

void IntLitNode::unparse(std::ostream& out, int indent){
  out << this->myVal;
}

void FnDeclNode::unparse(std::ostream& out, int indent){
  this->myID->unparse(out, indent);
  out << " : (";
  size_t index = 0;
  for( auto formal : *myParams ) {
    formal->unparse(out,0);
    if ( index < this->myParams->size() - 1) {
      out << ", ";
      index++;
    }
  }
  out << ") ";
  this->myRetType->unparse(out,0);
  out << " {\n";
  for ( StmtNode * stmt : *(this->myBody) ) {
    stmt->unparse(out, indent+4);
    out << "\n";
  }
  doIndent(out, indent);
  out << "}\n";
}

void FormalDeclNode::unparse(std::ostream& out, int indent){
  this->myID->unparse(out, 0);
  out << " : ";
  this->myType->unparse(out, 0);
}

void WhileStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "while (";
  myExp->unparse(out, 0);
  out << "){\n";
  for ( StmtNode * stmt : *(this->myBody) ) {
    stmt->unparse(out, indent+4);
    out << "\n";
  }
  out << "}\n";
}

void IfStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "if (";
  myExp->unparse(out, 0);
  out << "){\n";
  for ( StmtNode * stmt : *(this->myBody) ) {
    stmt->unparse(out, indent+4);
    out << "\n";
  }
  out << "}\n";
}

} // End namespace drewno_mars
