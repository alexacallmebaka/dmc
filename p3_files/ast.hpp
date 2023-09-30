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
	ExpNode(const Position * p, const bool inParens) : ASTNode(p), needsParens(inParens){ }
public:
  const bool needsParens;
	void unparse(std::ostream& out, int indent) = 0;
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
	: ExpNode(p,false) {}
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

class MemberFieldExpNode : public LocNode{
public:
  MemberFieldExpNode(const Position * p, LocNode * inBaseClass, IDNode * inField)
  : LocNode(p), myBaseClass(inBaseClass), myField(inField) {
	assert (myBaseClass != nullptr);
	assert (myField != nullptr);
  }
	void unparse(std::ostream& out, int indent);
private:
  LocNode * myBaseClass;
  IDNode * myField;
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
protected:
	IDNode * myID;
	TypeNode * myType;
  ExpNode * myVal; //unassigned vars will initialize to null.
};

class FnDeclNode : public DeclNode{
  public:
    FnDeclNode(const Position *p, TypeNode * inRetType, IDNode * inID, std::list< FormalDeclNode *> * inParams, std::list< StmtNode *> * inBody)
    : DeclNode(p), myRetType(inRetType), myID(inID), myParams(inParams), myBody(inBody) {
		assert (myRetType != nullptr);
		assert (myID != nullptr);
		assert (myParams != nullptr);
		assert (myBody != nullptr);
    }
    void unparse(std::ostream& out, int indent);
 
  private:
      TypeNode * myRetType;
      IDNode * myID;
      std::list< FormalDeclNode * > * myParams;
      std::list< StmtNode * > * myBody;
};

class FormalDeclNode : public VarDeclNode {
  public:
    FormalDeclNode(const Position * p, TypeNode * inType, IDNode * inID) : VarDeclNode(p, inID, inType) { }
    void unparse(std::ostream& out, int indent);
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

class WhileStmtNode : public StmtNode{
public:
  WhileStmtNode(const Position *p, ExpNode * inExp, std::list< StmtNode * > * inBody)
    : StmtNode(p), myExp(inExp), myBody(inBody) {
		assert (myExp != nullptr);
		assert (myBody != nullptr);
    }
	void unparse(std::ostream& out, int indent);
private:
  ExpNode * myExp;
  std::list< StmtNode * > * myBody;
};

class IfStmtNode : public StmtNode {
public:
  IfStmtNode(const Position *p, ExpNode * inExp, std::list< StmtNode * > * inBody)
    : StmtNode(p), myExp(inExp), myBody(inBody) {
		assert (myExp != nullptr);
		assert (myBody != nullptr);
    }
	void unparse(std::ostream& out, int indent);
private:
  ExpNode * myExp;
  std::list< StmtNode * > * myBody;
};

class IfElseStmtNode : public StmtNode {
public:
  IfElseStmtNode(const Position *p, ExpNode * inExp, std::list< StmtNode * > * inTrueBranch, std::list< StmtNode * > * inFalseBranch)
    : StmtNode(p), myExp(inExp), myTrueBranch(inTrueBranch), myFalseBranch(inFalseBranch) {
		assert (myExp != nullptr);
		assert (myTrueBranch != nullptr);
		assert (myFalseBranch != nullptr);
    }
	void unparse(std::ostream& out, int indent);
private:
  ExpNode * myExp;
  std::list< StmtNode * > * myTrueBranch;
  std::list< StmtNode * > * myFalseBranch;
};

class IntLitNode : public ExpNode{
public:
  IntLitNode(const Position *p, const int inVal) : ExpNode(p, false), myVal(inVal) { }
	void unparse(std::ostream& out, int indent);
private:
  int myVal;
};

class BinaryExpNode : public ExpNode{
protected:
  BinaryExpNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs)
  : ExpNode(p, true), myLhs(inLhs), myRhs(inRhs) {
		assert (myRhs != nullptr);
		assert (myLhs != nullptr);
  }
public:
  virtual void unparse(std::ostream& out, int indent) = 0;
  ExpNode * myLhs;
  ExpNode * myRhs;
};

class UnaryExpNode : public ExpNode {
  protected:
    UnaryExpNode(const Position * p, ExpNode * inOperand) : ExpNode(p, true), myOperand(inOperand) { 
		assert (myOperand != nullptr);
    }
  public:
    virtual void unparse(std::ostream& out, int indent) = 0;
    ExpNode * myOperand;
};

class NotNode : public UnaryExpNode {
  public:
    NotNode(const Position * p, ExpNode * inOperand) : UnaryExpNode(p,inOperand) { }
    void unparse(std::ostream& out, int indent);
};

class NegNode : public UnaryExpNode {
  public:
    NegNode(const Position * p, ExpNode * inOperand) : UnaryExpNode(p,inOperand) { }
    void unparse(std::ostream& out, int indent);
};

class AndNode : public BinaryExpNode {
public:
  AndNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class OrNode : public BinaryExpNode {
public:
  OrNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class DivideNode : public BinaryExpNode {
public:
  DivideNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class EqualsNode : public BinaryExpNode {
public:
  EqualsNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class GreaterEqNode : public BinaryExpNode {
public:
  GreaterEqNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class GreaterNode : public BinaryExpNode {
public:
  GreaterNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class LessEqNode : public BinaryExpNode {
public:
  LessEqNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class LessNode : public BinaryExpNode {
public:
  LessNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class MinusNode : public BinaryExpNode {
public:
  MinusNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class NotEqualsNode : public BinaryExpNode {
public:
  NotEqualsNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class PlusNode : public BinaryExpNode {
public:
  PlusNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class TimesNode : public BinaryExpNode {
public:
  TimesNode(const Position * p, ExpNode * inLhs, ExpNode * inRhs) : BinaryExpNode(p, inLhs, inRhs) { }
	void unparse(std::ostream& out, int indent);
};

class TrueNode : public ExpNode {
public:
  TrueNode(const Position * p) : ExpNode(p, false) { }
	void unparse(std::ostream& out, int indent);
};

class FalseNode : public ExpNode {
public:
  FalseNode(const Position * p) : ExpNode(p, false) { }
	void unparse(std::ostream& out, int indent);
};

class MagicNode : public ExpNode {
public:
  MagicNode(const Position * p) : ExpNode(p, false) { }
	void unparse(std::ostream& out, int indent);
};

class StrLitNode : public ExpNode {
public:
  StrLitNode(const Position * p, std::string inStr) : ExpNode(p, false), myStr(inStr) { }
	void unparse(std::ostream& out, int indent);
private:
  std::string myStr;
};

class CallExpNode : public ExpNode {
public:
  CallExpNode (const Position * p, std::list< ExpNode * > * inArgs, LocNode * inLoc)
    : ExpNode(p, false), myArgs(inArgs), myLoc(inLoc) {
		assert (myArgs != nullptr);
    }
	void unparse(std::ostream& out, int indent);
private:
  std::list< ExpNode * > * myArgs;
  LocNode * myLoc;
};

class CallStmtNode : public StmtNode {
public:
  CallStmtNode(const Position * p, ExpNode * inCall) : StmtNode(p), myCall(inCall) { }
	void unparse(std::ostream& out, int indent);
private:
  ExpNode * myCall;
};

class ExitStmtNode : public StmtNode {
public:
  ExitStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent);
};

class ReturnStmtNode : public StmtNode {
public:
  ReturnStmtNode(const Position * p, ExpNode * inRetVal) : StmtNode(p), myRetVal(inRetVal) { }
	void unparse(std::ostream& out, int indent);
private:
  ExpNode * myRetVal;
};

class TakeStmtNode : public StmtNode {
public:
  TakeStmtNode(const Position * p, LocNode * inLoc) : StmtNode(p), myLoc(inLoc) { }
	void unparse(std::ostream& out, int indent);
private:
  LocNode * myLoc;
};

class GiveStmtNode : public StmtNode {
public:
  GiveStmtNode(const Position * p, ExpNode * inExp) : StmtNode(p), myExp(inExp) { }
	void unparse(std::ostream& out, int indent);
private:
  ExpNode * myExp;
};

class PostIncStmtNode : public StmtNode {
public:
  PostIncStmtNode(const Position * p, LocNode * inLoc) : StmtNode(p), myLoc(inLoc) { }
	void unparse(std::ostream& out, int indent);
private:
  LocNode * myLoc;
};

class PostDecStmtNode : public StmtNode {
public:
  PostDecStmtNode(const Position * p, LocNode * inLoc) : StmtNode(p), myLoc(inLoc) { }
	void unparse(std::ostream& out, int indent);
private:
  LocNode * myLoc;
};

class AssgnStmtNode : public StmtNode {
public:
  AssgnStmtNode(const Position * p, LocNode * inSrc, ExpNode * inDest)
    : StmtNode(p), mySrc(inSrc), myDest(inDest) {
		assert (mySrc != nullptr);
		assert (myDest != nullptr);
    }
	void unparse(std::ostream& out, int indent);
private:
  LocNode * mySrc;
  ExpNode * myDest;
};

class ClassTypeNode : public TypeNode {
public:
  ClassTypeNode(const Position * p, IDNode * inID) : TypeNode(p), myID(inID) {
		assert (myID != nullptr);
  }
	void unparse(std::ostream& out, int indent);
private:
  IDNode * myID;
};

class ClassDefnNode : public DeclNode {
public:
  ClassDefnNode(const Position * p, IDNode * inID, std::list< DeclNode * > * inBody ) 
  : DeclNode(p), myID(inID), myBody(inBody) {
		assert (myID != nullptr);
		assert (myBody != nullptr);
  }
	void unparse(std::ostream& out, int indent);

private:
  IDNode * myID;
  std::list< DeclNode * > * myBody;
};

} //End namespace drewno_mars

#endif
