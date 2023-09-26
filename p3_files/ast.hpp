#ifndef DREWNO_MARS_AST_HPP
#define DREWNO_MARS_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"
#include <cassert>


/* You'll probably want to add a bunch of ASTNode subclasses */

namespace drewno_mars{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/

//subclasses of ASTNode {{{1
class ExpNode;
class ProgramNode;
class StmtNode;
class TypeNode;
//1}}}

//subclasses of ExpNode {{{2
class BinaryExpNode;
class CallExpNode;
class FalseNode;
class IntLitNode;
class LocNode;
class StrLitNode;
class TrueNode;
class UnaryExpNode;
//2}}}

//subclasses of StmtNode {{{1
class AssignStmtNode;
class CallStmtNode;
class DeclNode;
class ExitStmtNode;
class GiveStmtNode;
class IfElseStmtNode;
class IfStmtNode;
class PostDecStmtNode;
class PostIncStmtNode;
class ReturnStmtNode;
class TakeStmtNode;
class WhileStmtNode;
//1}}}

//subclasses of TypeNode {{{1
class BoolTypeNode;
class ClassTypeNode;
class IntTypeNode;
class PerfectTypeNode;
class VoidTypeNode;
//1}}}

//subclasses of BinaryExpNode {{{1
class AndNode;
class DivideNode;
class EqualsNode;
class GreaterEqNode;
class GreaterNode;
class LessEqNode;
class LessNode;
class MinusNode;
class NotEqualsNode;
class OrNode;
class PlusNode;
class TimesNode;
//1}}}

//subclasses of LocNode {{{1
class IDNode;
class MemberFieldExpNode;
//1}}}

//subclasses of UnaryExpNode {{{1
class NegNode;
class NotNode;
//1}}}

//subclasses of DeclNode {{{1
class ClassDefnNode;
class FnDeclNode;
class VarDeclNode;
//1}}}

//subclasses of VarDeclNode {{{1
class FormalDeclNode;
//1}}}

/** 
* \class ASTNode
* Base class for all other AST Node types
**/
class ASTNode{
public:
	ASTNode(const Position * p) : myPos(p){ }
	virtual void unparse(std::ostream& out, int indent) = 0;
	const Position * pos() { return myPos; }
	std::string posStr() { return pos()->span(); }
protected:
	const Position * myPos = nullptr;
};

/** 
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn) ;
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<DeclNode * > * myGlobals;
};

class StmtNode : public ASTNode{
public:
	StmtNode(const Position * p) : ASTNode(p){ }
	void unparse(std::ostream& out, int indent) override = 0;
};


/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to 
* declare a struct, function, variable, etc).  This base class will 
**/
class DeclNode : public StmtNode{
public:
	DeclNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

/**  \class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
protected:
	ExpNode(const Position * p) : ASTNode(p){ }
};

/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in 
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(const Position * p) : ASTNode(p){
	}
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};


/** A memory location. LocNodes subclass ExpNode
 * because they can be used as part of an expression. 
**/
class LocNode : public ExpNode{
public:
	LocNode(const Position * p)
	: ExpNode(p) {}
	void unparse(std::ostream& out, int indent) = 0;
};

/** An identifier. Note that IDNodes subclass
 * LocNode because they are a type of memory location. 
**/
class IDNode : public LocNode{
public:
	IDNode(const Position * p, std::string nameIn) 
	: LocNode(p), name(nameIn){ }
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string name;
};

/** A variable declaration.
**/
class VarDeclNode : public DeclNode{
public:
	VarDeclNode(const Position * p, IDNode * inID, TypeNode * inType) 
	: DeclNode(p), myID(inID), myType(inType){
		assert (myType != nullptr);
		assert (myID != nullptr);
	}
  VarDeclNode(const Position * p, IDNode * inID, TypeNode * inType, ExpNode * inVal) 
  : DeclNode(p), myID(inID), myType(inType), myVal(inVal){
		assert (myType != nullptr);
		assert (myID != nullptr);
		assert (myVal != nullptr);
  }
	void unparse(std::ostream& out, int indent);
private:
	IDNode * myID;
	TypeNode * myType;
  ExpNode * myVal; //unassigned vars will initialize to null.
};

class PerfectTypeNode : public TypeNode{
  public:
    PerfectTypeNode(const Position *p, TypeNode * inType) : TypeNode(p), myType(inType){ }
    void unparse(std::ostream& out, int indent);
   private:
    TypeNode * myType;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};


class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class ClassTypeNode : public TypeNode{
public:
	ClassTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class IntLitNode : public ExpNode{
public:
  IntLitNode(const Position *p, const int inVal) : ExpNode(p), myVal(inVal) { }
	void unparse(std::ostream& out, int indent);
private:
  int myVal;
};

} //End namespace drewno_mars

#endif
