#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token;

/* function prototypes for recursive calls */
static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * repeat_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * exp(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);

static void syntaxError(char *message)
{
    fprintf(listing, "\n>>> ");
    fprintf(listing, "Syntax error at line %d: %s", lineno, message);
    Error = true;
}

static void match(TokenType expected)
{
    if (token == expected)
    {
        token = getToken();
    }
    else
    {
        syntaxError("unexpected token -> ");
        printToken(token,tokenString);
        fprintf(listing,"      ");
    }
}


