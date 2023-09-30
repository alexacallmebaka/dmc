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
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}

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
   drewno_mars::Position* transPosition;
}

%define parse.assert

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
%token	<transToken>     OPEN
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

%type program
%type globals

/* NOTE: Make sure to add precedence and associativity 
 * declarations
 */
%right ASSIGN

%left AND
%left OR

%nonassoc LESS LESSEQ NOTEQUALS GREATER GREATEREQ EQUALS

%left DASH
%left CROSS
%left STAR 
%left SLASH
%left NOT

%%

program 	: globals {
						//For the project, we will only be checking std::cerr for 
						// correctness. You might choose to uncomment the following
						// Line to help you debug, which will print when this
						// production is applied
						std::cout << "got to the program ::= globals rule\n";
						}

globals 	: globals decl {std::cout << "got to the globals ::= decl rule\n"; }
			| /* epsilon */ {std::cout << "got to the program ::= epsilon rule\n"; }

decl		: varDecl SEMICOL {std::cout << "got to the varDecl ::= SEMICOL rule\n"; }
			| classDecl {std::cout << "got to the varDecl ::= classDecl rule\n"; }
			| fnDecl {std::cout << "got to the varDecl ::= fnDecl rule\n"; }

varDecl 	: id COLON type { std::cout << "got to the varDecl ::= id rule\n"; }
			| id COLON type ASSIGN exp {std::cout << "got to the varDecl ::= ASSIGN rule\n"; }

type		: primType {std::cout << "got to the type ::= primType rule\n"; }
			| PERFECT primType {std::cout << "got to the type ::= PERFECT primType rule\n"; }
			| id {std::cout << "got to the type ::= id rule\n"; }
			| PERFECT id {std::cout << "got to the type ::= PERFECT id rule\n"; }

primType	: INT {std::cout << "got to the primType ::= INT rule\n"; }
			| BOOL {std::cout << "got to the primType ::= BOOL rule\n"; }
			| VOID {std::cout << "got to the primType ::= VOID rule\n"; }

classDecl	: id COLON CLASS LCURLY classBody RCURLY SEMICOL {std::cout << "got to the classDecl ::= classBody rule\n"; }

classBody	: classBody varDecl {std::cout << "got to the classBody ::= varDecl rule\n"; }
			| classBody fnDecl {std::cout << "got to the classBody ::= fnDecl rule\n"; }
			| /* epsilon */ {std::cout << "got to the classBody ::= epsilon rule\n"; }

fnDecl		: id COLON LPAREN formals RPAREN type LCURLY stmtList RCURLY {std::cout << "got to the fnDecl ::= formals rule\n"; }
			|  id COLON LPAREN RPAREN type LCURLY stmtList RCURLY {std::cout << "got to the fnDecl ::= lpar rpar rule\n"; }

formals		: formalsList {std::cout << "got to the formals ::= formalsList rule\n"; }

formalsList	: formalDecl {std::cout << "got to the formalsList ::= formalDecl rule\n"; }
			| formalsList COMMA formalDecl {std::cout << "got to the formalsList ::= COMMA rule\n"; }

formalDecl	: id COLON type {std::cout << "got to the formalDecl ::= id COLON type rule\n"; }

stmtList	: stmtList stmt SEMICOL {std::cout << "got to the stmtList ::= SEMICOL rule\n"; }
			| stmtList blockStmt {std::cout << "got to the stmtList ::= blockStmt rule\n"; }
			| /* epsilon */ {std::cout << "got to the stmtList ::= epsilon rule\n"; }

blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY {std::cout << "got to the blockStmt ::= WHILE rule\n"; }
			| IF LPAREN exp RPAREN LCURLY stmtList RCURLY {std::cout << "got to the blockStmt ::= if rule\n"; }
			| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY {std::cout << "got to the blockStmt ::= else rule\n"; }

stmt		: varDecl {std::cout << "got to the stmt ::= varDecl rule\n"; }
			| loc ASSIGN exp {std::cout << "got to the stmt ::= ASSIGN rule\n"; }
			| loc POSTDEC {std::cout << "got to the stmt ::= POSTDEC rule\n"; }
			| GIVE exp {std::cout << "got to the stmt ::= GIVE rule\n"; }
			| TAKE loc {std::cout << "got to the stmt ::= TAKE rule\n"; }
			| RETURN exp {std::cout << "got to the stmt ::= RETURN exp rule\n"; }
			| RETURN {std::cout << "got to the stmt ::= RETURN rule\n"; }
			| EXIT {std::cout << "got to the stmt ::= EXIT rule\n"; }
			| callExp {std::cout << "got to the stmt ::= callExp rule\n"; }

exp         : exp DASH exp {std::cout << "got to the exp ::= DASH rule\n"; }
        	| exp CROSS exp {std::cout << "got to the exp ::= CROSS rule\n"; }
            | exp STAR exp {std::cout << "got to the exp ::= STAR rule\n"; }
            | exp SLASH exp {std::cout << "got to the exp ::= SLASH rule\n"; }
            | exp AND exp {std::cout << "got to the exp ::= AND rule\n"; }
            | exp OR exp {std::cout << "got to the exp ::= OR rule\n"; }
            | exp EQUALS exp {std::cout << "got to the exp ::= EQUALS rule\n"; }
            | exp NOTEQUALS exp {std::cout << "got to the exp ::= NOTEQUALS rule\n"; }
            | exp GREATER exp {std::cout << "got to the exp ::= GREATER rule\n"; }
            | exp GREATEREQ exp {std::cout << "got to the exp ::= GREATEREQ rule\n"; }
            | exp LESS exp {std::cout << "got to the exp ::= LESS rule\n"; }
            | exp LESSEQ exp {std::cout << "got to the exp ::= LESSEQ rule\n"; }
            | NOT exp {std::cout << "got to the exp ::= NOT rule\n"; }
    		| DASH term {std::cout << "got to the exp ::= DASH term rule\n"; }
            | term {std::cout << "got to the exp ::= term rule\n"; }

callExp     : id LPAREN RPAREN {std::cout << "got to the callExp ::= lpar rpar rule\n"; }   // fn call with no args
            | id LPAREN actualsList RPAREN {std::cout << "got to the callExp ::= actualsList rule\n"; }  // with args

actualsList : exp {std::cout << "got to the actualsList ::= exp rule\n"; }
            | actualsList COMMA exp {std::cout << "got to the actualsList ::= COMMA rule\n"; }
                
term        : loc {std::cout << "got to the term ::= loc rule\n"; }
            | INTLITERAL {std::cout << "got to the term ::= INTLITERAL rule\n"; }
            | STRINGLITERAL {std::cout << "got to the term ::= STRINGLITERAL rule\n"; }
            | TRUE {std::cout << "got to the term ::= TRUE rule\n"; }
            | FALSE {std::cout << "got to the term ::= FALSE rule\n"; }
            | MAGIC {std::cout << "got to the term ::= MAGIC rule\n"; }
            | LPAREN exp RPAREN {std::cout << "got to the term ::= exp rule\n"; }
            | callExp {std::cout << "got to the term ::= callExp rule\n"; }

loc			: id {std::cout << "got to the loc ::= id rule\n"; }
			| loc POSTDEC id {std::cout << "got to the loc ::= POSTDEC rule\n"; }

id			: ID {std::cout << "got to the id ::= ID rule\n"; }
	
%%

void drewno_mars::Parser::error(const std::string& msg){
	//For the project, we will only be checking std::cerr for 
	// correctness. You might choose to uncomment the following
	// Line to help you debug, since it gives slightly more 
	// descriptive error messages 
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
