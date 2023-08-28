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

%x IN_STR

DIGIT [0-9]
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
STRING (\\[nt\"\\]|[^\n\t\"\\])*

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

and { basicToken(AND) }
24Kmagic { basicToken(MAGIC) }
else { basicToken(ELSE) }
false|"too hot" { basicToken(FALSE) }
if { basicToken(IF) }
int { basicToken(INT) }
give { basicToken(GIVE) }
take { basicToken(TAKE) }
or { basicToken(OR) }
perfect { basicToken(PERFECT) }
return { basicToken(RETURN) }
true { basicToken(TRUE) }
"today I don't feel like doing any work" { basicToken(EXIT) }
void { basicToken(VOID) }
while { basicToken(WHILE) }
class { basicToken(CLASS) }
bool { basicToken(BOOL) }


= { basicToken(ASSIGN) }
: { basicToken(COLON) }
, { basicToken(COMMA) }
"+" { basicToken(CROSS) }
- { basicToken(DASH) }
== { basicToken(EQUALS) }
> { basicToken(GREATER) }
>= { basicToken(GREATEREQ) }
"{" { basicToken(LCURLY) }
"<" { basicToken(LESS) }
"<=" { basicToken(LESSEQ) }
"(" { basicToken(LPAREN) }
! { basicToken(NOT) }
!= { basicToken(NOTEQUALS) }
-- { basicToken(POSTDEC) }
"++" { basicToken(POSTINC) }
"}" { basicToken(RCURLY) }
")" { basicToken(RPAREN) }
; { basicToken(SEMICOL) }
"/" { basicToken(SLASH) }
"*" { basicToken(STAR) }

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

[//].* { // ignore comment
       }

\" { BEGIN(IN_STR);}

<IN_STR>{STRING}\n { 
    BEGIN(INITIAL);
    int offset = strlen(yytext);
    std::string msg = "Unterminated string literal detected";
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+offset);
		Report::fatal(pos,msg);
		colNum = 1;
    lineNum++;
    }

<IN_STR>{STRING}(\\[^nt\"\\])+{STRING}\" {
  BEGIN(INITIAL);
  int offset = strlen(yytext)+1;
  std::string msg = "String literal with bad escape sequence detected";
  Position * pos = new Position(
  lineNum, colNum, lineNum, colNum+offset);
  Report::fatal(pos,msg);
  colNum += offset;
  }

<IN_STR>{STRING}(\\[^nt\"\\])+{STRING}\n {
  BEGIN(INITIAL);
  int offset = strlen(yytext);
  std::string msg = "Unterminated string literal with bad escaped sequence detected";
  Position * pos = new Position(
  lineNum, colNum, lineNum, colNum+offset);
  Report::fatal(pos,msg);
  colNum = 1;
  lineNum++;
  }


<IN_STR>{STRING}\" {
  BEGIN(INITIAL);
  int offset = colNum+strlen(yytext);
  std::string stringVal = std::string("\"") + yytext;
    Position * pos = new Position(
      lineNum, colNum, lineNum, offset);
    yylval->lexeme = new StrToken(pos, stringVal);
    colNum += offset;
    return TokenKind::STRINGLITERAL;
}

[\t ] { colNum++; }

\n	{ //{{{2
		//Update the line number, reset the columnNumber
		lineNum++;
		colNum = 1;
		} //2}}}


.   { //{{{2
		std::string msg = "Illegal character ";
		msg += yytext;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		Report::fatal(pos,msg);
		colNum += strlen(yytext);
    } //2}}}
%%
