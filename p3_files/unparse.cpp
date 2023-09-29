#include "ast.hpp"

namespace drewno_mars{

/*
doIndent is declared static, which means that it can 
only be called in this file (its symbol is not exported).
*/
static void doIndent(std::ostream& out, int indent){
	for (int k = 0 ; k < indent; k++){ out << "\t"; }
}

static void printStmtList(std::ostream& out, int indent, std::list< StmtNode * > * stmts){
  for ( StmtNode * stmt : *stmts ) {
    stmt->unparse(out, indent);
  }
}


void printBinaryExp(std::ostream& out, int indent, BinaryExpNode * expr, std::string op){
  const bool lhsParens = expr->myLhs->needsParens;
  const bool rhsParens = expr->myRhs->needsParens;
  out << (lhsParens ? "( " : "");
  expr->myLhs->unparse(out,0);
  out << (lhsParens ? " )" : "");
  out << op;
  out << (rhsParens ? "( " : "");
  expr->myRhs->unparse(out,0);
  out << (rhsParens ? " )" : "");
  
}

void printUnaryExp(std::ostream& out, int indent, UnaryExpNode * expr, std::string op){
  const bool parens = expr->myOperand->needsParens;
  out << op << (parens ? "( " : "");
  expr->myOperand->unparse(out,0);
  out << (parens ? " )" : "");
  
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
  printStmtList(out, indent+4, this->myBody);
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
  this->myExp->unparse(out, 0);
  out << "){\n";
  printStmtList(out, indent+4, this->myBody);
  doIndent(out,indent);
  out << "}\n";
}

void IfStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "if (";
  this->myExp->unparse(out, 0);
  out << "){\n";
  printStmtList(out, indent+4, this->myBody);
  doIndent(out,indent);
  out << "}\n";
}

void IfElseStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "if (";
  this->myExp->unparse(out, 0);
  out << "){\n";
  printStmtList(out, indent+4, this->myTrueBranch);
  doIndent(out,indent);
  out << "} else {\n";
  printStmtList(out, indent+4, this->myFalseBranch);
  doIndent(out,indent);
  out << "}\n";
}

void AndNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " and ");
}

void OrNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " or ");
}

void DivideNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " / ");
}

void EqualsNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " == ");
}

void GreaterEqNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " >= ");
}

void GreaterNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " > ");
}

void LessEqNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " <= ");
}

void LessNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " < ");
}

void MinusNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " - ");
}

void NotEqualsNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " != ");
}

void PlusNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " + ");
}

void TimesNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printBinaryExp(out, indent, this, " * ");
}

void NotNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printUnaryExp(out, indent, this, "!");
}

void NegNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  printUnaryExp(out, indent, this, "-");
}

void MemberFieldExpNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  this->myBaseClass->unparse(out,0);
  out << "--";
  this->myField->unparse(out,0);
}

void TrueNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "true";
}

void FalseNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "false";
}

void MagicNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "24Kmagic";
}

void StrLitNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << myStr; //myStr includes quotes already.
}

void CallExpNode::unparse(std::ostream& out, int indent){
  this->myLoc->unparse(out,indent);
  out << "(";
  size_t index = 0;
  for( auto arg : *myArgs ) {
    arg->unparse(out,0);
    if ( index < this->myArgs->size() - 1) {
      out << ", ";
      index++;
    }
  }
  out << ")";
}

void CallStmtNode::unparse(std::ostream& out, int indent){
  this->myCall->unparse(out, indent);
  out << ";\n";
}

void ExitStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "today I don't feel like doing any work;\n";
}

void ReturnStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "return ";
  if(!this->myRetVal) {
    myRetVal->unparse(out, 0);
  }
  out << ";\n";
}

void TakeStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "take ";
  this->myLoc->unparse(out, 0);
  out << ";\n";
}

void GiveStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  out << "give ";
  this->myExp->unparse(out, 0);
  out << ";\n";
}

void PostIncStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  this->myLoc->unparse(out, 0);
  out << "++";
  out << ";\n";
}

void PostDecStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  this->myLoc->unparse(out, 0);
  out << "--";
  out << ";\n";
}

void AssgnStmtNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  this->myDest->unparse(out, indent);
  out << " = ";
  this->mySrc->unparse(out, indent);
  out << ";\n";
}

void ClassTypeNode::unparse(std::ostream& out, int indent){
  this->myID->unparse(out,indent);
}

void ClassDefnNode::unparse(std::ostream& out, int indent){
  doIndent(out, indent);
  this->myID->unparse(out, 0);
  out << " : class {\n";
  for( DeclNode * decl : *myBody ) {
    decl->unparse(out, indent+4);
  }
  out << "\n};\n";
}

} // End namespace drewno_mars
