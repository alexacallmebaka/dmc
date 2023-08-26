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

/* typedef to make the reference to token kinds shorter */
using TokenKind = drewno_mars::Parser::token;

%}

%option debug
%option nodefault
%option yyclass="drewno_mars::Scanner"
%option noyywrap
%option c++

DIGIT [0-9]

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

and { //{{{2
		int tokenKind = TokenKind::AND;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+3);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum+3;
		return tokenKind;
		} //2}}}

24Kmagic { //{{{2
		int tokenKind = TokenKind::MAGIC;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+8);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum = colNum+8;
		return tokenKind;
		} //2}}}

"+"		{ //{{{2
		// Keep track of the "kind" of token we matched. In this case,
		// it's a CROSS (aka the plus symbol). The full list of token
		// kinds is declared around line 197 of grammar.hh. The testing
		// functionScanner::outputTokens also indicates what each token
		// represents (declared in scanner.cpp line 9)
		int tokenKind = TokenKind::CROSS;

		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);

		//Instatiate a token. There are 5 classes of tokens defined based on 
		// - a literal string (StringLitToken)
		// - a literal integer (IntLitToken)
		// - a literal short (ShortLitToken)
		// - an identifier (IDToken)
		// - a base Token (any other type)
		// The token object is a handy place to keep information about the
		// token (i.e. token metadata) such as the line and column on which
		// the token character started (which can be used later in debug 
		// messages, etc). 
		Token * token = new Token(pos, tokenKind);

		//Assign the token to a instance variable of the scanner.
		// (When Flex processes a .l file in C++ mode, it creates 
		// a class called Scanner, which is a subclass of yyFlexScanner)
		// By assigning a value to this field, we put the token
		// metadata in a place where scanner's client program can find it.
		this->yylval->lexeme = token;

		//Since we consumed 1 character, increase the column number by 1
		colNum++;

		//Let the scanner's client program know that a PLUS token
		// has been matched
		return tokenKind;
		} //2}}}

"-" { //{{{2
		//Essentially the same thing as CROSS, but without the verbose
		// comments so it doesn't look so scary.
		int tokenKind = TokenKind::DASH;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		this->yylval->lexeme = new Token(pos, tokenKind);
		colNum++;
		return tokenKind;
		} //2}}}

{DIGIT}+	{ //{{{2
		//TODO: The following computation of the integer
		// value does not check for overflow. It should do so.


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

		//TODO: update the colNum count according to the number of
		// characters consumed. 
    colNum = colNum+strlen(yytext);

		//As before, alert the client program that an INTLITERAL 
		// token was matched
		return TokenKind::INTLITERAL;
		} //2}}}

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
