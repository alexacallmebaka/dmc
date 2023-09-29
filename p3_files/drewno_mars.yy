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
   std::list<drewno_mars::FormalDeclNode *> *  transFormalDeclList;
   std::list<drewno_mars::StmtNode *> *  transStmtList;
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
 		| classDecl { }
 		| fnDecl {
    $$ = $1;
    }

varDecl 	: id COLON type
		  {
		  const Position * p;
		  p = new Position($1->pos(), $2->pos());
		  $$ = new VarDeclNode(p, $1, $3);
		  }
		| id COLON type ASSIGN exp
		  {
		  const Position * p;
		  p = new Position($1->pos(), $2->pos());
		  $$ = new VarDeclNode(p, $1, $3, $5);
		  }

type		: primType
		  {
		  $$ = $1;
		  }
		| id
		  {
		  }
		| PERFECT primType
		  {
		  const Position * p;
		  p = new Position($1->pos(), $2->pos());
      $$ = new PerfectTypeNode(p, $2);
		  }
		| PERFECT id
		  {
		  }

primType 	: INT
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
		  }

classDecl	: id COLON CLASS LCURLY classBody RCURLY SEMICOL
		  {
		  }

classBody	: classBody varDecl SEMICOL
		  {
		  }
		| classBody fnDecl
		  {
		  }
		| /* epsilon */
		  {
		  }

fnDecl  : id COLON LPAREN formals RPAREN type LCURLY stmtList RCURLY
		  {
      const Position * p = $1->pos();
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
      const Position * p = $1->pos();
      $$ = new FormalDeclNode(p, $3, $1);
		  }

stmtList 	: /* epsilon */
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

blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
      const Position * p = $1->pos();
      $$ = new WhileStmtNode(p, $3, $6);
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
      const Position * p = $1->pos();
      $$ = new IfStmtNode(p, $3, $6);
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
		  {
      const Position * p = $1->pos();
      $$ = new IfElseStmtNode(p, $3, $6, $10);
		  }

stmt		: varDecl
		  {
      $$ = $1;
		  }
		| loc ASSIGN exp
		  {
		  }
		| loc POSTDEC
		  {
		  }
		| loc POSTINC
		  {
		  }
		| GIVE exp
		  {
		  }
		| TAKE loc
		  {
		  }
		| RETURN exp
		  {
		  }
		| RETURN
		  {
		  }
		| EXIT
		  {
		  }
		| callExp
		  { 
		  }

exp		: exp DASH exp
	  	  {
		  }
		| exp CROSS exp
	  	  {
		  }
		| exp STAR exp
	  	  {
		  }
		| exp SLASH exp
	  	  {
		  }
		| exp AND exp
	  	  {
		  }
		| exp OR exp
	  	  {
		  }
		| exp EQUALS exp
	  	  {
		  }
		| exp NOTEQUALS exp
	  	  {
		  }
		| exp GREATER exp
	  	  {
		  }
		| exp GREATEREQ exp
	  	  {
		  }
		| exp LESS exp
	  	  {
		  }
		| exp LESSEQ exp
	  	  {
		  }
		| NOT exp
	  	  {
		  }
		| DASH term
	  	  {
		  }
		| term
	  	  {
        $$ = $1;
		  }

callExp		: loc LPAREN RPAREN
		  {
		  }
		| loc LPAREN actualsList RPAREN
		  {
		  }

actualsList	: exp
		  {
		  }
		| actualsList COMMA exp
		  {
		  }

term 		: loc
		  { 
		  }
		| INTLITERAL 
		  {
		  const Position * p = $1->pos();
      $$ = new IntLitNode(p, $1->num());
		  }
		| STRINGLITERAL 
		  {
		  }
		| TRUE
		  {
		  }
		| FALSE
		  {
		  }
		| MAGIC
		  {
		  }
		| LPAREN exp RPAREN
		  {
		  }
		| callExp
		  {
		  }

loc		: id
		  {
		  $$ = $1;
		  }
		| loc POSTDEC id
		  {
		  }

id		: ID
		  {
		  const Position * pos = $1->pos();
		  $$ = new IDNode(pos, $1->value());
		  }
	
%%

void drewno_mars::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
