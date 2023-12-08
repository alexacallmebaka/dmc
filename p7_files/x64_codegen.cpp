#include <ostream>
#include "3ac.hpp"

namespace drewno_mars{

void IRProgram::allocGlobals(){
	//Choose a label for each global
	for (auto member : globals) {
		SemSymbol* semSymbol = member.first;
		SymOpd* symOpd = member.second;
		std::string label = "gbl_" + semSymbol->getName();
		symOpd->setMemoryLoc(label);
	}
}

void IRProgram::datagenX64(std::ostream& out){
	// TODO(Write out data section)
	out << ".data\n";
	for (auto member : globals) {
		if (member.first->getName() == "give") { continue; }
		SymOpd* symOpd = member.second;
		out << symOpd->getMemoryLoc() << ": .quad 0\n";
	}

	for (auto member : strings) {
		LitOpd* strLabel = member.first;
		out << strLabel->valString() + ": .asciz " << member.second << "\n";
	}
	//Put this directive after you write out strings
	// so that everything is aligned to a quadword value
	// again
	out << ".align 8\n";

}

void IRProgram::toX64(std::ostream& out){
	allocGlobals();
	datagenX64(out);
	// Iterate over each procedure and codegen it
	out << ".globl main\n" << ".text\n";
	for (auto proc : *procs) {
		proc->toX64(out);
	}
}

void Procedure::allocLocals(){
	//Allocate space for locals
	// Iterate over each procedure and codegen it
	int offset = -24;
	for (auto member : locals) {
		SymOpd* symOpd = member.second;
		std::string memLoc = std::to_string(offset) + "(%rbp)";
		symOpd->setMemoryLoc(memLoc);
		offset -= int(symOpd->getWidth());
	}
	for (auto temp : temps) {
		AuxOpd* auxOpd = temp;
		std::string memLoc = std::to_string(offset) + "(%rbp)";
		auxOpd->setMemoryLoc(memLoc);
		offset -= int(auxOpd->getWidth());
	}
	for (auto formal : formals) {
		SymOpd* symOpd = formal;
		std::string memLoc = std::to_string(offset) + "(%rbp)";
		symOpd->setMemoryLoc(memLoc);
		offset -= int(symOpd->getWidth());
	}
}

void Procedure::toX64(std::ostream& out){
	//Allocate all locals
	allocLocals();

	enter->codegenLabels(out);
	enter->codegenX64(out);
	out << "#Fn body " << myName << "\n";
	for (auto quad : *bodyQuads){
		quad->codegenLabels(out);
		out << "#" << quad->toString() << "\n";
		quad->codegenX64(out);
	}
	out << "#Fn epilogue " << myName << "\n";
	leave->codegenLabels(out);
	leave->codegenX64(out);
}

void Quad::codegenLabels(std::ostream& out){
	if (labels.empty()){ return; }

	size_t index = 0;
	for (Label * label : labels){
		out << label->getName() << ": ";
		if (index != labels.size() - 1){ out << "\n"; }
		index++;
	}
}

void BinOpQuad::codegenX64(std::ostream& out){
	// TODO(Implement me)
	src1->genLoadVal(out,A);
	src2->genLoadVal(out,B);

	bool isBool = false;
	std::string opStr;
	std::string boolStr;
	
	switch(opr){
		case BinOp::ADD64: 
			opStr = "addq ";
			break;
		case BinOp::SUB64: 
			opStr ="subq ";
			break;
		case BinOp::DIV64: 
			opStr = "idivq ";
			break;
		case BinOp::MULT64: 
			opStr = "imul ";
			break;
		case BinOp::EQ64:
			isBool = true; 
			boolStr = "sete ";
			opStr = "cmpq ";
			break;
		case BinOp::NEQ64:
			isBool = true; 
			boolStr = "setne ";
			opStr = "cmpq ";
			break;
		case BinOp::LT64:
			isBool = true; 
			boolStr = "setl "; 
			opStr = "cmpq ";
			break;
		case BinOp::GT64: 
			isBool = true;
			boolStr = "setg ";
			opStr = "cmpq ";
			break;
		case BinOp::LTE64: 
			isBool = true;
			boolStr = "setle ";
			opStr = "cmpq ";
			break;
		case BinOp::GTE64: 
			isBool = true;
			boolStr = "setge ";
			opStr = "cmpq ";
			break;
		case BinOp::OR64:
			opStr = "orq ";
			break;
		case BinOp::AND64:
			opStr = "andq ";
			break;
		case BinOp::ADD8: 
			opStr = "addb ";
			break;
		case BinOp::SUB8: 
			opStr = "subb ";
			break;
		case BinOp::DIV8: 
			opStr = "divb ";
			break;
		case BinOp::MULT8: 
			opStr = "multb ";
			break;
		case BinOp::EQ8: 
			opStr = "cmpb ";
			break;
		case BinOp::NEQ8: 
			opStr = "cmpb ";
			break;
		case BinOp::LT8: 
			opStr = "cmpb ";
			break;
		case BinOp::GT8: 
			opStr = "cmpb ";
			break;
		case BinOp::LTE8: 
			opStr = "cmpb ";
			break;
		case BinOp::GTE8: 
			opStr = "cmpb ";
			break;
		case BinOp::OR8: 
			opStr = "orb ";
			break;
		case BinOp::AND8: 
			opStr = "andb ";
			break;
	}

	if (opr == BinOp::DIV64){
		out << "movq $0, %rdx\n" << opStr << "%rbx\n";
	} else if (isBool){
		out << "movq $0, %rcx\n"; 
		out << opStr << "%rbx, %rax\n";
		out << boolStr <<  "%cl\n";
		dst->genStoreVal(out,C);
	} else {
		out << opStr << "%rbx, %rax\n";
	}

	if(!isBool){
		dst->genStoreVal(out,A);
	}
}

void UnaryOpQuad::codegenX64(std::ostream& out){
	// TODO(Implement me)
	src->genLoadVal(out, A);
  std::string opStr;
	switch (op) {
		case UnaryOp::NEG64:
			opStr = "negq ";
			break;
		case UnaryOp::NEG8:
			opStr = "negb ";
			break;
		case UnaryOp::NOT64:
			opStr = "notq ";
			break;
		case UnaryOp::NOT8:
			opStr = "notb ";
			break;
	}
	out << opStr << "%rax \n";
	dst->genStoreVal(out,A);
}

void AssignQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void ReadQuad::codegenX64(std::ostream& out){
	if (myDst->locString() != "give") {
		myDst->genLoadVal(out, DI);
	} else {
		out << "movq $1, %rdi\n";
	}
	if (myDstType->isInt()) {
		out << "callq getInt\n";
	} else if (myDstType->isBool()) {
		out << "callq getBool\n";
	}
	myDst->genStoreVal(out, A);
}

void MagicQuad::codegenX64(std::ostream& out){
	// TODO(Implement me)
	out << "callq magic\n";
}

void ExitQuad::codegenX64(std::ostream& out){
	// out << "#leave quad to do\n";
	// out << "movq $60, %rax\n";
	// out << "syscall\n";
	out << "call exit\n";
}

void WriteQuad::codegenX64(std::ostream& out){
	mySrc->genLoadVal(out, DI);
	if (mySrcType->isInt()) {
		out << "callq printInt\n";
	} else if (mySrcType->isString()) {
		out << "callq printString\n";
	} else if (mySrcType->isBool()) {
		out << "callq printBool\n";
	}
}

void GotoQuad::codegenX64(std::ostream& out){
	out << "jmp " << tgt->getName() << "\n";
}

void IfzQuad::codegenX64(std::ostream& out){
	cnd->genLoadVal(out, A);
  out << "cmpq $0, %rax\n" << "je " << tgt->getName() << "\n";
}

void NopQuad::codegenX64(std::ostream& out){
	out << "nop" << "\n"; 
}

void CallQuad::codegenX64(std::ostream& out){
	int argSize = sym->getDataType()->asFn()->getFormalTypes()->getSize();
	int argNum = argSize/8;
	out << "callq fun_" << sym->getName() << "\n";
	if ( argNum >= 7 ) {
		out << "addq $" << ( argSize - 48 ) << ", %rsp\n";
	}
}

void EnterQuad::codegenX64(std::ostream& out){
	out << "pushq %rbp\n";
	out << "movq %rsp, %rbp\n";
	out << "addq $16, %rbp\n";
	out << "subq $" << myProc->arSize() << ", %rsp\n";
}

void LeaveQuad::codegenX64(std::ostream& out){
	out << "addq $" << myProc->arSize() << ", %rsp\n";
	out << "popq %rbp\n";
	out << "retq\n";
}

void SetArgQuad::codegenX64(std::ostream& out){
	switch (index) {
		case 1:
			opd->genLoadVal(out, DI);
			break;
		case 2:
			opd->genLoadVal(out, SI);
			break;
		case 3:
			opd->genLoadVal(out, D);
			break;
		case 4:
			opd->genLoadVal(out, C);
			break;
		case 5:
<<<<<<< HEAD
			opd->genLoadVal(out, DI);
			break;
		case 6:
			opd->genLoadVal(out, SI);
			break;
=======
			opd->genLoadVal(out, EIGHT);
      break;
		case 6:
			opd->genLoadVal(out, NINE);
      break;
>>>>>>> 1bbf409736180f389980004b2888052692382f61
		default:
			opd->genLoadVal(out, A);
			out << "pushq %rax\n";
	}
}

void GetArgQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
  
  switch(index) {
    case 1: 
      opd->genStoreVal(out, DI);
      break;
    case 2: 
      opd->genStoreVal(out, SI);
      break;
    case 3: 
      opd->genStoreVal(out, D);
      break;
    case 4: 
      opd->genStoreVal(out, C);
      break;
    case 5: 
      opd->genStoreVal(out, EIGHT);
      break;
    case 6: 
      opd->genStoreVal(out, NINE);
      break;
    default:
      size_t offset = 8*(totalArgs-index);
      out << "movq " << offset << "(%rbp), %rax\n";
      opd->genStoreVal(out,A);
  }
}

void SetRetQuad::codegenX64(std::ostream& out){
	// TODO(Implement me)
	opd->genLoadVal(out, A);
}

void GetRetQuad::codegenX64(std::ostream& out){
	// TODO(Implement me)
	opd->genStoreVal(out, A);
}

void LocQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
}

void SymOpd::genLoadVal(std::ostream& out, Register reg){
	out << getMovOp() << " " << getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void SymOpd::genStoreVal(std::ostream& out, Register reg){
	out << getMovOp() << " " << getReg(reg) << ", " << getMemoryLoc() << "\n";
}

void SymOpd::genLoadAddr(std::ostream& out, Register reg) {
	TODO(Implement me if necessary)
}

void AuxOpd::genLoadVal(std::ostream& out, Register reg){
	out << getMovOp() << " " << getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void AuxOpd::genStoreVal(std::ostream& out, Register reg){
	out << getMovOp() << " " << getReg(reg) << ", " << getMemoryLoc() << "\n";
}
void AuxOpd::genLoadAddr(std::ostream& out, Register reg){
	out << getMovOp() << " " << this->getMemoryLoc() << ", " << getReg(reg) << "\n"; 
}


void AddrOpd::genStoreVal(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadVal(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genStoreAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadAddr(std::ostream & out, Register reg){
	TODO(Implement me)
}

void LitOpd::genLoadVal(std::ostream & out, Register reg){
	out << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

}
