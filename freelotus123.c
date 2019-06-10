
///////////////////
// Freelotus123
///////////////////

#include <stdio.h>
#if defined(__linux__)
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__)
#define MYOS 4
#define PATH_MAX 2500
#else
#define MYOS 0
#endif

#define VERSIONNBR "0.14" 

/// max lines of a given file
#define CELLYMAXY 101 
#define CELLYMAXX 18

#define CELLXSPACE 11
int savecellymaxy;
int savecellymaxx;

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
int rows, cols; 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ncurses.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

#include <time.h>
#include <math.h>

// #include <string.h>
// #include <stdlib.h>
// #include <dirent.h>
// #include <ctype.h>
#include <sys/stat.h>
// #include <dirent.h>
// #include <sys/types.h>
// #include <unistd.h> 
// #include <time.h>

 char clipcell[PATH_MAX];
 char freelotus123_statusline[PATH_MAX];

 char filesource[PATH_MAX];
 char debugclip[PATH_MAX];
 char clipboard[PATH_MAX];
 int clipboardtype = 0; 
 char celldata[ CELLYMAXY+10 ][ CELLYMAXX+10  ][PATH_MAX];
 int celldatatype[ CELLYMAXY+10  ][ CELLYMAXX+10 ];
 char currentcellline[PATH_MAX];
 int user_scrolly = 0; 
 int user_col_space   = CELLXSPACE;
 int user_col_charmax = CELLXSPACE;




void nsystem( char *mycmd )
{
       def_prog_mode();
       endwin();

       printf( "<SYSTEM>\n" );
       printf( " >> CMD:%s\n", mycmd );
       system( mycmd );
       printf( "</SYSTEM>\n");

       reset_prog_mode();
}




/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}





int filelinecount( char *filesource )
{
  FILE *fp;
  FILE *fp1; 
  FILE *fp2;
  int counter = 0 ; 
  int freader = 1 ; 
  int i , j ,posy, posx ; 
  char linetmp[PATH_MAX] ;
  char line[PATH_MAX];
  char foundline[PATH_MAX] ;
  strncpy( foundline , "" , PATH_MAX );
  if ( fexist( filesource ) == 1 )
  {
        fp1 = fopen( filesource, "rb");
        counter = 0; 
	freader = 1;
        while( !feof(fp1) && ( freader == 1)   ) {
          strncpy( linetmp, "", PATH_MAX );
          fgets(linetmp, PATH_MAX, fp1); 
              //strncpy(line, binstrrlf( linetmp ), PATH_MAX );
              if ( !feof(fp1) )
              {
                  counter++;
	      }
         }
        fclose(fp1);
    }
   return counter;
   //return counter-1;
}

















//////////////////////////////////////////////
// start of lib for maths
//////////////////////////////////////////////
//////////////////////////////////////////////
#ifndef __TINYEXPR_H__
#define __TINYEXPR_H__

#ifdef __cplusplus
extern "C" {
#endif



typedef struct te_expr {
    int type;
    union {double value; const double *bound; const void *function;};
    void *parameters[1];
} te_expr;


enum {
    TE_VARIABLE = 0,

    TE_FUNCTION0 = 8, TE_FUNCTION1, TE_FUNCTION2, TE_FUNCTION3,
    TE_FUNCTION4, TE_FUNCTION5, TE_FUNCTION6, TE_FUNCTION7,

    TE_CLOSURE0 = 16, TE_CLOSURE1, TE_CLOSURE2, TE_CLOSURE3,
    TE_CLOSURE4, TE_CLOSURE5, TE_CLOSURE6, TE_CLOSURE7,

    TE_FLAG_PURE = 32
};

typedef struct te_variable {
    const char *name;
    const void *address;
    int type;
    void *context;
} te_variable;



/* Parses the input expression, evaluates it, and frees it. */
/* Returns NaN on error. */
double te_interp(const char *expression, int *error);

/* Parses the input expression and binds variables. */
/* Returns NULL on error. */
te_expr *te_compile(const char *expression, const te_variable *variables, int var_count, int *error);

/* Evaluates the expression. */
double te_eval(const te_expr *n);

/* Prints debugging information on the syntax tree. */
void te_print(const te_expr *n);

/* Frees the expression. */
/* This is safe to call on NULL pointers. */
void te_free(te_expr *n);


#ifdef __cplusplus
}
#endif

#endif /*__TINYEXPR_H__*/







//#include "te.c"
/*
 * TINYEXPR - Tiny recursive descent parser and evaluation engine in C
 *
 * Copyright (c) 2015, 2016 Lewis Van Winkle
 *
 * http://CodePlea.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgement in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/* COMPILE TIME OPTIONS */

/* Exponentiation associativity:
For a^b^c = (a^b)^c and -a^b = (-a)^b do nothing.
For a^b^c = a^(b^c) and -a^b = -(a^b) uncomment the next line.*/
/* #define TE_POW_FROM_RIGHT */

/* Logarithms
For log = base 10 log do nothing
For log = natural log uncomment the next line. */
/* #define TE_NAT_LOG */

//#include "tinyexpr.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#ifndef NAN
#define NAN (0.0/0.0)
#endif

#ifndef INFINITY
#define INFINITY (1.0/0.0)
#endif


typedef double (*te_fun2)(double, double);

enum {
    TOK_NULL = TE_CLOSURE7+1, TOK_ERROR, TOK_END, TOK_SEP,
    TOK_OPEN, TOK_CLOSE, TOK_NUMBER, TOK_VARIABLE, TOK_INFIX
};


enum {TE_CONSTANT = 1};


typedef struct state {
    const char *start;
    const char *next;
    int type;
    union {double value; const double *bound; const void *function;};
    void *context;

    const te_variable *lookup;
    int lookup_len;
} state;


#define TYPE_MASK(TYPE) ((TYPE)&0x0000001F)

#define IS_PURE(TYPE) (((TYPE) & TE_FLAG_PURE) != 0)
#define IS_FUNCTION(TYPE) (((TYPE) & TE_FUNCTION0) != 0)
#define IS_CLOSURE(TYPE) (((TYPE) & TE_CLOSURE0) != 0)
#define ARITY(TYPE) ( ((TYPE) & (TE_FUNCTION0 | TE_CLOSURE0)) ? ((TYPE) & 0x00000007) : 0 )
#define NEW_EXPR(type, ...) new_expr((type), (const te_expr*[]){__VA_ARGS__})

static te_expr *new_expr(const int type, const te_expr *parameters[]) 
{
    const int arity = ARITY(type);
    const int psize = sizeof(void*) * arity;
    const int size = (sizeof(te_expr) - sizeof(void*)) + psize + (IS_CLOSURE(type) ? sizeof(void*) : 0);
    te_expr *ret = malloc(size);
    memset(ret, 0, size);
    if (arity && parameters) {
        memcpy(ret->parameters, parameters, psize);
    }
    ret->type = type;
    ret->bound = 0;
    return ret;
}


void te_free_parameters(te_expr *n) {
    if (!n) return;
    switch (TYPE_MASK(n->type)) {
        case TE_FUNCTION7: case TE_CLOSURE7: te_free(n->parameters[6]);
        case TE_FUNCTION6: case TE_CLOSURE6: te_free(n->parameters[5]);
        case TE_FUNCTION5: case TE_CLOSURE5: te_free(n->parameters[4]);
        case TE_FUNCTION4: case TE_CLOSURE4: te_free(n->parameters[3]);
        case TE_FUNCTION3: case TE_CLOSURE3: te_free(n->parameters[2]);
        case TE_FUNCTION2: case TE_CLOSURE2: te_free(n->parameters[1]);
        case TE_FUNCTION1: case TE_CLOSURE1: te_free(n->parameters[0]);
    }
}


void te_free(te_expr *n) {
    if (!n) return;
    te_free_parameters(n);
    free(n);
}


static double pi() {return 3.14159265358979323846;}
static double e() {return 2.71828182845904523536;}
static double fac(double a) {/* simplest version of fac */
    if (a < 0.0)
        return NAN;
    if (a > UINT_MAX)
        return INFINITY;
    unsigned int ua = (unsigned int)(a);
    unsigned long int result = 1, i;
    for (i = 1; i <= ua; i++) {
        if (i > ULONG_MAX / result)
            return INFINITY;
        result *= i;
    }
    return (double)result;
}
static double ncr(double n, double r) {
    if (n < 0.0 || r < 0.0 || n < r) return NAN;
    if (n > UINT_MAX || r > UINT_MAX) return INFINITY;
    unsigned long int un = (unsigned int)(n), ur = (unsigned int)(r), i;
    unsigned long int result = 1;
    if (ur > un / 2) ur = un - ur;
    for (i = 1; i <= ur; i++) {
        if (result > ULONG_MAX / (un - ur + i))
            return INFINITY;
        result *= un - ur + i;
        result /= i;
    }
    return result;
}
static double npr(double n, double r) {return ncr(n, r) * fac(r);}

static const te_variable functions[] = {
    /* must be in alphabetical order */
    {"abs", fabs,     TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"acos", acos,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"asin", asin,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"atan", atan,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"atan2", atan2,  TE_FUNCTION2 | TE_FLAG_PURE, 0},
    {"ceil", ceil,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"cos", cos,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"cosh", cosh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"e", e,          TE_FUNCTION0 | TE_FLAG_PURE, 0},
    {"exp", exp,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"fac", fac,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"floor", floor,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"ln", log,       TE_FUNCTION1 | TE_FLAG_PURE, 0},

#ifdef TE_NAT_LOG
    {"log", log,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
#else
    {"log", log10,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
#endif
    {"log10", log10,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"ncr", ncr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
    {"npr", npr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
    {"pi", pi,        TE_FUNCTION0 | TE_FLAG_PURE, 0},
    {"pow", pow,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
    {"sin", sin,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"sinh", sinh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"sqrt", sqrt,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"tan", tan,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {"tanh", tanh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
    {0, 0, 0, 0}
};

static const te_variable *find_builtin(const char *name, int len) {
    int imin = 0;
    int imax = sizeof(functions) / sizeof(te_variable) - 2;

    /*Binary search.*/
    while (imax >= imin) {
        const int i = (imin + ((imax-imin)/2));
        int c = strncmp(name, functions[i].name, len);
        if (!c) c = '\0' - functions[i].name[len];
        if (c == 0) {
            return functions + i;
        } else if (c > 0) {
            imin = i + 1;
        } else {
            imax = i - 1;
        }
    }

    return 0;
}

static const te_variable *find_lookup(const state *s, const char *name, int len) {
    int iters;
    const te_variable *var;
    if (!s->lookup) return 0;

    for (var = s->lookup, iters = s->lookup_len; iters; ++var, --iters) {
        if (strncmp(name, var->name, len) == 0 && var->name[len] == '\0') {
            return var;
        }
    }
    return 0;
}



static double add(double a, double b) {return a + b;}
static double sub(double a, double b) {return a - b;}
static double mul(double a, double b) {return a * b;}
static double divide(double a, double b) {return a / b;}
static double negate(double a) {return -a;}
static double comma(double a, double b) {(void)a; return b;}


void next_token(state *s) {
    s->type = TOK_NULL;

    do {

        if (!*s->next){
            s->type = TOK_END;
            return;
        }

        /* Try reading a number. */
        if ((s->next[0] >= '0' && s->next[0] <= '9') || s->next[0] == '.') {
            s->value = strtod(s->next, (char**)&s->next);
            s->type = TOK_NUMBER;
        } else {
            /* Look for a variable or builtin function call. */
            if (s->next[0] >= 'a' && s->next[0] <= 'z') {
                const char *start;
                start = s->next;
                while ((s->next[0] >= 'a' && s->next[0] <= 'z') || (s->next[0] >= '0' && s->next[0] <= '9') || (s->next[0] == '_')) s->next++;

                const te_variable *var = find_lookup(s, start, s->next - start);
                if (!var) var = find_builtin(start, s->next - start);

                if (!var) {
                    s->type = TOK_ERROR;
                } else {
                    switch(TYPE_MASK(var->type))
                    {
                        case TE_VARIABLE:
                            s->type = TOK_VARIABLE;
                            s->bound = var->address;
                            break;

                        case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
                        case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
                            s->context = var->context;

                        case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
                        case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
                            s->type = var->type;
                            s->function = var->address;
                            break;
                    }
                }

            } else {
                /* Look for an operator or special character. */
                switch (s->next++[0]) {
                    case '+': s->type = TOK_INFIX; s->function = add; break;
                    case '-': s->type = TOK_INFIX; s->function = sub; break;
                    case '*': s->type = TOK_INFIX; s->function = mul; break;
                    case '/': s->type = TOK_INFIX; s->function = divide; break;
                    case '^': s->type = TOK_INFIX; s->function = pow; break;
                    case '%': s->type = TOK_INFIX; s->function = fmod; break;
                    case '(': s->type = TOK_OPEN; break;
                    case ')': s->type = TOK_CLOSE; break;
                    case ',': s->type = TOK_SEP; break;
                    case ' ': case '\t': case '\n': case '\r': break;
                    default: s->type = TOK_ERROR; break;
                }
            }
        }
    } while (s->type == TOK_NULL);
}



static te_expr *list(state *s);
static te_expr *expr(state *s);
static te_expr *power(state *s);

static te_expr *base(state *s) {
    /* <base>      =    <constant> | <variable> | <function-0> {"(" ")"} | <function-1> <power> | <function-X> "(" <expr> {"," <expr>} ")" | "(" <list> ")" */
    te_expr *ret;
    int arity;

    switch (TYPE_MASK(s->type)) {
        case TOK_NUMBER:
            ret = new_expr(TE_CONSTANT, 0);
            ret->value = s->value;
            next_token(s);
            break;

        case TOK_VARIABLE:
            ret = new_expr(TE_VARIABLE, 0);
            ret->bound = s->bound;
            next_token(s);
            break;

        case TE_FUNCTION0:
        case TE_CLOSURE0:
            ret = new_expr(s->type, 0);
            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[0] = s->context;
            next_token(s);
            if (s->type == TOK_OPEN) {
                next_token(s);
                if (s->type != TOK_CLOSE) {
                    s->type = TOK_ERROR;
                } else {
                    next_token(s);
                }
            }
            break;

        case TE_FUNCTION1:
        case TE_CLOSURE1:
            ret = new_expr(s->type, 0);
            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[1] = s->context;
            next_token(s);
            ret->parameters[0] = power(s);
            break;

        case TE_FUNCTION2: case TE_FUNCTION3: case TE_FUNCTION4:
        case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
        case TE_CLOSURE2: case TE_CLOSURE3: case TE_CLOSURE4:
        case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
            arity = ARITY(s->type);

            ret = new_expr(s->type, 0);
            ret->function = s->function;
            if (IS_CLOSURE(s->type)) ret->parameters[arity] = s->context;
            next_token(s);

            if (s->type != TOK_OPEN) {
                s->type = TOK_ERROR;
            } else {
                int i;
                for(i = 0; i < arity; i++) {
                    next_token(s);
                    ret->parameters[i] = expr(s);
                    if(s->type != TOK_SEP) {
                        break;
                    }
                }
                if(s->type != TOK_CLOSE || i != arity - 1) {
                    s->type = TOK_ERROR;
                } else {
                    next_token(s);
                }
            }

            break;

        case TOK_OPEN:
            next_token(s);
            ret = list(s);
            if (s->type != TOK_CLOSE) {
                s->type = TOK_ERROR;
            } else {
                next_token(s);
            }
            break;

        default:
            ret = new_expr(0, 0);
            s->type = TOK_ERROR;
            ret->value = NAN;
            break;
    }

    return ret;
}





static te_expr *power(state *s) {
    /* <power>     =    {("-" | "+")} <base> */
    int sign = 1;
    while (s->type == TOK_INFIX && (s->function == add || s->function == sub)) {
        if (s->function == sub) sign = -sign;
        next_token(s);
    }

    te_expr *ret;

    if (sign == 1) {
        ret = base(s);
    } else {
        ret = NEW_EXPR(TE_FUNCTION1 | TE_FLAG_PURE, base(s));
        ret->function = negate;
    }

    return ret;
}

#ifdef TE_POW_FROM_RIGHT
static te_expr *factor(state *s) {
    /* <factor>    =    <power> {"^" <power>} */
    te_expr *ret = power(s);

    int neg = 0;
    te_expr *insertion = 0;

    if (ret->type == (TE_FUNCTION1 | TE_FLAG_PURE) && ret->function == negate) {
        te_expr *se = ret->parameters[0];
        free(ret);
        ret = se;
        neg = 1;
    }

    while (s->type == TOK_INFIX && (s->function == pow)) {
        te_fun2 t = s->function;
        next_token(s);

        if (insertion) {
            /* Make exponentiation go right-to-left. */
            te_expr *insert = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, insertion->parameters[1], power(s));
            insert->function = t;
            insertion->parameters[1] = insert;
            insertion = insert;
        } else {
            ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, power(s));
            ret->function = t;
            insertion = ret;
        }
    }

    if (neg) {
        ret = NEW_EXPR(TE_FUNCTION1 | TE_FLAG_PURE, ret);
        ret->function = negate;
    }

    return ret;
}
#else
static te_expr *factor(state *s) {
    /* <factor>    =    <power> {"^" <power>} */
    te_expr *ret = power(s);

    while (s->type == TOK_INFIX && (s->function == pow)) {
        te_fun2 t = s->function;
        next_token(s);
        ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, power(s));
        ret->function = t;
    }

    return ret;
}
#endif



static te_expr *term(state *s) {
    /* <term>      =    <factor> {("*" | "/" | "%") <factor>} */
    te_expr *ret = factor(s);

    while (s->type == TOK_INFIX && (s->function == mul || s->function == divide || s->function == fmod)) {
        te_fun2 t = s->function;
        next_token(s);
        ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, factor(s));
        ret->function = t;
    }

    return ret;
}


static te_expr *expr(state *s) {
    /* <expr>      =    <term> {("+" | "-") <term>} */
    te_expr *ret = term(s);

    while (s->type == TOK_INFIX && (s->function == add || s->function == sub)) {
        te_fun2 t = s->function;
        next_token(s);
        ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, term(s));
        ret->function = t;
    }

    return ret;
}


static te_expr *list(state *s) {
    /* <list>      =    <expr> {"," <expr>} */
    te_expr *ret = expr(s);

    while (s->type == TOK_SEP) {
        next_token(s);
        ret = NEW_EXPR(TE_FUNCTION2 | TE_FLAG_PURE, ret, expr(s));
        ret->function = comma;
    }

    return ret;
}


#define TE_FUN(...) ((double(*)(__VA_ARGS__))n->function)
#define M(e) te_eval(n->parameters[e])


double te_eval(const te_expr *n) 
{
    if (!n) return NAN;

    switch(TYPE_MASK(n->type)) 
    {
        case TE_CONSTANT: return n->value;
        case TE_VARIABLE: return *n->bound;

        case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
        case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
            switch(ARITY(n->type)) {
                case 0: return TE_FUN(void)();
                case 1: return TE_FUN(double)(M(0));
                case 2: return TE_FUN(double, double)(M(0), M(1));
                case 3: return TE_FUN(double, double, double)(M(0), M(1), M(2));
                case 4: return TE_FUN(double, double, double, double)(M(0), M(1), M(2), M(3));
                case 5: return TE_FUN(double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4));
                case 6: return TE_FUN(double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5));
                case 7: return TE_FUN(double, double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5), M(6));
                default: return NAN;
            }

        case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
        case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
            switch(ARITY(n->type)) {
                case 0: return TE_FUN(void*)(n->parameters[0]);
                case 1: return TE_FUN(void*, double)(n->parameters[1], M(0));
                case 2: return TE_FUN(void*, double, double)(n->parameters[2], M(0), M(1));
                case 3: return TE_FUN(void*, double, double, double)(n->parameters[3], M(0), M(1), M(2));
                case 4: return TE_FUN(void*, double, double, double, double)(n->parameters[4], M(0), M(1), M(2), M(3));
                case 5: return TE_FUN(void*, double, double, double, double, double)(n->parameters[5], M(0), M(1), M(2), M(3), M(4));
                case 6: return TE_FUN(void*, double, double, double, double, double, double)(n->parameters[6], M(0), M(1), M(2), M(3), M(4), M(5));
                case 7: return TE_FUN(void*, double, double, double, double, double, double, double)(n->parameters[7], M(0), M(1), M(2), M(3), M(4), M(5), M(6));
                default: return NAN;
            }

        default: return NAN;
    }

}

#undef TE_FUN
#undef M

static void optimize(te_expr *n) {
    /* Evaluates as much as possible. */
    if (n->type == TE_CONSTANT) return;
    if (n->type == TE_VARIABLE) return;

    /* Only optimize out functions flagged as pure. */
    if (IS_PURE(n->type)) {
        const int arity = ARITY(n->type);
        int known = 1;
        int i;
        for (i = 0; i < arity; ++i) {
            optimize(n->parameters[i]);
            if (((te_expr*)(n->parameters[i]))->type != TE_CONSTANT) {
                known = 0;
            }
        }
        if (known) {
            const double value = te_eval(n);
            te_free_parameters(n);
            n->type = TE_CONSTANT;
            n->value = value;
        }
    }
}


te_expr *te_compile(const char *expression, const te_variable *variables, int var_count, int *error) {
    state s;
    s.start = s.next = expression;
    s.lookup = variables;
    s.lookup_len = var_count;

    next_token(&s);
    te_expr *root = list(&s);

    if (s.type != TOK_END) {
        te_free(root);
        if (error) {
            *error = (s.next - s.start);
            if (*error == 0) *error = 1;
        }
        return 0;
    } else {
        optimize(root);
        if (error) *error = 0;
        return root;
    }
}





double te_interp(const char *expression, int *error) 
{
    te_expr *n = te_compile(expression, 0, 0, error);
    double ret;
    if (n) {
        ret = te_eval(n);
        te_free(n);
    } else {
        ret = NAN;
    }
    return ret;
}

static void pn (const te_expr *n, int depth) {
    int i, arity;
    printf("%*s", depth, "");

    switch(TYPE_MASK(n->type)) {
    case TE_CONSTANT: printf("%f\n", n->value); break;
    case TE_VARIABLE: printf("bound %p\n", n->bound); break;

    case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
    case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
    case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
    case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
         arity = ARITY(n->type);
         printf("f%d", arity);
         for(i = 0; i < arity; i++) {
             printf(" %p", n->parameters[i]);
         }
         printf("\n");
         for(i = 0; i < arity; i++) {
             pn(n->parameters[i], depth + 1);
         }
         break;
    }
}


void te_print(const te_expr *n) 
{
    pn(n, 0);
}
/// end of lib 







int user_celly = 1;
int user_cellx = 1;


char *strrlf(char *str)
{  // seems to work
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if (str[i] != '\n' && str[i] != '\n') 
        ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}






////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
char *strfetchcontent( char *str, int foocy, int foocx  )
{ 
      // str is input // ptr is output 
      char ptr[PATH_MAX];
      char charo[PATH_MAX];
      strncpy( ptr, "", PATH_MAX );
      int i,j=0;
      int foocel, footrg, footrgx, footrgy;
      for(i=0; str[i]!='\0' ; i++)
      {

        /////////////
        /// if you target same S twice, it may crash because it still needs a quick check. 
        // using S for sum is still not yet advanced. 
        // S is still beta testing
        /////////////
        if (( str[i] == 'S' ) && ( str[i+1] == '(' ))  // SUM like =S(1)+1 
        {
           footrg = str[i+2] - 48;
           strncat( ptr , " " , PATH_MAX - strlen( ptr ) -1 ); 
           if ( footrg != foocx ) 
           {
            for( foocel=1; foocel<= CELLYMAXY ; foocel++)
            {
              if ( ( strcmp( celldata[ foocel ][ footrg ] , "" ) != 0 ) 
               && ( celldatatype[ foocel ][ footrg ] == 2 ) )
              {
                 snprintf( charo, PATH_MAX , " + ( %s ) ", strfetchcontent( celldata[ foocel ][ footrg ], foocy , foocx+1 ) );
                 strncat( ptr , charo , PATH_MAX - strlen( ptr ) -1 ); 
              }
            }
            i=i+3;
           }
           else
              strncat( ptr , " error " , PATH_MAX - strlen( ptr ) -1 ); 
        }

        /////////////
        // C to target a given cell content (formulas)
        // C is still beta testing
        /////////////
        else if (( str[i] == 'C' ) && ( str[i+1] == '(' ))  // SUM like =S(1)+1 
        {
           // FORMAT C(X,Y)   X--->   and   Yv vertically
           footrgx = str[i+2]   - 48;      //  S(1)
           footrgy = str[i+2+2] - 48;      //  C(1,1)
           strncat( ptr , " " , PATH_MAX - strlen( ptr ) -1 ); 
           if ( strcmp( celldata[ footrgy ][ footrgx ] , "" ) != 0 ) 
           {
                 snprintf( charo, PATH_MAX , " + ( %s ) ", strfetchcontent( celldata[ footrgy ][ footrgx ], footrgy , footrgx ) );
                 strncat( ptr , charo , PATH_MAX - strlen( ptr ) -1 ); 
           }
           i=i+3+2; // we do still cell up to 9 and 9 (y,x), work in progress. 
        }

        /////////////
        // Right works well
        /////////////
        else if ( str[i] == 'R' ) //right
        {
           snprintf( charo, PATH_MAX , " ( %s ) ", strfetchcontent( celldata[ foocy ][ foocx +1 ], foocy , foocx+1 ) );
           strncat( ptr , charo , PATH_MAX - strlen( ptr ) -1 ); 
        }

        /////////////
        // Left works well
        /////////////
        else if ( str[i] == 'L' ) //left
        {
           snprintf( charo, PATH_MAX , " ( %s ) ", strfetchcontent( celldata[ foocy ][ foocx -1 ], foocy , foocx-1 ) );
           strncat( ptr , charo , PATH_MAX - strlen( ptr ) -1 ); 
        }

        /////////////
        /// OK
        /////////////
        else if ( str[i] == 'X' )  //upper diagonal, left
        {
           ///snprintf( charo, PATH_MAX , "%s", celldata[ foocy -1 ][ foocx -1 ] );
           snprintf( charo, PATH_MAX , " ( %s ) ", strfetchcontent( celldata[ foocy-1 ][ foocx-1 ], foocy-1 , foocx-1 ) );
           strncat( ptr , charo , PATH_MAX - strlen( ptr ) -1 ); 
        }

        /////////////
        /// OK
        /////////////
        else if ( str[i] == 'D' ) // Down
        {
           ///snprintf( charo, PATH_MAX , "%s", celldata[ foocy -1 ][ foocx -1 ] );
           //snprintf( charo, PATH_MAX , " ( %s ) ", strfetchcontent( celldata[ foocy+1 ][ foocx ], foocy+1 , foocx-1 ) );
           snprintf( charo, PATH_MAX , " ( %s ) ", strfetchcontent( celldata[ foocy+1 ][ foocx ], foocy+1 , foocx ) );
           strncat( ptr , charo , PATH_MAX - strlen( ptr ) -1 ); 
        }

        /////////////
        /// OK
        /////////////
        else if ( str[i] == 'U' ) // up
        {
           snprintf( charo, PATH_MAX , " ( %s ) ", strfetchcontent( celldata[ foocy-1 ][ foocx ], foocy-1 , foocx ) );
           //snprintf( charo, PATH_MAX , "%s", celldata[ foocy -1 ][ foocx ] );
           strncat( ptr , charo , PATH_MAX - strlen( ptr ) -1 ); 
        }

        else if (str[i] != '\n' && str[i] != '\n') 
        {
           //   ptr[j++]=str[i];
           snprintf( charo, PATH_MAX , "%c", str[i] );
           strncat( ptr , charo , PATH_MAX - strlen( ptr ) -1 ); 
        }

      } 
      //ptr[j]='\0';
      //size_t siz = sizeof ptr ; 
      //char *r = malloc( sizeof ptr );
      //return r ? memcpy(r, ptr, siz ) : NULL;
      char *base = ptr ; 
      return (base);
}


char *fbasename(char *name)
{
  char *base = name;
  while (*name)
    {
      if (*name++ == '/')
      {
	  base = name;
      }
    }
  return (base);
}




/*
  1.mvprintw( rows-2,cols-9, "%s" , strcut( "hello" , 3 , 6 ) );
                                              3456 
       > it outputs lolu 
  2.now it gives:
  mvprintw( rows-2,cols-9, "%s" , strcut( "hello" , 3 , 6 ) );
                                             3456       
       > it outputs llol   (which might be okay)
*/
char *strcut( char *str , int myposstart, int myposend )
{  
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( ( str[i] != '\0' ) && ( str[i] != '\0') )
         if ( ( i >=  myposstart-1 ) && (  i <= myposend-1 ) )
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}




void gfxhline( int y1, int x1, int x2 )
{
    int foox ;
    for( foox = x1 ; foox <= x2 ; foox++) 
        mvaddch( y1 , foox , ' ' );
}


/////////////
void gfxbox( int y1, int x1, int y2, int x2t )
{
    int x2 = x2t -1 ; 
    int foo, fooy , foox ;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
     for( foox = x1 ; foox <= x2 ; foox++) 
      mvaddch( fooy , foox , ' ' );

    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
        mvaddch( fooy , foo , ACS_VLINE );
    foo = x2;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
         mvaddch( fooy , foo , ACS_VLINE );
    foo = y1;
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( foo , foox , ACS_HLINE );
    foo = y2;
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( foo , foox , ACS_HLINE );
    mvaddch( y1 , x1 , ACS_ULCORNER );
    mvaddch( y1 , x2 , ACS_URCORNER );
    mvaddch( y2 , x1 , ACS_LLCORNER );
    mvaddch( y2 , x2 , ACS_LRCORNER );
}










///////////////////////////////
///////////////////////////////
///////////////////////////////
char *strtimestamp()
{
      long t;
      struct tm *ltime;
      time(&t);
      ltime=localtime(&t);
      char charo[50];  int fooi ; 
      fooi = snprintf( charo, 50 , "%04d%02d%02d%02d%02d%02d",
	1900 + ltime->tm_year, ltime->tm_mon +1 , ltime->tm_mday, 
	ltime->tm_hour, ltime->tm_min, ltime->tm_sec 
	);
    size_t siz = sizeof charo ; 
    char *r = malloc( sizeof charo );
    return r ? memcpy(r, charo, siz ) : NULL;
}






////////////////////////
////////////////////////
////////////////////////
////////////////////////
char *strninput( char *myinitstring )
{

   int strninput_gameover = 0; 
   char strmsg[PATH_MAX];
   char charo[PATH_MAX];
   strncpy( strmsg, myinitstring , PATH_MAX );

   int ch ;  int foo ; 
   int fooj; 
   while ( strninput_gameover == 0 )
   {
                  getmaxyx( stdscr, rows, cols);
                  attroff( A_REVERSE );
                  for ( fooj = 0 ; fooj <= cols-1;  fooj++)
                  {
                    mvaddch( rows-1, fooj , ' ' );
                  }
                  mvprintw( rows-1, 0, ":> %s" , strrlf( strmsg ) );
                  attron( A_REVERSE );
                  printw( " " );
                  attroff( A_REVERSE );
                  attroff( A_REVERSE );
                  attroff( A_BOLD );
                  refresh() ; 

                  curs_set( 0 );
                  ch = getch();

  if ( ch == 8 ) 
    strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );

   else if ( ch == KEY_F(5) )
   {
     strncat( strmsg , clipcell , PATH_MAX - strlen( strmsg ) -1 );  // take with F4
   } 
   else if ( ch == KEY_F(6) )
   {
     strncat( strmsg , clipboard , PATH_MAX - strlen( strmsg ) -1 );  // take with F4
   } 

   else if ( ch == 20 ) strncpy( strmsg, strtimestamp() ,  PATH_MAX );  //ctrl+t

   else if ( ch == 4 ) 
     strncat( strmsg ,  strtimestamp()  , PATH_MAX - strlen( strmsg ) -1 );  //ctrl+d for dateit

  else if ( ch == 2 ) strncpy( strmsg,  ""  ,  PATH_MAX );  // bsd

  else if ( ch == 27 )  
  {
    strncpy( strmsg, ""  ,  PATH_MAX );
    strninput_gameover = 1;
  }

  else if ( ch == 274 )  
  {
    strncpy( strmsg, ""  ,  PATH_MAX );
    strninput_gameover = 1;
  }

  else if ( ch == 263 )  
    strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );



		  else if ( ch == 4 ) 
		  {
			   strncpy( strmsg, ""  ,  PATH_MAX );
		  }
		  else if ( ch == 27 ) 
		  {
			   strncpy( strmsg, ""  ,  PATH_MAX );
		  }
	          else if (
			(( ch >= 'a' ) && ( ch <= 'z' ) ) 
		        || (( ch >= 'A' ) && ( ch <= 'Z' ) ) 
		        || (( ch >= '1' ) && ( ch <= '9' ) ) 
		        || (( ch == '0' ) ) 
		        || (( ch == '~' ) ) 
		        || (( ch == '!' ) ) 
		        || (( ch == '&' ) ) 
		        || (( ch == '=' ) ) 
		        || (( ch == ':' ) ) 
		        || (( ch == ';' ) ) 
		        || (( ch == '<' ) ) 
		        || (( ch == '>' ) ) 
		        || (( ch == ' ' ) ) 
		        || (( ch == '|' ) ) 
		        || (( ch == '#' ) ) 
		        || (( ch == '?' ) ) 
		        || (( ch == '+' ) ) 
		        || (( ch == '/' ) ) 
		        || (( ch == '\\' ) ) 
		        || (( ch == '.' ) ) 
		        || (( ch == '$' ) ) 
		        || (( ch == '%' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == ',' ) ) 
		        || (( ch == '{' ) ) 
		        || (( ch == '}' ) ) 
		        || (( ch == '(' ) ) 
		        || (( ch == ')' ) ) 
		        || (( ch == ']' ) ) 
		        || (( ch == '[' ) ) 
		        || (( ch == '*' ) ) 
		        || (( ch == '"' ) ) 
		        || (( ch == '@' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '_' ) ) 
		        || (( ch == '^' ) ) 
		        || (( ch == '\'' ) ) 
	             ) 
		  {
                           foo = snprintf( charo, PATH_MAX , "%s%c",  strmsg, ch );
			   strncpy( strmsg,  charo ,  PATH_MAX );
		  }
		  else if ( ch == KEY_DOWN ) 
                        strninput_gameover = 1;
		  else if ( ch == KEY_UP ) 
                        strninput_gameover = 1;
		  else if ( ch == 10 ) 
                        strninput_gameover = 1;
     }

     char ptr[PATH_MAX];
     strncpy( ptr, strmsg, PATH_MAX );
     size_t siz = sizeof ptr ; 
     char *r = malloc( sizeof ptr );
     return r ? memcpy(r, ptr, siz ) : NULL;
}






////////////////
void gfxshadow( int y1, int x1, int y2, int x2 )
{
   color_set( 0, NULL ); attroff( A_REVERSE );
   int foo, fooy , foox ;
   for( fooy = y1+1 ; fooy <= y2+1 ; fooy++) 
      mvaddch( fooy , x2 , ' ' );

    for( foox = x1+1 ; foox <= x2 ; foox++) 
      mvaddch( y2+1 , foox , ' ' );

    color_set( 9, NULL ); attron( A_REVERSE );
}

void mvtitle( int myposypass, char *mytext )
{
      color_set( 9, NULL ); attron( A_REVERSE );
      mvaddch( myposypass, cols/2 - strlen( mytext )/2 -4  , ACS_HLINE );
      addch( ACS_RTEE );
      color_set( 11, NULL ); attroff( A_REVERSE );
      printw( " %s ", mytext );
      color_set( 9, NULL ); attron( A_REVERSE );
      addch( ACS_LTEE );
      addch( ACS_HLINE );
      color_set( 9, NULL ); attron( A_REVERSE );
}

void mvbarok( int pxy )
{
      int textlength = 0; int decal = 5;
      color_set( 9, NULL ); attron( A_REVERSE );
      mvprintw( pxy , cols/2 - 2 , "<Ok>" );
}
void mvbarokca( int myposypass)
{
      int textlength = 0; int decal = 5;
      color_set( 9, NULL ); attron( A_REVERSE );
      mvprintw( myposypass, cols/3 - decal , "<Ok>" );
      mvprintw( myposypass, cols-1 - cols/3 - decal+1 , "<Cancel>" );
}
/////////////
void gfxframe( int y1, int x1, int y2, int x2t )
{
    int x2 = x2t ; 
    int foo, fooy , foox ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
        mvaddch( fooy , foo , ACS_VLINE );
    foo = x2;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
         mvaddch( fooy , foo , ACS_VLINE );
    foo = y1;
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( foo , foox , ACS_HLINE );
    foo = y2;
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( foo , foox , ACS_HLINE );
    mvaddch( y1 , x1 , ACS_ULCORNER );
    mvaddch( y1 , x2 , ACS_URCORNER );
    mvaddch( y2 , x1 , ACS_LLCORNER );
    mvaddch( y2 , x2 , ACS_LRCORNER );
}


void mvcenter( int myposypass, char *mytext )
{
   mvprintw( myposypass, cols/2 - strlen( mytext )/2  , "%s", mytext );
}


int void_erase()
{
    attron( A_REVERSE ); curs_set( 1 ); color_set( 7, NULL );
    int foo, fooy , foox ;
    int y1 = 0; int x1=0; 
    int y2 = rows-1; int x2= cols -1 ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
     for( foox = x1 ; foox <= x2 ; foox++) 
      mvprintw( fooy , foox , " " );
}


int void_mycolors()
{
    color_set( 7, NULL ); attron( A_REVERSE ); 
    mvcenter( rows*50/100 -1, "[ 07, NBLUE Press Key ]" );
    color_set( 8, NULL ); attron( A_REVERSE ); 
    mvcenter( rows*50/100 +2 , "[ 08, NRE Press Key ]" );
    color_set( 9, NULL ); attron( A_REVERSE ); 
    mvcenter( rows*50/100 +3 , "[ 09, Button Press Key ]" );
    color_set( 10, NULL ); attron( A_REVERSE ); 
    mvcenter( rows*50/100 +4 , "[ 10, Button Press Key ]" );
    color_set( 11, NULL ); attron( A_REVERSE ); 
    mvcenter( rows*50/100 +4 , "[ 11, Button Press Key ]" );
}


/////////////////////////////////////////////////////
int void_ncdialog_message( char *toptitle, char *mytitle, char *mytext )
{
        int myvar = 0;
        int mypopupgameover = 0; int mych ; 
        while( mypopupgameover == 0)
        {
           void_erase();
           color_set( 9, NULL ); attron( A_REVERSE );
           gfxbox( 2, 2,    rows-3, cols-3 );
           gfxshadow( 2, 2, rows-3, cols-3 );
           mvtitle( 2 , mytitle );
           mvbarokca( rows-5 );
           //mvbarok( rows-5 );
           color_set( 7, NULL ); mvprintw( 0, 0, "%s", toptitle ); 
           color_set( 9, NULL ); mvprintw( 3, 4, "%s", mytext );
           mych = getch();
           mypopupgameover = 1;
           if ( mych == '1' )      myvar = 1;
           else if ( mych == 'y' ) myvar = 1;
        }
        return myvar ;
}


/////////////////////////////////////////////////////
int void_ncwin_message( char *toptitle, char *mytitle, char *mytext )
{
        int myvar = 0;
        int mypopupgameover = 0; int mych ; 
        while( mypopupgameover == 0)
        {
           //void_erase();
           color_set( 9, NULL ); attron( A_REVERSE );
           gfxbox(    2, 2,    rows-3, cols-3 );
           gfxshadow( 2, 2, rows-3, cols-3 );
           mvtitle( 2 , mytitle );
           mvbarokca( rows-5 );
           //mvbarok( rows-5 );
           if ( strcmp( toptitle , "" ) != 0 ) 
           {   color_set( 7, NULL ); mvprintw( 0, 0, "%s", toptitle ); }
           color_set( 9, NULL ); mvprintw( 3, 4, "%s", mytext );
           mych = getch();
           mypopupgameover = 1;
           if ( mych == '1' )      myvar = 1;
           else if ( mych == 'y' ) myvar = 1;
        }
        return myvar ;
}


////////////////////////////////
void ncurses_runwith( char *thecmd , char *thestrfile  ) //thecmd first
{
       char cmdi[PATH_MAX];
       def_prog_mode();
       endwin();
       printf( "<APP-CMD: Command...>\n" );
       strncpy( cmdi , "  " , PATH_MAX );
       strncat( cmdi , thecmd , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , thestrfile , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\" " , PATH_MAX - strlen( cmdi ) -1 );
       printf( "<APP-CMD: %s>\n", cmdi );
       system( cmdi );
       reset_prog_mode();
}







////////////////////////
void void_load()
{
  savecellymaxy = CELLYMAXY;
  savecellymaxx = CELLYMAXX;
  
  int y1 = 0; int x1=0; 
  int y2 = rows-1; int x2= cols -1 ;
  int foox ; int fooy; 
  color_set( 7, NULL ); attron( A_REVERSE ); 
  for( fooy = y1 ; fooy <= y2 ; fooy++) 
  for( foox = x1 ; foox <= x2 ; foox++) 
      mvprintw( fooy , foox , " " );
    
  int foocelly = 1;
  int foocellx = 1;

  int posy; int posx;
  posy = 1; 
  char charo[PATH_MAX];
  char statusbar_cellcontent[PATH_MAX];
  strncpy( statusbar_cellcontent , "" , PATH_MAX );

  int fetchi; int fetchj;
  FILE *fp5;
  FILE *fp6;
  char fetchline[PATH_MAX];
  char fetchlinetmp[PATH_MAX];
  int linepos = 0;  int cellnew = 0; 
   /////////////////////////////////////////////////
   if ( fexist( filesource ) == 1 )
   {
     fp6 = fopen( filesource , "rb");
     while( !feof(fp6) ) 
     {
          fgets(fetchlinetmp, PATH_MAX, fp6); 
          strncpy( fetchline, "" , PATH_MAX );
          for( fetchi = 0 ; ( fetchi <= strlen( fetchlinetmp ) ); fetchi++ )
          // if ( fetchlinetmp[ fetchi ] != '\n' )
              fetchline[fetchi]=fetchlinetmp[fetchi];
          linepos++;

         if ( !feof(fp6) )
         if ( linepos >= user_scrolly ) 
         {
 	     //fputs( fetchline , fp5 );
  	     //fputs( "\n", fp5 );
             //attron( A_REVERSE );
             //if ( posy <= rows - 3) 
             //mvprintw( posy++, 2, "%s",  fetchline  );  
             //if ( foocelly == user_celly ) mvprintw( posy, 1, ">" );
             // mvprintw( posy, 2, "" );
             if ( posy <= CELLYMAXY ) 
             {
               foocellx = 1; cellnew = 1; 
               strncpy( statusbar_cellcontent , "" , PATH_MAX ); 
               celldatatype[posy][foocellx] = 1; 
               for( fetchi = 0 ; ( fetchi <= strlen( fetchline ) ); fetchi++ )
               {
                     //attron( A_REVERSE );
                     //if ( ( foocelly == user_celly ) && ( foocellx == user_cellx ) )
                     if (( fetchline[ fetchi ] == ';' ) || ( fetchline[ fetchi ] == 9 ) || ( fetchline[ fetchi ] == '\n' ))   
                     {
                           if ( statusbar_cellcontent[ 0 ] == '=' ) 
                           {
                                        char str[PATH_MAX]; 
                                        strncpy( str, statusbar_cellcontent, PATH_MAX );
                                        char ptr[strlen(str)+1];
                                        int i,j=0;
                                        for(i=0; str[i]!='\0'; i++)
                                        {
                                          //if (str[i] != '\n' ) 
                                           if (str[i] != '=' )
                                             ptr[j++]=str[i];
                                        } 
                                        ptr[j]='\0';
                      
                              strncpy( celldata[posy][ foocellx ], ptr , PATH_MAX );
                              celldatatype[posy][foocellx] = 2; 
                           }
                           else   
                           {
                              celldatatype[posy][foocellx] = 1; 
                              strncpy( celldata[posy][ foocellx ], statusbar_cellcontent , PATH_MAX );
                           }
                           // next
                           foocellx++;
                           strncpy( statusbar_cellcontent , "" , PATH_MAX ); 
                           celldatatype[posy][foocellx] = 1; 
                           cellnew = 1; 
                     }
                     else
                     {
                            snprintf( charo, PATH_MAX , "%c", fetchline[ fetchi ] );
                            strncat( statusbar_cellcontent , charo , PATH_MAX - strlen( statusbar_cellcontent ) -1 );
                            cellnew = 0;
                     }

                     //if ( posx <= cols-2 -5)
                     //{
                     //printw( "%c", fetchline[fetchi] );
                     //  posx = posx + 1;
                     //}
               }
              }
               
              foocelly++;
              posy++;
 
	 }
     }
     fclose( fp6 );
     }
}



void void_cell_clear()
{
    int fooy, foox; 
    for( fooy = 1 ; fooy <= CELLYMAXY+5 ; fooy++) 
    for( foox = 1 ; foox <= CELLYMAXX+5 ; foox++) 
    {
       strncpy( celldata[fooy][foox], "", PATH_MAX );
       celldatatype[fooy][foox] = 1;
    }
}




   void printfile( char *ttuxfile )
   {
       int posyy = 2 ;  
       int zui = 0 ;  FILE *fpsource;  char ptrout[PATH_MAX];
       color_set( 7, NULL );  attron( A_REVERSE );
       gfxbox( 0, cols/2-2, rows-1, cols-1 );
       gfxframe( 0, cols/2-2, rows-1, cols-1 );
           if ( fexist( ttuxfile ) == 1 )
           {  
             mvprintw( 0, cols/2, "[FILE: %s]", fbasename( ttuxfile ) );
             posyy = 2;
             fpsource = fopen( ttuxfile , "r");
             while(  !feof(fpsource) )
             {
                 if ( !feof(fpsource) )
                 {
                   fgets( ptrout , PATH_MAX , fpsource ); 
                   if ( posyy <= rows-3 )
                    for(zui=0; ptrout[zui]!='\0'; zui++)
                     if ( ptrout[zui] != '\n' )
                      if ( zui <=  cols/2 -2 )
                        mvprintw( posyy, cols/2 + zui , "%c", ptrout[zui] );
                   posyy++;
                 }
             }
             fclose(fpsource);
           }
   }



/////////////////////////
/////////////////////////
int  nexp_user_sel = 1;
int  nexp_user_scrolly =0 ;
char nexp_user_fileselection[PATH_MAX]; 
int  tc_det_dir_type = 0;
/////////////////////////
void printdir()
{

   DIR *dirp; int posy = 0;  int posx, chr ; 
   int fooselection = 0;
   posy = 1; posx = cols/2;
   char cwd[PATH_MAX];
   struct dirent *dp;
   dirp = opendir( "." );
   attron( A_REVERSE );

   color_set( 7, NULL ); attron( A_REVERSE );
   gfxbox( 0, cols/2-2, rows-1, cols-1 );
   //gfxhline( 0, 0, cols-1 );
   mvprintw( 0, cols/2, "[DIR: %s]", getcwd( cwd, PATH_MAX ) );
   strncpy( nexp_user_fileselection, "", PATH_MAX );
   color_set( 11, NULL ); attron( A_REVERSE );
   int entrycounter = 0;
   while  ((dp = readdir( dirp )) != NULL ) 
   if ( posy <= rows-3 )
   {
        entrycounter++;
        color_set( 13, NULL ); attron( A_REVERSE ); attroff( A_BOLD );

        if ( entrycounter <= nexp_user_scrolly )
              continue;

        if (  dp->d_name[0] !=  '.' ) 
        if (  strcmp( dp->d_name, "." ) != 0 )
        if (  strcmp( dp->d_name, ".." ) != 0 )
        {
            posy++;  fooselection++;

            if ( dp->d_type == DT_DIR ) 
            {
                 color_set( 12 , NULL );
                 mvaddch( posy, posx++ , '/' );
            }
            else if ( dp->d_type == 0 )
            {
               if ( tc_det_dir_type == 1 )
               if ( fexist( dp->d_name ) == 2 )
               {
                 color_set( 7 , NULL );
                 mvaddch( posy, posx++ , '/' );
               }
            }

            if ( nexp_user_sel == fooselection ) 
            {
                  strncpy( nexp_user_fileselection, dp->d_name , PATH_MAX );
                  color_set( 6, NULL );
            }

            for ( chr = 0 ;  chr <= strlen(dp->d_name) ; chr++) 
            {
              if  ( dp->d_name[chr] == '\n' )
                  posx = cols/2;
              else if  ( dp->d_name[chr] == '\0' )
                  posx = cols/2;
              else
                 mvaddch( posy, posx++ , dp->d_name[chr] );
            }
        }
   }
   closedir( dirp );

   color_set( 7, NULL ); attroff( A_REVERSE );
   //gfxhline( rows-1, 0, cols-1 );
   mvprintw( rows-1, cols/2, "[FILE: %s]", nexp_user_fileselection );
}








////////////////////////
void void_draw()
{
    int y1 = 0; int x1=0; 
    int y2 = rows-1; int x2= cols -1 ;
    int foox ; int fooy; 
    color_set( 7, NULL ); attron( A_REVERSE ); 
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
    for( foox = x1 ; foox <= x2 ; foox++) 
      mvprintw( fooy , foox , " " );
    
  int foocelly = 1;
  int foocellx = 1;

  int posy; int posx;
  posy = 2; 
  char charo[PATH_MAX];
  char statusbar_cellcontent[PATH_MAX];
  strncpy( statusbar_cellcontent , "" , PATH_MAX );

  int fetchi; int fetchj;
  FILE *fp5;
  FILE *fp6;
  char fetchline[PATH_MAX];
  char fetchlinetmp[PATH_MAX];
  int linepos = 0; 
   /////////////////////////////////////////////////
   if ( fexist( filesource ) == 1 )
   {
     fp6 = fopen( filesource , "rb");
     while( !feof(fp6) ) 
     {
          fgets(fetchlinetmp, PATH_MAX, fp6); 
          strncpy( fetchline, "" , PATH_MAX );
          for( fetchi = 0 ; ( fetchi <= strlen( fetchlinetmp ) ); fetchi++ )
            if ( fetchlinetmp[ fetchi ] != '\n' )
              fetchline[fetchi]=fetchlinetmp[fetchi];
          linepos++;

         if ( !feof(fp6) )
         if ( linepos >= user_scrolly ) 
         {
 	     //fputs( fetchline , fp5 );
  	     //fputs( "\n", fp5 );
             foocellx = 1;
             posx     = 2;
             attron( A_REVERSE );
             if ( posy <= rows - 3) 
             {
               //mvprintw( posy++, 2, "%s",  fetchline  );  
              if ( foocelly == user_celly ) 
                mvprintw( posy, 1, ">" );

               mvprintw( posy, 2, "" );
               for( fetchi = 0 ; ( fetchi <= strlen( fetchline ) ); fetchi++ )
               {
                     attron( A_REVERSE );
                     if ( ( foocelly == user_celly ) && ( foocellx == user_cellx ) )
                     {
                        attroff( A_REVERSE );
                        snprintf( charo, PATH_MAX , "%c", fetchline[ fetchi ] );
                        strncat( statusbar_cellcontent , charo , PATH_MAX - strlen( statusbar_cellcontent ) -1 );
                     }

                     if ( posx <= cols-2 -5)
                     {
                        posx = posx + 1;
                     }
               }
               
               foocelly++;
               posy++;
             }
 
	 }
     }
     fclose( fp6 );
     }
}




/////////////////////
/////////////////////
/////////////////////
void app_save( char *strfileoutput )
{
   int readcellx , readcelly ; 
   FILE *fp6;
   fp6 = fopen( strfileoutput , "wb+");
   for( readcelly=1 ; readcelly<= savecellymaxy ; readcelly++) 
   {
     for( readcellx=1 ; readcellx<= savecellymaxx ; readcellx++) 
     {
        if ( celldatatype[readcelly][readcellx] == 2 ) fputs( "=", fp6 );
        fputs( celldata[readcelly][readcellx], fp6 );
  	fputs( ";", fp6 );
     }
     fputs( "\n", fp6 );
   }
   fclose( fp6 );
}
/////////////////////
/////////////////////
/////////////////////





////
void main_app_draw()
{
   erase();
   getmaxyx( stdscr, rows, cols);
   int y1 = 0; int x1=0; 
   int y2 = rows-1; int x2= cols -1 ;
   int foox ; int fooy; 
   color_set( 0, NULL ); attroff( A_REVERSE ); 

   char foocellcontent[PATH_MAX];
    
   int foocelly = 1;
   int foocellx = 1;
   int posy , posx ; 
   int lineskip = 1;

   int readcellx , readcelly ; 
   posy = 0; 
   int corex = 1;
   color_set( 6, NULL ); 
   attron( A_REVERSE );
   gfxhline( 0 , 0, cols-1 );
   for( readcellx = 65 ; readcellx <= 65+ CELLYMAXX-1 ; readcellx++) 
   {
     if ( user_cellx == readcellx -64 ) 
      color_set( 3, NULL );  else color_set( 6, NULL ); 
     mvprintw( posy , corex* user_col_space , "        ", readcellx );
     mvprintw( posy , corex* user_col_space , "%c", readcellx );
     corex++;
   }
   for( readcelly = 1 ; readcelly <= rows-3 ; readcelly++) 
   {
     if ( user_celly == readcelly ) 
      color_set( 3, NULL );  else color_set( 6, NULL ); 
     mvprintw( readcelly , 0, "   "); 
     mvprintw( readcelly , 0, "%d", readcelly );
   }

   // clear and clean
   color_set( 0, NULL );
   attroff( A_REVERSE );
   strncpy( currentcellline, "" , PATH_MAX );

   //posy = readcelly;
   posy = 1;
   for( readcelly = 1 ; readcelly <= CELLYMAXY ; readcelly++) 
   {
       lineskip = 0;
       if ( celldata[readcelly][1][0] == '/' )
       if ( celldata[readcelly][1][1] == '/' )
         lineskip = 1;

     if ( lineskip == 0 )
     {
      for( readcellx = 1 ; readcellx <= CELLYMAXX ; readcellx++) 
      {
       color_set( 0, NULL ); attroff( A_REVERSE ); 
       posx = user_col_space * readcellx ;  

       if ( strcmp( celldata[ readcelly ][ readcellx ] , "!MAX" ) == 0 ) //new
       {  savecellymaxy = readcelly ;  savecellymaxx = readcellx ; }

       if (( readcellx == savecellymaxx ) && ( readcelly == savecellymaxy ))
             color_set( 5, NULL ); 

       if ( ( user_cellx == readcellx ) && ( user_celly == readcelly ) ) {   
             attron(A_REVERSE );  
       }

       if ( ( celldatatype[readcelly][readcellx] == 1 ) && ( strcmp( celldata[readcelly][readcellx] , "" ) == 0 ) ) 
          mvprintw( posy , posx, "-" );
       else if ( celldatatype[readcelly][readcellx] == 1 )
       {
          mvprintw( posy , posx, "" );
          int fetchi; char fetchlinetmp[PATH_MAX];
          strncpy( fetchlinetmp, "" , PATH_MAX );
          strncpy( fetchlinetmp, celldata[readcelly][readcellx] , PATH_MAX );
          //mvprintw( posy , posx, "%s", celldata[readcelly][readcellx] );

          for( fetchi = 0 ; ( fetchi <= strlen( fetchlinetmp ) ); fetchi++ )
            if ( fetchlinetmp[ fetchi ] != '\n' )
             if ( fetchlinetmp[ fetchi ] != '\0' )
              if ( fetchi <= user_col_charmax -2 ) 
               addch( fetchlinetmp[ fetchi] );
       }
       else if ( celldatatype[readcelly][readcellx] == 2 )
       {
          strncpy( foocellcontent, strfetchcontent( celldata[readcelly][readcellx] , readcelly, readcellx ) , PATH_MAX );
          //char *strfetchcontent( char *str, int foocy, int foocx  )
          mvprintw( posy , posx, "%f",  
          te_interp( foocellcontent , 0 ) );
          strncpy( debugclip, foocellcontent , PATH_MAX );
       }
       
       // end of loop right
        if ( user_celly == readcelly )
        {
         if ( celldatatype[readcelly][readcellx] == 2 ) 
               strncat( currentcellline , "="  , PATH_MAX - strlen( currentcellline ) -1 ); 
         strncat( currentcellline , celldata[readcelly][readcellx]  , PATH_MAX - strlen( currentcellline ) -1 ); 
         strncat( currentcellline , ";"  , PATH_MAX - strlen( currentcellline ) -1 );
        }
      // end of loop right
     }
     posy++;
    }
   }
   color_set( 0, NULL ); 



   // lower status line/bar and content
   attroff( A_REVERSE ); color_set( 0, NULL );  attroff( A_BOLD );
   gfxhline( rows-2 , 0, cols-1 );
   gfxhline( rows-1 , 0, cols-1 );

   //mvprintw( rows-1, 0, "[%d,%d]|%s|", user_celly, user_cellx, filesource );  <- might be cool to stick to C
   mvprintw( rows-1, 0, "[%d,%d]|%s|", user_cellx, user_celly, filesource );  // used for C(2,3),...
   printw( " [F1:Help][F9:Menu][F10:Quit]%s", freelotus123_statusline );
   strncpy( freelotus123_statusline , "", PATH_MAX );

   if (  celldatatype[ user_celly][user_cellx ]  == 2 )  
     mvprintw( rows-2, 0, "[%d][=%s]",  
     celldatatype[ user_celly][user_cellx ] , 
     celldata[ user_celly][user_cellx ] );
   else
     mvprintw( rows-2, 0, "[%d][%s]", 
     celldatatype[ user_celly][user_cellx ] , 
     celldata[ user_celly][user_cellx ] );
}





///////////////
int main( int argc, char *argv[])
{
    strncpy( clipcell, "", PATH_MAX );
    strncpy( freelotus123_statusline , "", PATH_MAX );

    ////////////////////////////////////////////////////////
    if ( argc >= 2)
    if ( strcmp(argv[1], "--version" ) == 0)
    {
	  printf( "Software version: %s\n", VERSIONNBR );
          return 0;
    }



    int foo; int foochg; int i ; 
    char foostr[PATH_MAX];
    char foocwd[PATH_MAX];
    char foomsgstr[PATH_MAX];
    //////////////////////////////////////////
    strncpy( filesource , "noname.ws1" , PATH_MAX );
    //if ( fexist( argv[ 1 ] ) == 1 ) 
    if ( argc == 2)
    {
        strncpy( filesource , argv[ 1 ], PATH_MAX );
    }


    int fooy, foox; 
    void_cell_clear();
    strncpy( currentcellline , "" , PATH_MAX );

    initscr();	
    curs_set( 0 );
    noecho();            
    keypad( stdscr, TRUE ); // with Function keys 
    start_color();
    init_pair(0,  COLOR_WHITE,     COLOR_BLACK);
    init_pair(1,  COLOR_RED,     COLOR_BLACK);
    init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
    init_pair(3,  COLOR_BLUE,    COLOR_WHITE );
    init_pair(4,  COLOR_YELLOW,  COLOR_BLACK);
    init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
    init_pair(7,  COLOR_BLUE,    COLOR_WHITE);
    init_pair(8,  COLOR_BLUE,    COLOR_GREEN);
    init_pair(9,  COLOR_WHITE,   COLOR_BLACK);
    init_pair(10, COLOR_WHITE,   COLOR_RED);
    init_pair(11,  COLOR_RED,     COLOR_WHITE);
    init_pair(12,  COLOR_BLUE,    COLOR_YELLOW);
    init_pair(13,  COLOR_BLUE,    COLOR_MAGENTA);


    erase();
    getmaxyx( stdscr, rows, cols);

    attron( A_REVERSE );
    color_set( 7, NULL );

    int y1 = 0; int x1=0; 
    int y2 = rows-1; int x2= cols -1 ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
     for( foox = x1 ; foox <= x2 ; foox++) 
      mvprintw( fooy , foox , " " );

   if ( argc == 2)
   if ( fexist( filesource ) == 1 ) 
   {
         strncpy( filesource , argv[ 1 ], PATH_MAX );
         void_load();
   }

   color_set( 7, NULL ); attron( A_REVERSE ); 
   int ch ; foo = 0; ch = 0;
   int gameover = 0; 
   while( gameover == 0)
   {
        if ( user_cellx <= 1 ) user_cellx = 1 ; 
        if ( user_celly <= 1 ) user_celly = 1 ; 
        if ( user_cellx >= CELLYMAXX ) user_cellx = CELLYMAXX ; 
        if ( user_celly >= CELLYMAXY ) user_celly = CELLYMAXY ; 
        color_set( 7, NULL ); attroff( A_REVERSE ); 
        main_app_draw();
        color_set( 0, NULL ); attroff( A_REVERSE ); 
        mvprintw( rows-1, cols-2, "%d", ch );
        ch = getch();
        switch( ch )
        {
           case KEY_F(10):
              gameover = 1; 
              break;

           case 'q':
              foo = void_ncwin_message( "" , " Exit Program?", "This will quit the following application." );
              if ( foo == 1 ) gameover = 1;
              break;

           case KEY_F(1):
           //   foo = void_ncwin_message( "Help Content" , "Help", 
           //   "hjlk: move, enter/=:cell edit, ?:content, F10:exit, have fun with freedom and opensource." );
           color_set( 9, NULL ); attron( A_REVERSE );
           gfxbox(    2, 2,    rows-3, cols-3 );
           gfxshadow( 2, 2, rows-3, cols-3 );
           mvtitle( 2 , "HELP" );
           foo = 5;
           mvprintw( foo++, 4, "hjlk: move (like vim)" ) ; 
           mvprintw( foo++, 4, "Enter:cell edit (string)");
           mvprintw( foo++, 4, "= to edit a formula (math/formulas)");
           mvprintw( foo++, 4, "    = R fetch formula content of right cell " );
           mvprintw( foo++, 4, "    = L fetch formula content of left cell " );
           mvprintw( foo++, 4, "    = U fetch formula content of upper cell (just above) " );
           mvprintw( foo++, 4, "    = X fetch formula content of upper/left cell " );
           mvprintw( foo++, 4, "    = S fetch full column content (formula) e.g. =S(1) " );
           mvprintw( foo++, 4, "    = L X D S : are regular commands" );
           mvprintw( foo++, 4, "= C : is a command to read a given cell position" );
           mvprintw( foo++, 4, "= C(2,2) is a command to read a given cell position (2,2)" );
           mvprintw( foo++, 4, "F1 or ?: content"); 
           mvprintw( foo++, 4, "F2: Save"); 
           mvprintw( foo++, 4, "F3: Load"); 
           mvprintw( foo++, 4, "F4: Save as..."); 
           mvprintw( foo++, 4, "F10:exit, have fun with freedom and opensource." );
           mvbarok( rows-5 );
              getch();
              break;

           case '?':
              foo = void_ncwin_message( "Current line" , "Information",  currentcellline );
              break;

           case KEY_F(2):
              snprintf( foostr , PATH_MAX , "Would you like to save to %s filename.", filesource );
              foo = void_ncwin_message( "File operation" , "Save data [y/N]?", foostr );
              if ( foo == 1 )
              {
                 app_save( filesource );
                 if ( fexist( filesource ) == 1 ) 
                  void_ncwin_message( "File operation" , "Saved!", "The file has been saved with success." );
              }
              break;

           case KEY_F(3):
              snprintf( foostr  , PATH_MAX , "Would you like to load to %s filename.", filesource );
              foo = void_ncwin_message( "File operation" , "Load data [y/N]?", foostr );
              if ( foo == 1 )
              {
                    void_cell_clear();
                    void_load();
              }
              break;


           case KEY_F(4):
           case 'Y':
              strncpy( freelotus123_statusline , " Clipcell!", PATH_MAX );
              strncpy( clipcell, "", PATH_MAX );
              //user_celly++;
              snprintf( clipcell , PATH_MAX , "C(%d,%d)", user_cellx, user_celly  );
              /*
              color_set( 0, NULL ); attron( A_REVERSE );  mvprintw( 0, 0, "|SAVE AS|" );
              strncpy(  foomsgstr , strninput( filesource ) , PATH_MAX );
              snprintf( foostr , PATH_MAX , "Would you like to save to %s filename.", foomsgstr );
              foo = void_ncwin_message( "File operation" , "Save data [y/N]?", foostr );
              if ( foo == 1 )
              {
                 app_save( foomsgstr );
                 strncpy( filesource, foomsgstr , PATH_MAX ); void_cell_clear(); void_load();
                 if ( fexist( filesource ) == 1 ) void_ncwin_message( "File operation" , "Saved!", "The file has been saved with success." );
              }
              */
              break;


           case 15:
           case 'o':
              foochg = 0; 
              while ( foochg != 'i' )
              { 
                printdir(); 
                foochg = getch();
                if ( foochg == 'j' )      nexp_user_sel++; 
                else if ( foochg == 'k' ) nexp_user_sel--;
                else if ( foochg == 'g' ) nexp_user_sel = 1;
                else if ( foochg == 'r' ) ncurses_runwith( " tcview " , nexp_user_fileselection );
                else if ( foochg == 'v' ) ncurses_runwith( " vim " , nexp_user_fileselection );
                else if ( foochg == 10 ) { strncpy( filesource, nexp_user_fileselection, PATH_MAX ); void_cell_clear(); void_load(); foochg = 'i'; }
                else if ( foochg == 'o' ) { printfile( nexp_user_fileselection ); getch();  }
                else if ( foochg == 'c' ) 
                {
                      strncpy( foostr , getcwd( foocwd, PATH_MAX ), PATH_MAX );
                      chdir( getenv( "HOME" ) );
                      if ( fexist( ".clipboard" ) == 1 )
                      {
                        printfile( ".clipboard" ); 
                        ch = getch(); 
                        if ( ch == 'v' )         ncurses_runwith( " vim " , ".clipboard" );
                        else if  ( ch == 'n' )   ncurses_runwith( " screen -d -m nedit  " , ".clipboard" );
                      }
                      chdir( foostr );
                }
              }
              break;

             case 'c':
                      strncpy( foostr , getcwd( foocwd, PATH_MAX ), PATH_MAX );
                      chdir( getenv( "HOME" ) );
                      if ( fexist( ".clipboard" ) == 1 )
                      {
                        printfile( ".clipboard" ); 
                        ch = getch(); 
                        if ( ch == 'v' )  ncurses_runwith( " vim " , ".clipboard" );
                        else if  ( ch == 'n' )   ncurses_runwith( " screen -d -m nedit  " , ".clipboard" );
                      }
                      chdir( foostr );
                   break;

           case 'v':
              snprintf( foostr , PATH_MAX , "Would you like to use vim with %s filename.", filesource );
              foo = void_ncwin_message( "File operation" , "VIM data [y/N]?", foostr );
              if ( foo == 1 ) 
              {
                ncurses_runwith( " vim  " , filesource ); 
                void_cell_clear();
                void_load(); 
              }
              break;
           ////////// reload end

           case '$':
              nsystem( strninput( "" ) ); 
              break;

           case '!':
              ncurses_runwith( strninput( "" ) , filesource ); 
              break;

           case ':':
              color_set( 0, NULL ); attroff( A_REVERSE );
              strncpy( foostr, strninput( "" ) , PATH_MAX );
              if (  strcmp( foostr, "linecount" ) == 0 )
                   {  erase(); mvcenter( 0, "INFORMATION" ); mvprintw( 2, 0, "Line count: %s %d", filesource, filelinecount(     filesource ) ); getch();  }

              else if (  strcmp( foostr, "path" ) == 0 )
              {
                     erase();
                     mvprintw( 0 , 0 , "PATH: %s" , getcwd( foocwd , PATH_MAX ) );
                     getch();
              }

              else if (  strcmp( foostr, "args" ) == 0 )
              {
                     erase();
                     if ( argc >= 2)
                     {
                           for( i = 1 ; i < argc ; i++) 
                 	       mvprintw( i, 0 , "Arg nbr: %d/%d: %s fexist=%d\n", i, argc-1 ,  argv[ i ], fexist( argv[ i ]) );
                     }
                     getch();
              }
              break;

           //case 'd':
           case 'd':
              user_celly++;
              user_celly++;
              user_celly++;
              user_celly++;
              break;
           case 'u':
              user_celly--;
              user_celly--;
              user_celly--;
              user_celly--;
              break;
           case KEY_RIGHT:
           case 'l':
              user_cellx++;
              break;
           case KEY_LEFT:
           case 'h':
              user_cellx--;
              break;
           case 'j':
           case KEY_DOWN:
              user_celly++;
              break;
           case KEY_UP:
           case 'k':
              user_celly--;
              break;


           //case '0':
           //   user_cellx = 1; 
           //   break;

           case 'g':
              user_cellx = 1;
              user_celly = 1;
              break;

          // case '$':
              //user_cellx = CELLYMAXX-1;
           //   user_cellx = CELLYMAXX;
            //  break;

           case 'C':
              void_mycolors(); getch();
              break;

           ////////// reload and co, start
           //case 's':
             // void_cell_clear();
             // void_load(); 
             // break;



           case '=':
              strncpy( celldata[user_celly][user_cellx], strninput( "" ) , PATH_MAX );
              celldatatype[user_celly][user_cellx] = 2;
              break;

           case '1':
           case '2':
           case '3':
           case '4':
           case '5':
           case '6':
           case '7':
           case '8':
           case '9':
           case '0':
              snprintf( foostr  , PATH_MAX , "%c", ch );
              strncpy( celldata[user_celly][user_cellx], strninput( foostr ) , PATH_MAX );
              celldatatype[user_celly][user_cellx] = 2;
              user_celly++;
              break;


           case '(':
              user_col_charmax--;
              break;
           case ')':
              user_col_charmax++;
              break;

           case '<':
              user_col_space--;
              break;
           case '>':
              user_col_space++;
              break;

           case 'y':
           case KEY_F(5):
              strncpy( clipboard, celldata[user_celly][user_cellx], PATH_MAX );
              clipboardtype = celldatatype[user_celly][user_cellx]; 
              strncpy( freelotus123_statusline , " Clip formula!", PATH_MAX );
              break;
           case 'p':
           case KEY_F(6):
              strncpy( celldata[user_celly][user_cellx], clipboard , PATH_MAX );
              celldatatype[user_celly][user_cellx] = 1;
              celldatatype[user_celly][user_cellx] = clipboardtype ; 
              break;
           case KEY_F(7):
              strncpy( celldata[user_celly][user_cellx], strninput( "" ) , PATH_MAX );
              celldatatype[user_celly][user_cellx] = 1;
              break;
           case KEY_F(8):
           case 'x':
           case 330:
              strncpy( clipboard, celldata[user_celly][user_cellx], PATH_MAX );
              clipboardtype = celldatatype[user_celly][user_cellx]; 
              strncpy( celldata[user_celly][user_cellx], "" , PATH_MAX );
              celldatatype[user_celly][user_cellx] = 1;
              break;

           case 4:
              snprintf( foostr , PATH_MAX , "id-%s", strtimestamp() );
              strncpy( celldata[user_celly][user_cellx], foostr , PATH_MAX );
              celldatatype[user_celly][user_cellx] = 1;
              break;


           case 'Q':
              foo = void_ncdialog_message( "Message" , " Exit Program?", "This will quit the following application." );
              if ( foo == 1 ) gameover = 1;
              break;

           case 'e':
           case 10:
           case 32:
              strncpy( celldata[user_celly][user_cellx], strninput( celldata[user_celly][user_cellx]  ) , PATH_MAX );
              break;

           case 'b': //blank cell
              strncpy( celldata[user_celly][user_cellx], strninput( "" ) , PATH_MAX );
              celldatatype[user_celly][user_cellx] = 1;
              break;

           case '#':
              erase(); 
              mvprintw( 0, 0, "%s Clip" , debugclip );
              mvprintw( 5, 2, "Define max save y/x cell [y/N] ?" );
              ch = getch();
              if ( ( ch == '1' ) || ( ch == 'y' ) ) 
              {
                 savecellymaxy = user_celly;  
                 savecellymaxx = user_cellx;
              }
              break;

         }
        }

   attroff( A_REVERSE ); curs_set( 1 ); color_set( 0, NULL );
   endwin();		/* End curses mode  */
   return 0;
}



