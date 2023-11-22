#include "ast.hpp"

namespace drewno_mars{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta){
	IRProgram * prog = new IRProgram(ta);

	// prog->toString(true);
	for (auto global : *myGlobals){
		global->to3AC(prog);
	}
	return prog;
}

void FnDeclNode::to3AC(IRProgram * prog){
	SemSymbol * symbol = myID->getSymbol();
	Procedure * proc = prog->makeProc(symbol->getName());

	prog->gatherGlobal(symbol);
	for (auto formal : *myFormals){
		formal->to3AC(proc);
	}
	size_t indexIn = 1;
	for (auto formal : *myFormals){
		SemSymbol * sym = formal->ID()->getSymbol();
		SymOpd * opd = proc->getSymOpd(sym);
		Quad * inQuad = new GetArgQuad(indexIn, opd);
		proc->addQuad(inQuad);
		indexIn++;
	}
	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}
}

void FnDeclNode::to3AC(Procedure * proc){
	//This never needs to be implemented,
	// the function only exists because of
	// inheritance needs (A function declaration
	// never occurs within another function)
	throw new InternalError("FnDecl at a local scope");
}

void FormalDeclNode::to3AC(IRProgram * prog){
	//This never needs to be implemented,
	// the function only exists because of
	// inheritance needs (A formal never
	// occurs at global scope)
	throw new InternalError("Formal at a global scope");
}

void FormalDeclNode::to3AC(Procedure * proc){
	SemSymbol * symbol = ID()->getSymbol();
	proc->gatherFormal(symbol);
}

Opd * IntLitNode::flatten(Procedure * proc){
	const DataType * type = proc->getProg()->nodeType(this);
	return new LitOpd(std::to_string(myNum), 8);
}

Opd * StrLitNode::flatten(Procedure * proc){
	Opd * res = proc->getProg()->makeString(myStr);
	return res;
}

Opd * TrueNode::flatten(Procedure * proc){
	Opd * res = new LitOpd("1", 8);
	return res;
}

Opd * FalseNode::flatten(Procedure * proc){
	Opd * res = new LitOpd("0", 8);
	return res;
}

Opd * CallExpNode::flatten(Procedure * proc){
	std::list<Opd *> argOpds;
	for (auto node : *myArgs){
		Opd * argOpd = node->flatten(proc);
		const DataType * argType = proc->getProg()->nodeType(node);
		argOpds.push_back(argOpd);
	}
	size_t indexIn = 1;
	for (auto argOpd : argOpds){
		Quad * quad = new SetArgQuad(indexIn, argOpd);
		proc->addQuad(quad);
		indexIn++;
	}

	SemSymbol * idSymbol = myCallee->getSymbol();
	Quad * callQuad = new CallQuad(idSymbol);
	proc->addQuad(callQuad);
	const FnType * calleeType = idSymbol->getDataType()->asFn();
	const DataType * retType = calleeType->getReturnType();
	if (retType->isVoid()) return nullptr;
	Opd * retValOpd = proc->makeTmp(Opd::width(retType));
	Quad * getRetQuad = new GetRetQuad(retValOpd);
	proc->addQuad(getRetQuad);
	return retValOpd;
}

Opd * NegNode::flatten(Procedure * proc){
	Opd * childOpd = myExp->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new UnaryOpQuad(tmp, UnaryOp::NEG64, childOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * NotNode::flatten(Procedure * proc){
	Opd * childOpd = myExp->flatten(proc);
	size_t width = proc->getProg()->opWidth(myExp);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new UnaryOpQuad(tmp, UnaryOp::NOT64, childOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * PlusNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::ADD64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * MinusNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::SUB64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * TimesNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::MULT64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * DivideNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::DIV64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * AndNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::AND64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * OrNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::OR64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * EqualsNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::EQ64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::NEQ64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * LessNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::LT64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * GreaterNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::GT64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * LessEqNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::LTE64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	Opd * lhsOpd = myExp1->flatten(proc);
	Opd * rhsOpd = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * tmp = proc->makeTmp(width);
	Quad * quad = new BinOpQuad(tmp, BinOp::GTE64, lhsOpd, rhsOpd);
	proc->addQuad(quad);
	return tmp;
}

void AssignStmtNode::to3AC(Procedure * proc){
	Opd * srcOpd = mySrc->flatten(proc);
	Opd * dstOpd = myDst->flatten(proc);
	Quad * quad = new AssignQuad(dstOpd, srcOpd);
	proc->addQuad(quad);
}

void PostIncStmtNode::to3AC(Procedure * proc){
	Opd * childOpd = myLoc->flatten(proc);
	size_t width = proc->getProg()->opWidth(myLoc);
	LitOpd * litOpd = new LitOpd("1", width);
	Quad * quad = new BinOpQuad(childOpd, BinOp::ADD64, childOpd, litOpd);
	proc->addQuad(quad);
}

void PostDecStmtNode::to3AC(Procedure * proc){
	Opd * childOpd = myLoc->flatten(proc);
	size_t width = proc->getProg()->opWidth(myLoc);
	LitOpd * litOpd = new LitOpd("1", width);
	Quad * quad = new BinOpQuad(childOpd, BinOp::SUB64, childOpd, litOpd);
	proc->addQuad(quad);
}

void GiveStmtNode::to3AC(Procedure * proc){
	Opd * childOpd = this->mySrc->flatten(proc);
	const DataType * type = proc->getProg()->nodeType(mySrc);
	Quad * quad = new WriteQuad(childOpd, type);
	proc->addQuad(quad);
}

void TakeStmtNode::to3AC(Procedure * proc){
	Opd * childOpd = this->myDst->flatten(proc);
	const DataType * type = proc->getProg()->nodeType(myDst);
	Quad * quad = new WriteQuad(childOpd, type);
	proc->addQuad(quad);
}

void IfStmtNode::to3AC(Procedure * proc){
	Opd * condOpd = myCond->flatten(proc);

	//after the if scope
	Label * afterLabel = proc->makeLabel();
	Quad * afterQuad = new NopQuad();
	afterQuad->addLabel(afterLabel);

	Quad * ifQuad = new IfzQuad(condOpd, afterLabel);
	proc->addQuad(ifQuad);


	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}

	proc->addQuad(afterQuad);
}

void IfElseStmtNode::to3AC(Procedure * proc){
	Opd * cond = myCond->flatten(proc);

	//else branch
	Label * elseLabel = proc->makeLabel();
	Quad * elseNopQuad = new NopQuad();
	elseNopQuad->addLabel(elseLabel);

	//after if-else
	Label * afterLabel = proc->makeLabel();
	Quad * afterQuad = new NopQuad();
	afterQuad->addLabel(afterLabel);


	Quad * jmp = new IfzQuad(cond, elseLabel);
	proc->addQuad(jmp);

	for (auto stmt : *myBodyTrue){
		stmt->to3AC(proc);
	}
	
	Quad * jmpFalse = new GotoQuad(afterLabel);
	proc->addQuad(jmpFalse);
	proc->addQuad(elseNopQuad);
	
	for (auto stmt : *myBodyFalse){
		stmt->to3AC(proc);
	}
	proc->addQuad(afterQuad);
}

void WhileStmtNode::to3AC(Procedure * proc){

	Label * headLabel = proc->makeLabel();
	Quad * headQuad = new NopQuad();
	headQuad->addLabel(headLabel);

	Label * afterLabel = proc->makeLabel();
	Quad * afterQuad = new NopQuad();
	afterQuad->addLabel(afterLabel);

	proc->addQuad(headQuad);
	Opd * cond = myCond->flatten(proc);
	Quad * jmpQuad = new IfzQuad(cond, afterLabel);
	proc->addQuad(jmpQuad);

	//loop through all nodes in body
	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}

	//create a loop
	Quad * jmpBackQuad = new GotoQuad(headLabel);
	proc->addQuad(jmpBackQuad);

	proc->addQuad(afterQuad);
}

void CallStmtNode::to3AC(Procedure * proc){
	Opd * res = myCallExp->flatten(proc);
	if (res){
		Quad * last = proc->popQuad();
	}
}

void ReturnStmtNode::to3AC(Procedure * proc){
	if (myExp){
		Opd * res = myExp->flatten(proc);
		const DataType * type = proc->getProg()->nodeType(myExp);
		Quad * quad = new SetRetQuad(res);
		proc->addQuad(quad);
	}
	
	Label * leaveLabel = proc->getLeaveLabel();
	Quad * jmpQuad = new GotoQuad(leaveLabel);
	proc->addQuad(jmpQuad);
}

void ExitStmtNode::to3AC(Procedure * proc){
	Quad * quad = new LeaveQuad(proc);
	proc->addQuad(quad);
}

void VarDeclNode::to3AC(Procedure * proc){
	SemSymbol * symbol = myID->getSymbol();
	assert(symbol != nullptr);
	proc->gatherLocal(symbol);
  if ( myInit == nullptr ) {
    return;
  }
  Opd * srcOpd = myInit->flatten(proc);
  Opd * dstOpd = myID->flatten(proc);
  Quad * quad = new AssignQuad(dstOpd, srcOpd);
  proc->addQuad(quad);

}

void VarDeclNode::to3AC(IRProgram * prog){
	SemSymbol * symbol = myID->getSymbol();
	assert(symbol != nullptr);
	// Procedure * proc = prog->makeProc(symbol->getName());
	prog->gatherGlobal(symbol);

}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent)
Opd * IDNode::flatten(Procedure * proc){
	SemSymbol * symbol = this->getSymbol();
	Opd * res = proc->getSymOpd(symbol);
	assert(res != nullptr);
	return res;
}

}
