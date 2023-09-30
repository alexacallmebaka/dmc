// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "frontend.hh"


// Unqualified %code blocks.
#line 32 "adair.yy"

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

#line 61 "parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "adair.yy"
namespace drewno_mars {
#line 135 "parser.cc"

  /// Build a parser object.
  Parser::Parser (drewno_mars::Scanner &scanner_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}


  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: globals
#line 138 "adair.yy"
                          {
						//For the project, we will only be checking std::cerr for 
						// correctness. You might choose to uncomment the following
						// Line to help you debug, which will print when this
						// production is applied
						std::cout << "got to the program ::= globals rule\n";
						}
#line 598 "parser.cc"
    break;

  case 3: // globals: globals decl
#line 146 "adair.yy"
                               {std::cout << "got to the globals ::= decl rule\n"; }
#line 604 "parser.cc"
    break;

  case 4: // globals: %empty
#line 147 "adair.yy"
                                        {std::cout << "got to the program ::= epsilon rule\n"; }
#line 610 "parser.cc"
    break;

  case 5: // decl: varDecl SEMICOL
#line 149 "adair.yy"
                                  {std::cout << "got to the varDecl ::= SEMICOL rule\n"; }
#line 616 "parser.cc"
    break;

  case 6: // decl: classDecl
#line 150 "adair.yy"
                                    {std::cout << "got to the varDecl ::= classDecl rule\n"; }
#line 622 "parser.cc"
    break;

  case 7: // decl: fnDecl
#line 151 "adair.yy"
                                 {std::cout << "got to the varDecl ::= fnDecl rule\n"; }
#line 628 "parser.cc"
    break;

  case 8: // varDecl: id COLON type
#line 153 "adair.yy"
                                { std::cout << "got to the varDecl ::= id rule\n"; }
#line 634 "parser.cc"
    break;

  case 9: // varDecl: id COLON type ASSIGN exp
#line 154 "adair.yy"
                                                   {std::cout << "got to the varDecl ::= ASSIGN rule\n"; }
#line 640 "parser.cc"
    break;

  case 10: // type: primType
#line 156 "adair.yy"
                           {std::cout << "got to the type ::= primType rule\n"; }
#line 646 "parser.cc"
    break;

  case 11: // type: PERFECT primType
#line 157 "adair.yy"
                                           {std::cout << "got to the type ::= PERFECT primType rule\n"; }
#line 652 "parser.cc"
    break;

  case 12: // type: id
#line 158 "adair.yy"
                             {std::cout << "got to the type ::= id rule\n"; }
#line 658 "parser.cc"
    break;

  case 13: // type: PERFECT id
#line 159 "adair.yy"
                                     {std::cout << "got to the type ::= PERFECT id rule\n"; }
#line 664 "parser.cc"
    break;

  case 14: // primType: INT
#line 161 "adair.yy"
                      {std::cout << "got to the primType ::= INT rule\n"; }
#line 670 "parser.cc"
    break;

  case 15: // primType: BOOL
#line 162 "adair.yy"
                               {std::cout << "got to the primType ::= BOOL rule\n"; }
#line 676 "parser.cc"
    break;

  case 16: // primType: VOID
#line 163 "adair.yy"
                               {std::cout << "got to the primType ::= VOID rule\n"; }
#line 682 "parser.cc"
    break;

  case 17: // classDecl: id COLON CLASS LCURLY classBody RCURLY SEMICOL
#line 165 "adair.yy"
                                                                 {std::cout << "got to the classDecl ::= classBody rule\n"; }
#line 688 "parser.cc"
    break;

  case 18: // classBody: classBody varDecl
#line 167 "adair.yy"
                                    {std::cout << "got to the classBody ::= varDecl rule\n"; }
#line 694 "parser.cc"
    break;

  case 19: // classBody: classBody fnDecl
#line 168 "adair.yy"
                                           {std::cout << "got to the classBody ::= fnDecl rule\n"; }
#line 700 "parser.cc"
    break;

  case 20: // classBody: %empty
#line 169 "adair.yy"
                                        {std::cout << "got to the classBody ::= epsilon rule\n"; }
#line 706 "parser.cc"
    break;

  case 21: // fnDecl: id COLON LPAREN formals RPAREN type LCURLY stmtList RCURLY
#line 171 "adair.yy"
                                                                             {std::cout << "got to the fnDecl ::= formals rule\n"; }
#line 712 "parser.cc"
    break;

  case 22: // fnDecl: id COLON LPAREN RPAREN type LCURLY stmtList RCURLY
#line 172 "adair.yy"
                                                                              {std::cout << "got to the fnDecl ::= lpar rpar rule\n"; }
#line 718 "parser.cc"
    break;

  case 23: // formals: formalsList
#line 174 "adair.yy"
                              {std::cout << "got to the formals ::= formalsList rule\n"; }
#line 724 "parser.cc"
    break;

  case 24: // formalsList: formalDecl
#line 176 "adair.yy"
                             {std::cout << "got to the formalsList ::= formalDecl rule\n"; }
#line 730 "parser.cc"
    break;

  case 25: // formalsList: formalsList COMMA formalDecl
#line 177 "adair.yy"
                                                       {std::cout << "got to the formalsList ::= COMMA rule\n"; }
#line 736 "parser.cc"
    break;

  case 26: // formalDecl: id COLON type
#line 179 "adair.yy"
                                {std::cout << "got to the formalDecl ::= id COLON type rule\n"; }
#line 742 "parser.cc"
    break;

  case 27: // stmtList: stmtList stmt SEMICOL
#line 181 "adair.yy"
                                        {std::cout << "got to the stmtList ::= SEMICOL rule\n"; }
#line 748 "parser.cc"
    break;

  case 28: // stmtList: stmtList blockStmt
#line 182 "adair.yy"
                                             {std::cout << "got to the stmtList ::= blockStmt rule\n"; }
#line 754 "parser.cc"
    break;

  case 29: // stmtList: %empty
#line 183 "adair.yy"
                                        {std::cout << "got to the stmtList ::= epsilon rule\n"; }
#line 760 "parser.cc"
    break;

  case 30: // blockStmt: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
#line 185 "adair.yy"
                                                                 {std::cout << "got to the blockStmt ::= WHILE rule\n"; }
#line 766 "parser.cc"
    break;

  case 31: // blockStmt: IF LPAREN exp RPAREN LCURLY stmtList RCURLY
#line 186 "adair.yy"
                                                                      {std::cout << "got to the blockStmt ::= if rule\n"; }
#line 772 "parser.cc"
    break;

  case 32: // blockStmt: IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
#line 187 "adair.yy"
                                                                                                  {std::cout << "got to the blockStmt ::= else rule\n"; }
#line 778 "parser.cc"
    break;

  case 33: // stmt: varDecl
#line 189 "adair.yy"
                          {std::cout << "got to the stmt ::= varDecl rule\n"; }
#line 784 "parser.cc"
    break;

  case 34: // stmt: loc ASSIGN exp
#line 190 "adair.yy"
                                         {std::cout << "got to the stmt ::= ASSIGN rule\n"; }
#line 790 "parser.cc"
    break;

  case 35: // stmt: loc POSTDEC
#line 191 "adair.yy"
                                      {std::cout << "got to the stmt ::= POSTDEC rule\n"; }
#line 796 "parser.cc"
    break;

  case 36: // stmt: GIVE exp
#line 192 "adair.yy"
                                   {std::cout << "got to the stmt ::= GIVE rule\n"; }
#line 802 "parser.cc"
    break;

  case 37: // stmt: TAKE loc
#line 193 "adair.yy"
                                   {std::cout << "got to the stmt ::= TAKE rule\n"; }
#line 808 "parser.cc"
    break;

  case 38: // stmt: RETURN exp
#line 194 "adair.yy"
                                     {std::cout << "got to the stmt ::= RETURN exp rule\n"; }
#line 814 "parser.cc"
    break;

  case 39: // stmt: RETURN
#line 195 "adair.yy"
                                 {std::cout << "got to the stmt ::= RETURN rule\n"; }
#line 820 "parser.cc"
    break;

  case 40: // stmt: EXIT
#line 196 "adair.yy"
                               {std::cout << "got to the stmt ::= EXIT rule\n"; }
#line 826 "parser.cc"
    break;

  case 41: // stmt: callExp
#line 197 "adair.yy"
                                  {std::cout << "got to the stmt ::= callExp rule\n"; }
#line 832 "parser.cc"
    break;

  case 42: // exp: exp DASH exp
#line 199 "adair.yy"
                           {std::cout << "got to the exp ::= DASH rule\n"; }
#line 838 "parser.cc"
    break;

  case 43: // exp: exp CROSS exp
#line 200 "adair.yy"
                                {std::cout << "got to the exp ::= CROSS rule\n"; }
#line 844 "parser.cc"
    break;

  case 44: // exp: exp STAR exp
#line 201 "adair.yy"
                           {std::cout << "got to the exp ::= STAR rule\n"; }
#line 850 "parser.cc"
    break;

  case 45: // exp: exp SLASH exp
#line 202 "adair.yy"
                            {std::cout << "got to the exp ::= SLASH rule\n"; }
#line 856 "parser.cc"
    break;

  case 46: // exp: exp AND exp
#line 203 "adair.yy"
                          {std::cout << "got to the exp ::= AND rule\n"; }
#line 862 "parser.cc"
    break;

  case 47: // exp: exp OR exp
#line 204 "adair.yy"
                         {std::cout << "got to the exp ::= OR rule\n"; }
#line 868 "parser.cc"
    break;

  case 48: // exp: exp EQUALS exp
#line 205 "adair.yy"
                             {std::cout << "got to the exp ::= EQUALS rule\n"; }
#line 874 "parser.cc"
    break;

  case 49: // exp: exp NOTEQUALS exp
#line 206 "adair.yy"
                                {std::cout << "got to the exp ::= NOTEQUALS rule\n"; }
#line 880 "parser.cc"
    break;

  case 50: // exp: exp GREATER exp
#line 207 "adair.yy"
                              {std::cout << "got to the exp ::= GREATER rule\n"; }
#line 886 "parser.cc"
    break;

  case 51: // exp: exp GREATEREQ exp
#line 208 "adair.yy"
                                {std::cout << "got to the exp ::= GREATEREQ rule\n"; }
#line 892 "parser.cc"
    break;

  case 52: // exp: exp LESS exp
#line 209 "adair.yy"
                           {std::cout << "got to the exp ::= LESS rule\n"; }
#line 898 "parser.cc"
    break;

  case 53: // exp: exp LESSEQ exp
#line 210 "adair.yy"
                             {std::cout << "got to the exp ::= LESSEQ rule\n"; }
#line 904 "parser.cc"
    break;

  case 54: // exp: NOT exp
#line 211 "adair.yy"
                      {std::cout << "got to the exp ::= NOT rule\n"; }
#line 910 "parser.cc"
    break;

  case 55: // exp: DASH term
#line 212 "adair.yy"
                            {std::cout << "got to the exp ::= DASH term rule\n"; }
#line 916 "parser.cc"
    break;

  case 56: // exp: term
#line 213 "adair.yy"
                   {std::cout << "got to the exp ::= term rule\n"; }
#line 922 "parser.cc"
    break;

  case 57: // callExp: id LPAREN RPAREN
#line 215 "adair.yy"
                               {std::cout << "got to the callExp ::= lpar rpar rule\n"; }
#line 928 "parser.cc"
    break;

  case 58: // callExp: id LPAREN actualsList RPAREN
#line 216 "adair.yy"
                                           {std::cout << "got to the callExp ::= actualsList rule\n"; }
#line 934 "parser.cc"
    break;

  case 59: // actualsList: exp
#line 218 "adair.yy"
                  {std::cout << "got to the actualsList ::= exp rule\n"; }
#line 940 "parser.cc"
    break;

  case 60: // actualsList: actualsList COMMA exp
#line 219 "adair.yy"
                                    {std::cout << "got to the actualsList ::= COMMA rule\n"; }
#line 946 "parser.cc"
    break;

  case 61: // term: loc
#line 221 "adair.yy"
                  {std::cout << "got to the term ::= loc rule\n"; }
#line 952 "parser.cc"
    break;

  case 62: // term: INTLITERAL
#line 222 "adair.yy"
                         {std::cout << "got to the term ::= INTLITERAL rule\n"; }
#line 958 "parser.cc"
    break;

  case 63: // term: STRINGLITERAL
#line 223 "adair.yy"
                            {std::cout << "got to the term ::= STRINGLITERAL rule\n"; }
#line 964 "parser.cc"
    break;

  case 64: // term: TRUE
#line 224 "adair.yy"
                   {std::cout << "got to the term ::= TRUE rule\n"; }
#line 970 "parser.cc"
    break;

  case 65: // term: FALSE
#line 225 "adair.yy"
                    {std::cout << "got to the term ::= FALSE rule\n"; }
#line 976 "parser.cc"
    break;

  case 66: // term: MAGIC
#line 226 "adair.yy"
                    {std::cout << "got to the term ::= MAGIC rule\n"; }
#line 982 "parser.cc"
    break;

  case 67: // term: LPAREN exp RPAREN
#line 227 "adair.yy"
                                {std::cout << "got to the term ::= exp rule\n"; }
#line 988 "parser.cc"
    break;

  case 68: // term: callExp
#line 228 "adair.yy"
                      {std::cout << "got to the term ::= callExp rule\n"; }
#line 994 "parser.cc"
    break;

  case 69: // loc: id
#line 230 "adair.yy"
                             {std::cout << "got to the loc ::= id rule\n"; }
#line 1000 "parser.cc"
    break;

  case 70: // loc: loc POSTDEC id
#line 231 "adair.yy"
                                         {std::cout << "got to the loc ::= POSTDEC rule\n"; }
#line 1006 "parser.cc"
    break;

  case 71: // id: ID
#line 233 "adair.yy"
                             {std::cout << "got to the id ::= ID rule\n"; }
#line 1012 "parser.cc"
    break;


#line 1016 "parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // Parser::context.
  Parser::context::context (const Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -75;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     -75,    16,     1,   -75,   -75,   -75,    17,   -75,   -75,    56,
     -75,    76,   -75,    46,   -75,   -15,    10,   -75,    60,   -75,
     -75,   -75,     7,    32,    63,   -75,    65,   -75,   -75,   206,
     -13,    57,     7,     1,     7,   338,   -75,   -75,   206,   -75,
     206,   -75,   -75,   213,   -75,   -75,    51,    64,    52,   -75,
     -75,    85,   -75,    71,   -75,   -75,   -75,   120,   -75,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,     1,   144,   -75,    94,    68,   -75,   -75,   293,    83,
     314,   314,   314,   314,   314,   314,   334,   -28,   -75,    66,
     -75,   -75,   213,     0,   -75,   206,    74,   206,   -75,     1,
      77,   -75,   -75,    70,   -75,     2,    -1,   236,   206,   -75,
     213,   206,   213,    51,   -75,   206,   -75,   206,     1,     7,
     -75,   213,   151,   182,   213,    87,    89,   -75,   -75,   244,
     249,   105,   -75,    99,   -75,   278,   -75
  };

  const signed char
  Parser::yydefact_[] =
  {
       4,     0,     2,     1,    71,     3,     0,     6,     7,     0,
       5,     0,    15,     0,    14,     0,     0,    16,     8,    10,
      12,    20,     0,     0,    23,    24,     0,    11,    13,     0,
       0,     0,     0,     0,     0,     0,    65,    62,     0,    66,
       0,    63,    64,     9,    68,    56,    61,    69,     0,    18,
      19,     0,    29,     0,    25,    26,    55,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    17,     0,     0,    29,    67,    46,    42,
      48,    50,    51,    52,    53,    49,    47,    43,    45,    44,
      70,    57,    59,     0,    40,     0,     0,    39,    22,     0,
       0,    33,    28,     0,    41,     0,    69,     0,     0,    58,
      36,     0,    38,    37,    69,     0,    27,     0,    35,     0,
      21,    60,     0,     0,    34,     0,     0,    29,    29,     0,
       0,    31,    30,     0,    29,     0,    32
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -75,   -75,   -75,   -75,    50,    43,   109,   -75,   -75,    96,
     -75,   -75,    97,   -68,   -75,   -75,   -21,   -72,   -75,   103,
     -74,    -2
  };

  const signed char
  Parser::yydefgoto_[] =
  {
       0,     1,     2,     5,   101,    18,    19,     7,    30,     8,
      23,    24,    25,    75,   102,   103,    43,    44,    93,    45,
      46,    47
  };

  const short
  Parser::yytable_[] =
  {
       9,   105,     4,   104,     4,   119,   117,   108,   107,    20,
      69,    70,    12,    26,    28,    12,     3,    57,     4,    58,
      20,    22,    48,    72,     4,   113,    14,     4,    51,    14,
      20,    26,    20,   105,   118,   104,   109,    16,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      17,    92,     6,    17,    10,   105,   105,   104,   104,   129,
     130,   105,    11,   104,    29,    31,   135,    21,    32,    90,
      33,    34,    20,   106,   110,    53,   112,    55,    52,    94,
      49,    12,    95,    71,    13,     4,    96,   121,    72,    73,
     122,    74,    76,     4,   123,    14,   124,   114,   111,    12,
      15,   115,    97,    98,    69,   106,    16,   116,   127,    99,
     128,     4,   100,    14,    68,   133,    90,    20,    15,    17,
     134,    69,    70,    59,    16,    27,    50,   106,   106,    60,
      54,     0,    61,   106,     0,    62,    63,    17,    56,     0,
       0,     0,    64,    65,     0,     0,     0,    66,    67,     0,
       0,    68,     0,    35,    59,     0,    77,    36,    69,    70,
      60,     4,     0,    61,    37,     0,    62,    63,    38,    39,
      40,     0,     0,    64,    65,     0,     0,     0,    66,    67,
      91,     0,    68,     0,    41,    59,    42,   125,     0,    69,
      70,    60,     0,     0,    61,     0,     0,    62,    63,     0,
       0,     0,     0,     0,    64,    65,     0,     0,     0,    66,
      67,     0,     0,    68,     0,    35,    59,     0,   126,    36,
      69,    70,    60,     4,     0,    61,    37,     0,    62,    63,
      38,    39,    40,     0,     0,    64,    65,     0,     0,     0,
      66,    67,     0,     0,    68,     0,    41,    94,    42,     0,
      95,    69,    70,     4,    96,    94,     0,     0,    95,     0,
      94,     4,    96,    95,     0,     0,     4,    96,     0,     0,
      97,   120,     0,     0,     0,     0,     0,    99,    97,   131,
     100,     0,     0,    97,   132,    99,     0,     0,   100,    94,
      99,     0,    95,   100,     0,     4,    96,     0,     0,     0,
       0,     0,    60,     0,     0,    61,     0,     0,    62,    63,
       0,     0,    97,   136,     0,    64,    65,     0,     0,    99,
      66,    67,   100,    60,    68,     0,    -1,     0,     0,    -1,
      -1,    69,    70,     0,     0,     0,    -1,    -1,     0,     0,
       0,    -1,     0,    60,     0,    68,    61,     0,     0,    62,
      63,    36,    69,    70,     0,     4,    64,    65,    37,     0,
       0,    66,    38,    39,     0,    68,     0,     0,     0,     0,
       0,     0,    69,    70,     0,     0,     0,     0,    41,     0,
      42
  };

  const short
  Parser::yycheck_[] =
  {
       2,    75,    17,    75,    17,     6,     4,     7,    76,    11,
      38,    39,     5,    15,    16,     5,     0,    38,    17,    40,
      22,    36,    35,    24,    17,    99,    19,    17,    30,    19,
      32,    33,    34,   107,    32,   107,    36,    30,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      43,    72,     2,    43,    37,   129,   130,   129,   130,   127,
     128,   135,     6,   135,     4,    22,   134,    21,    36,    71,
       7,     6,    74,    75,    95,    32,    97,    34,    21,    11,
      30,     5,    14,    32,     8,    17,    18,   108,    24,    37,
     111,     6,    21,    17,   115,    19,   117,    99,    24,     5,
      24,    24,    34,    35,    38,   107,    30,    37,    21,    41,
      21,    17,    44,    19,    31,    10,   118,   119,    24,    43,
      21,    38,    39,     3,    30,    16,    30,   129,   130,     9,
      33,    -1,    12,   135,    -1,    15,    16,    43,    35,    -1,
      -1,    -1,    22,    23,    -1,    -1,    -1,    27,    28,    -1,
      -1,    31,    -1,     9,     3,    -1,    36,    13,    38,    39,
       9,    17,    -1,    12,    20,    -1,    15,    16,    24,    25,
      26,    -1,    -1,    22,    23,    -1,    -1,    -1,    27,    28,
      36,    -1,    31,    -1,    40,     3,    42,    36,    -1,    38,
      39,     9,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    27,
      28,    -1,    -1,    31,    -1,     9,     3,    -1,    36,    13,
      38,    39,     9,    17,    -1,    12,    20,    -1,    15,    16,
      24,    25,    26,    -1,    -1,    22,    23,    -1,    -1,    -1,
      27,    28,    -1,    -1,    31,    -1,    40,    11,    42,    -1,
      14,    38,    39,    17,    18,    11,    -1,    -1,    14,    -1,
      11,    17,    18,    14,    -1,    -1,    17,    18,    -1,    -1,
      34,    35,    -1,    -1,    -1,    -1,    -1,    41,    34,    35,
      44,    -1,    -1,    34,    35,    41,    -1,    -1,    44,    11,
      41,    -1,    14,    44,    -1,    17,    18,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    34,    35,    -1,    22,    23,    -1,    -1,    41,
      27,    28,    44,     9,    31,    -1,    12,    -1,    -1,    15,
      16,    38,    39,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    27,    -1,     9,    -1,    31,    12,    -1,    -1,    15,
      16,    13,    38,    39,    -1,    17,    22,    23,    20,    -1,
      -1,    27,    24,    25,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    40,    -1,
      42
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    46,    47,     0,    17,    48,    49,    52,    54,    66,
      37,     6,     5,     8,    19,    24,    30,    43,    50,    51,
      66,    21,    36,    55,    56,    57,    66,    51,    66,     4,
      53,    50,    36,     7,     6,     9,    13,    20,    24,    25,
      26,    40,    42,    61,    62,    64,    65,    66,    35,    49,
      54,    66,    21,    50,    57,    50,    64,    61,    61,     3,
       9,    12,    15,    16,    22,    23,    27,    28,    31,    38,
      39,    32,    24,    37,     6,    58,    21,    36,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      66,    36,    61,    63,    11,    14,    18,    34,    35,    41,
      44,    49,    59,    60,    62,    65,    66,    58,     7,    36,
      61,    24,    61,    65,    66,    24,    37,     4,    32,     6,
      35,    61,    61,    61,    61,    36,    36,    21,    21,    58,
      58,    35,    35,    10,    21,    58,    35
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    45,    46,    47,    47,    48,    48,    48,    49,    49,
      50,    50,    50,    50,    51,    51,    51,    52,    53,    53,
      53,    54,    54,    55,    56,    56,    57,    58,    58,    58,
      59,    59,    59,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    62,    62,    63,
      63,    64,    64,    64,    64,    64,    64,    64,    64,    65,
      65,    66
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     2,     1,     1,     3,     5,
       1,     2,     1,     2,     1,     1,     1,     7,     2,     2,
       0,     9,     8,     1,     1,     3,     3,     3,     2,     0,
       7,     7,    11,     1,     3,     2,     2,     2,     2,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     1,     3,     4,     1,
       3,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       3,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end file\"", "error", "\"invalid token\"", "AND", "ASSIGN", "BOOL",
  "COLON", "COMMA", "CLASS", "DASH", "ELSE", "EXIT", "EQUALS", "FALSE",
  "GIVE", "GREATER", "GREATEREQ", "ID", "IF", "INT", "INTLITERAL",
  "LCURLY", "LESS", "LESSEQ", "LPAREN", "MAGIC", "NOT", "NOTEQUALS", "OR",
  "OPEN", "PERFECT", "CROSS", "POSTDEC", "POSTINC", "RETURN", "RCURLY",
  "RPAREN", "SEMICOL", "SLASH", "STAR", "STRINGLITERAL", "TAKE", "TRUE",
  "VOID", "WHILE", "$accept", "program", "globals", "decl", "varDecl",
  "type", "primType", "classDecl", "classBody", "fnDecl", "formals",
  "formalsList", "formalDecl", "stmtList", "blockStmt", "stmt", "exp",
  "callExp", "actualsList", "term", "loc", "id", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,   138,   138,   146,   147,   149,   150,   151,   153,   154,
     156,   157,   158,   159,   161,   162,   163,   165,   167,   168,
     169,   171,   172,   174,   176,   177,   179,   181,   182,   183,
     185,   186,   187,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   215,   216,   218,
     219,   221,   222,   223,   224,   225,   226,   227,   228,   230,
     231,   233
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
    };
    // Last valid token kind.
    const int code_max = 299;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 5 "adair.yy"
} // drewno_mars
#line 1670 "parser.cc"

#line 235 "adair.yy"


void drewno_mars::Parser::error(const std::string& msg){
	//For the project, we will only be checking std::cerr for 
	// correctness. You might choose to uncomment the following
	// Line to help you debug, since it gives slightly more 
	// descriptive error messages 
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
