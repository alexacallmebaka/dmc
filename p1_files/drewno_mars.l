%{
#include <string>
#include <limits.h>

/* Get our custom yyFlexScanner subclass */
#include "scanner.hpp"
#include "tokens.hpp"
#undef  YY_DECL
#define YY_DECL int drewno_mars::Scanner::yylex( drewno_mars::Parser::semantic_type * const lval )

/* define yyterminate as this instead of NULL */
#define yyterminate() return( TokenKind::END )

/* Exclude unistd.h for Visual Studio compatability. */
#define YY_NO_UNISTD_H

/* max int size */
#define INTMAX 2147483647

/*macro for basic token*/
#define basicToken(TOK)\
int tokenKind;\
tokenKind = TokenKind::TOK;\
int offset = strlen(yytext);\
Position * pos = new Position(lineNum, colNum, lineNum, colNum+offset);\
this->yylval->lexeme = new Token(pos, tokenKind);\
colNum = colNum+offset;\
return tokenKind;

/* typedef to make the reference to token kinds shorter */
using TokenKind = drewno_mars::Parser::token;

%}

%option debug
%option nodefault
%option yyclass="drewno_mars::Scanner"
%option noyywrap
%option c++

DIGIT [0-9]
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
STRING [^"]*
%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

bool { basicToken(BOOL) }
and { basicToken(AND) }
24Kmagic { basicToken(MAGIC) }
"+" { basicToken(CROSS) }
- { basicToken(DASH) }

{DIGIT}+	{ //{{{2
    //Note that the IntLitToken does not take a TokenKind argument
    // since it's implicitly known that the token represents an
    // INTLITERAL		
    Position * pos = new Position(
      lineNum, colNum, lineNum, colNum+strlen(yytext));
    
    /*if the int is big enough c++'s int will overflow so we
    use a long long for the best safety possible. */  
    long long longIntVal = std::strtoll(yytext, nullptr, 10);
    if (longIntVal > INTMAX) {
      std::string msg = "Integer literal overflow.";
      Report::fatal(pos,msg);
      yylval->lexeme = new IntLitToken(pos, 0);
    }

    else {
      int intVal = atoi(yytext);

      yylval->lexeme = new IntLitToken(pos, intVal);
    }

    colNum = colNum+strlen(yytext);

		//As before, alert the client program that an INTLITERAL 
		// token was matched
		return TokenKind::INTLITERAL;
		} //2}}}

{IDENTIFIER}+   {
    std::string idVal = yytext;
    Position * pos = new Position(
      lineNum, colNum, lineNum, colNum+strlen(yytext));
    colNum += strlen(yytext);
    yylval->lexeme = new IDToken(pos, idVal);
    return TokenKind::ID;
  }

[//].* {
				// ignore comment
	}


\n	{ //{{{2
		//Update the line number, reset the columnNumber
		lineNum++;
		colNum = 1;
		} //2}}}


.   { //{{{2
		//TODO: Add the rest of the rules above this point so that legal characters
		// are properly accounted for. 
		std::string msg = "Illegal character ";
		msg += yytext;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		Report::fatal(pos,msg);
		colNum += strlen(yytext);
    } //2}}}
%%
