/* preamble {{{1*/
%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace{drewno_mars}
%define api.parser.class {Parser}
%define parse.error verbose
%output "parser.cc"
%token-table

%code requires{
	#include <list>
	#include "tokens.hpp"
	#include "ast.hpp"
	namespace drewno_mars {
		class Scanner;
	}

//The following definition is required when 
// we don't use the %locations directive (which we won't)
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

//End "requires" code
}

%parse-param { drewno_mars::Scanner &scanner }
%parse-param { drewno_mars::ProgramNode** root }
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   #include "ast.hpp"
   #include "tokens.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}
/*end preamble 1}}}*/

/*types {{{1*/
/*typedefs (union) {{{2*/
/*
The %union directive is a way to specify the 
set of possible types that might be used as
translation attributes on a symbol.
For this project, only terminals have types (we'll
have translation attributes for non-terminals in the next
project)
*/

%union {
   drewno_mars::Token*                         transToken;
   drewno_mars::Token*                         lexeme;
   drewno_mars::IDToken*                       transIDToken;
   drewno_mars::IntLitToken*                   transIntToken;
   drewno_mars::StrToken*                      transStrToken;
   drewno_mars::ProgramNode*                   transProgram;
   drewno_mars::DeclNode *                     transDecl;
   std::list<drewno_mars::DeclNode *> *        transDeclList;
   drewno_mars::VarDeclNode *                  transVarDecl;
   drewno_mars::TypeNode *                     transType;
   drewno_mars::LocNode *                      transLoc;
   drewno_mars::IDNode *                       transID;
   drewno_mars::ExpNode *                      transExp;
   drewno_mars::StmtNode *                     transStmt;
   drewno_mars::FnDeclNode *                   transFnDecl;
   drewno_mars::FormalDeclNode *               transFormalDecl;
   drewno_mars::ClassDefnNode *                transClassDefn;
   std::list<drewno_mars::FormalDeclNode *> *  transFormalDeclList;
   std::list<drewno_mars::StmtNode *> *        transStmtList;
   std::list<drewno_mars::ExpNode *> *         transExpList;
}
/*end typedefs 2}}}*/
%define parse.assert

/* types of terminals {{{2*/
/* Terminals 
 *  No need to touch these, but do note the translation type
 *  of each node. Most are just "transToken", which is defined in
 *  the %union above to mean that the token translation is an instance
 *  of drewno_mars::Token *, and thus has no fields (other than line and column).
 *  Some terminals, like ID, are "transIDToken", meaning the translation
 *  also has a name field. 
*/
%token                   END	   0 "end file"
%token	<transToken>     AND
%token	<transToken>     ASSIGN
%token	<transToken>     BOOL
%token	<transToken>     COLON
%token	<transToken>     COMMA
%token	<transToken>     CLASS
%token	<transToken>     DASH
%token	<transToken>     ELSE
%token	<transToken>     EXIT
%token	<transToken>     EQUALS
%token	<transToken>     FALSE
%token	<transToken>     GIVE
%token	<transToken>     GREATER
%token	<transToken>     GREATEREQ
%token	<transIDToken>   ID
%token	<transToken>     IF
%token	<transToken>     INT
%token	<transIntToken>  INTLITERAL
%token	<transToken>     LCURLY
%token	<transToken>     LESS
%token	<transToken>     LESSEQ
%token	<transToken>     LPAREN
%token	<transToken>     MAGIC
%token	<transToken>     NOT
%token	<transToken>     NOTEQUALS
%token	<transToken>     OR
%token	<transToken>     PERFECT
%token	<transToken>     CROSS
%token	<transToken>     POSTDEC
%token	<transToken>     POSTINC
%token	<transToken>     RETURN
%token	<transToken>     RCURLY
%token	<transToken>     RPAREN
%token	<transToken>     SEMICOL
%token	<transToken>     SLASH
%token	<transToken>     STAR
%token	<transStrToken>  STRINGLITERAL
%token	<transToken>     TAKE
%token	<transToken>     TRUE
%token	<transToken>     VOID
%token	<transToken>     WHILE
/*end terminal types 2}}}*/

/*types of nonterminals {{{2*/
/* Nonterminals
*  The specifier in angle brackets
*  indicates the type of the translation attribute using
*  the names defined in the %union directive above
*  TODO: You will need to add more attributes for other nonterminals
*  to this list as you add productions to the grammar
*  below (along with indicating the appropriate translation
*  attribute type).
*/
/*    (attribute type)    (nonterminal)    */
%type <transProgram> program
%type <transDeclList> globals
%type <transDecl> decl
%type <transVarDecl> varDecl
%type <transType> type
%type <transType> primType
%type <transLoc> loc
%type <transID> id
%type <transExp> exp
%type <transExp> term
%type <transStmt> stmt
%type <transStmt> blockStmt
%type <transFnDecl> fnDecl
%type <transFormalDeclList> formals
%type <transFormalDeclList> formalsList
%type <transFormalDecl> formalDecl
%type <transStmtList> stmtList
%type <transExp> callExp
%type <transClassDefn> classDecl
%type <transExpList> actualsList
%type <transDeclList> classBody
/*end types of nonterminals 2}}}*/
/*end types 1}}}*/

/*associativity {{{1*/
%right ASSIGN
%left OR
%left AND
%nonassoc LESS GREATER LESSEQ GREATEREQ EQUALS NOTEQUALS
%left DASH CROSS
%left STAR SLASH
%left NOT 
/* end associativity 1}}}*/

%%

/*globals and decls {{{1*/
program 	: globals
		  {
		  $$ = new ProgramNode($1);
		  *root = $$;
		  }

globals 	: globals decl
	  	  { 
		  $$ = $1;
		  DeclNode * declNode = $2;
		  $$->push_back(declNode);
	  	  }
		| /* epsilon */
		  {
		  $$ = new std::list<DeclNode *>();
		  }

decl 		: varDecl SEMICOL
		  {
		  $$ = $1;
		  }
 		| classDecl { $$ = $1; }
 		| fnDecl {
    $$ = $1;
    }
/*1}}}*/

varDecl 	: id COLON type /*{{{1*/
		  {
		  const Position * p;
		  p = new Position($1->pos(), $3->pos());
		  $$ = new VarDeclNode(p, $1, $3);
		  }
		| id COLON type ASSIGN exp
		  {
		  const Position * p;
		  p = new Position($1->pos(), $5->pos());
		  $$ = new VarDeclNode(p, $1, $3, $5);
		  }
/*1}}}*/

/* types {{{1*/
type		: primType
		  {
		  $$ = $1;
		  }
		| id
		  {
		  const Position * p;
      $$ = new ClassTypeNode($1->pos(), $1);
		  }
		| PERFECT primType
		  {
		  const Position * p;
		  p = new Position($1->pos(), $2->pos());
      $$ = new PerfectTypeNode(p, $2);
		  }
		| PERFECT id
		  {
		  const Position * p = new Position($1->pos(), $2->pos());
      ClassTypeNode * type = new ClassTypeNode($2->pos(), $2);
      $$ = new PerfectTypeNode(p, type);
		  }
/*1}}}*/

primType 	: INT /*{{{1*/
	  	  { 
		  $$ = new IntTypeNode($1->pos());
		  }
		| BOOL
		  {
		  $$ = new BoolTypeNode($1->pos());
		  }
		| VOID
		  {
		  $$ = new VoidTypeNode($1->pos());
		  } /*1}}}*/

/* classes {{{1*/
classDecl	: id COLON CLASS LCURLY classBody RCURLY SEMICOL
		  {
      const Position * p = new Position($1->pos(), $7->pos());
      $$ = new ClassDefnNode(p,$1,$5);
		  }

classBody	: classBody varDecl SEMICOL
		  {
      $1->push_back($2);
      $$ = $1;
		  }
		| classBody fnDecl
		  {
      $1->push_back($2);
      $$ = $1;
		  }
		| /* epsilon */
		  {
      $$ = new std::list< DeclNode * >();
		  }
/*1}}}*/

/* fndecl + formals {{{1*/
fnDecl  : id COLON LPAREN formals RPAREN type LCURLY stmtList RCURLY
		  {
      const Position * p = new Position($1->pos(), $9->pos());
      $$ = new FnDeclNode(p, $6, $1, $4, $8); 
		  }

formals 	: /* epsilon */
		  {
      $$ = new std::list< FormalDeclNode * >;
		  }
		| formalsList
		  {
      $$ = $1;
		  }

formalsList 	: formalDecl
		  {
      std::list< FormalDeclNode * > * formals = new std::list< FormalDeclNode * >;
      formals->push_back($1);
      $$ = formals;
		  }
		| formalsList COMMA formalDecl
		  {
      $1->push_back($3);
      $$ = $1;
		  }

formalDecl 	: id COLON type
		  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new FormalDeclNode(p, $3, $1);
		  }
/*end fn decl + formals 1}}}*/

stmtList 	: /* epsilon */ /*{{{1*/
	   	  {
        $$ = new std::list< StmtNode * >;
	   	  }
		| stmtList stmt SEMICOL
	  	  {
        $1->push_back($2);
        $$ = $1;
	  	  }
		| stmtList blockStmt
	  	  {
        $1->push_back($2);
        $$ = $1;
	  	  }
/*1}}}*/

blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY /*{{{1*/
		  {
      const Position * p = new Position($1->pos(), $7->pos());
      $$ = new WhileStmtNode(p, $3, $6);
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
      const Position * p = new Position($1->pos(), $7->pos());
      $$ = new IfStmtNode(p, $3, $6);
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
		  {
      const Position * p = new Position($1->pos(), $9->pos());
      $$ = new IfElseStmtNode(p, $3, $6, $10);
		  }
/*1}}}*/

/*stmts {{{1*/
stmt		: varDecl
		  {
      $$ = $1;
		  }
		| loc ASSIGN exp
		  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new AssgnStmtNode(p, $1, $3);
		  }
		| loc POSTDEC
		  {
      const Position * p = new Position($1->pos(), $2->pos());
      $$ = new PostDecStmtNode(p, $1);
		  }
		| loc POSTINC
		  {
      const Position * p = new Position($1->pos(), $2->pos());
      $$ = new PostIncStmtNode(p, $1);
		  }
		| GIVE exp
		  {
      const Position * p = new Position($1->pos(), $2->pos());
      $$ = new GiveStmtNode(p, $2);
		  }
		| TAKE loc
		  {
      const Position * p = new Position($1->pos(), $2->pos());
      $$ = new TakeStmtNode(p, $2);
		  }
		| RETURN exp
		  {
      const Position * p = new Position($1->pos(), $2->pos());
      $$ = new ReturnStmtNode(p, $2); 
		  }
		| RETURN
		  {
      $$ = new ReturnStmtNode($1->pos(), nullptr); 
		  }
		| EXIT
		  {
      $$ = new ExitStmtNode($1->pos()); 
		  }
		| callExp
		  { 
      $$ = new CallStmtNode($1->pos(), $1);
		  }
/*1}}}*/

exp		: exp DASH exp /*{{{1*/
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new MinusNode(p, $1, $3);
		  }
		| exp CROSS exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new PlusNode(p, $1, $3);
		  }
		| exp STAR exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new TimesNode(p, $1, $3);
		  }
		| exp SLASH exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new DivideNode(p, $1, $3);
		  }
		| exp AND exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new AndNode(p, $1, $3);
		  }
		| exp OR exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new OrNode(p, $1, $3);
		  }
		| exp EQUALS exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new EqualsNode(p, $1, $3);
		  }
		| exp NOTEQUALS exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new NotEqualsNode(p, $1, $3);
		  }
		| exp GREATER exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new GreaterNode(p, $1, $3);
		  }
		| exp GREATEREQ exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new GreaterEqNode(p, $1, $3);
		  }
		| exp LESS exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new LessNode(p, $1, $3);
		  }
		| exp LESSEQ exp
	  	  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new LessEqNode(p, $1, $3);
		  }
		| NOT exp
	  	  {
      const Position * p = new Position($1->pos(), $2->pos());
      $$ = new NotNode(p, $2);
		  }
		| DASH term
	  	  {
      const Position * p = new Position($1->pos(), $2->pos());
      $$ = new NegNode(p, $2);
		  }
		| term
	  	  {
        $$ = $1;
		  }
/*1}}}*/

/*call exprs{{{1*/
callExp		: loc LPAREN RPAREN
		  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new CallExpNode(p, new std::list< ExpNode * >(), $1);
		  }
		| loc LPAREN actualsList RPAREN
		  {
      const Position * p = new Position($1->pos(), $4->pos());
      $$ = new CallExpNode(p, $3, $1);
		  }

actualsList	: exp
		  {
      std::list< ExpNode *> * actuals = new std::list< ExpNode * >;
      actuals->push_back($1);
      $$ = actuals;
		  }
		| actualsList COMMA exp
		  {
      $1->push_back($3);
      $$ = $1;
		  }
/*1}}}*/

term 		: loc /*{{{1*/
		  { 
		  }
		| INTLITERAL 
		  {
		  const Position * p = $1->pos();
      $$ = new IntLitNode(p, $1->num());
		  }
		| STRINGLITERAL 
		  {
		  const Position * p = $1->pos();
      $$ = new StrLitNode(p, $1->str());
		  }
		| TRUE
		  {
		  const Position * p = $1->pos();
      $$ = new TrueNode(p);
		  }
		| FALSE
		  {
		  const Position * p = $1->pos();
      $$ = new FalseNode(p);
		  }
		| MAGIC
		  {
		  const Position * p = $1->pos();
      $$ = new MagicNode(p);
		  }
		| LPAREN exp RPAREN
		  {
      $$ = $2;
		  }
		| callExp
		  {
      $$ = $1;
		  }
/*1}}}*/

loc		: id /*{{{1*/
		  {
		  $$ = $1;
		  }
		| loc POSTDEC id
		  {
      const Position * p = new Position($1->pos(), $3->pos());
      $$ = new MemberFieldExpNode(p, $1, $3);
		  }
/*1}}}*/

id		: ID /*{{{1*/
		  {
		  const Position * pos = $1->pos();
		  $$ = new IDNode(pos, $1->value());
		  }
/*1}}}*/
	
%%

void drewno_mars::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
