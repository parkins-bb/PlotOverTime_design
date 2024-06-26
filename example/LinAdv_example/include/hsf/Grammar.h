// 类别  :  %Internal File% ( Don't delete this line )
/* A Bison parser, made by GNU Bison 1.875c.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_AND = 258,
     T_ASSIGN = 259,
     T_CHAR = 260,
     T_CLOSE_CURLY = 261,
     T_CLOSE_PAREN = 262,
     T_CLOSE_SQBKT = 263,
     T_COMMA = 264,
     T_DIV = 265,
     T_DOUBLE = 266,
     T_ELSE = 267,
     T_EXP = 268,
     T_EQUALS = 269,
     T_GREATER_EQUALS = 270,
     T_GREATER = 271,
     T_LESS_EQUALS = 272,
     T_LESS = 273,
     T_FALSE = 274,
     T_INTEGER = 275,
     T_KEYWORD = 276,
     T_MINUS = 277,
     T_MULT = 278,
     T_NOT = 279,
     T_NOT_EQUALS = 280,
     T_OR = 281,
     T_OPEN_CURLY = 282,
     T_OPEN_PAREN = 283,
     T_OPEN_SQBKT = 284,
     T_PLUS = 285,
     T_QUESTION = 286,
     T_SEMI = 287,
     T_STRING = 288,
     T_TRUE = 289,
     T_NEGATION = 290
   };
#endif
#define T_AND 258
#define T_ASSIGN 259
#define T_CHAR 260
#define T_CLOSE_CURLY 261
#define T_CLOSE_PAREN 262
#define T_CLOSE_SQBKT 263
#define T_COMMA 264
#define T_DIV 265
#define T_DOUBLE 266
#define T_ELSE 267
#define T_EXP 268
#define T_EQUALS 269
#define T_GREATER_EQUALS 270
#define T_GREATER 271
#define T_LESS_EQUALS 272
#define T_LESS 273
#define T_FALSE 274
#define T_INTEGER 275
#define T_KEYWORD 276
#define T_MINUS 277
#define T_MULT 278
#define T_NOT 279
#define T_NOT_EQUALS 280
#define T_OR 281
#define T_OPEN_CURLY 282
#define T_OPEN_PAREN 283
#define T_OPEN_SQBKT 284
#define T_PLUS 285
#define T_QUESTION 286
#define T_SEMI 287
#define T_STRING 288
#define T_TRUE 289
#define T_NEGATION 290




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 89 "Grammar.y"
typedef union YYSTYPE {
  char          u_char;
  double        u_double;
  int           u_integer;
  KeyData* u_keydata;
  string*       u_keyword;
  string*       u_string;
} YYSTYPE;
/* Line 1268 of yacc.c.  */
#line 116 "Grammar.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
///@endcond


