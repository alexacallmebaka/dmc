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
   drewno_mars::Position* 										 transPosition;
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

%type <transPosition> program
%type <transPosition> globals
%type <transPosition> decl
/* NOTE: Make sure to add precedence and associativity 
 * declarations
 */

%right ASSIGN
%left AND
%left OR
%nonassoc LESS LESSEQ GREATER GREATEREQ NOTEQUALS EQUALS
%left DASH
%left CROSS
%left STAR
%left SLASH
%left NOT


%%

program 		: globals { }

globals 		: globals decl { }
						| /* epsilon */ { }

decl 				: varDecl SEMICOL { }
						| classDecl { }
						| fnDecl { }

varDecl 		: id COLON type { /* There are other rules for varDecl to add as well */ }
						| id COLON type ASSIGN exp { }

	type			: primType { }
						| PERFECT primType { }
						| id { }
						| PERFECT id { }

primType  	: INT { }
        		| BOOL { }
          	| VOID { }

classDecl		: id COLON CLASS LCURLY classBody RCURLY SEMICOL { }

classBody		: classBody varDecl { }
          	| classBody fnDecl { }
          	| /* epsilon */ { }

fnDecl    	: id COLON LPAREN formals RPAREN type LCURLY stmtList RCURLY { }
		 				| id COLON LPAREN RPAREN type LCURLY stmtList RCURLY { }

formals   	: formalsList { }
          	//|  /* epsilon */ { }

formalsList : formalDecl { }
          	| formalsList COMMA formalDecl { }

formalDecl	: id COLON type { }

stmtList    : stmtList stmt SEMICOL { }
            | stmtList blockStmt { }
            | /* epsilon */ { }

blockStmt		: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY { }
						| IF LPAREN exp RPAREN LCURLY stmtList RCURLY { }
						| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY { }

stmt        : varDecl { }
						| loc ASSIGN exp { }
						| loc POSTDEC { }
						| loc POSTINC { }
						| GIVE exp { }
						| TAKE loc { }
						| RETURN exp { }
						| RETURN { }
						| EXIT { }
						| callExp { }

exp         : exp DASH exp { }
						| exp CROSS exp { }
						| exp STAR exp { }
						| exp SLASH exp { }
						| exp AND exp { }
						| exp OR exp { }
						| exp EQUALS exp { }
						| exp NOTEQUALS exp { }
						| exp GREATER exp { }
						| exp GREATEREQ exp { }
						| exp LESS exp { }
						| exp LESSEQ exp { }
						| NOT exp { }
						| DASH term { }
						| term { }

callExp     : id LPAREN RPAREN   // fn call with no args { }
          	| id LPAREN actualsList RPAREN  // with args { }

actualsList : exp { }
          	| actualsList COMMA exp { }
                
term        : loc { }
						| INTLITERAL { }
						| STRINGLITERAL { }
						| TRUE { }
						| FALSE { }
						| MAGIC { }
						| LPAREN exp RPAREN { }
						| callExp { }

loc					: id { }
						| loc POSTDEC id { }

id					: ID { }
 /* TODO: add productions for the entire grammar of the language */
	
%%

void drewno_mars::Parser::error(const std::string& msg){
	//For the project, we will only be checking std::cerr for 
	// correctness. You might choose to uncomment the following
	// Line to help you debug, since it gives slightly more 
	// descriptive error messages 
	//std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
