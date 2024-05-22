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


// stmt_sequence ->statement { ; statement }
TreeNode * stmt_sequence(void)
{
    TreeNode* begin = statement();
    TreeNode* end = begin;

    while ((token != ENDFILE) && (token != END)
            && (token != ELSE) && (token != UNTIL))
    {
        TreeNode* current;

        matchch(SEMI);
        current = statement();

        if (current != NULL)
        {
            if (begin == NULL)
            {
                begin = end = current;
            }
            else
            {
                end->sibling = current;
                end = current;
            }  
        }
    }

    return begin;
}

// statement -> if_stmt | repeat_stmt | assign_stmt | read_stmt | write_stmt
TreeNode * statement(void)
{
    TreeNode* node = NULL;

    switch (token)
    {
    case IF:
        node = if_stmt();
        break;
    case REPEAT:
        node = repeat_stmt();
        break;
    case ID:
        node = assign_stmt();
        break;
    case READ:
        node = read_stmt();
        break;
    case WRITE:
        node = write_stmt();
        break;  
    default:
        syntaxError("unexpected token -> ");
        printToken(token,tokenString);
        token = getToken();
        break;
    }

    return node;
}

// if_stmt -> if exp then stmt_sequence [ else stmt_sequence ] end
TreeNode * if_stmt(void)
{
    TreeNode* node = newStmtNode(IfK);

    match(IF);
    node->child[0] = exp();  
    match(THEN);
    node->child[1] = stmt_sequence();  
    if (token == ELSE)
    {
        match(ELSE);
        node->child[2] = stmt_sequence();     
    }   
    match(END);

    return node;
}

// repeat_stmt -> repeat stmt_sequence until exp
TreeNode* repeat_stmt()
{
    TreeNode* node = newStmtNode(RepeatK);

    match(REPEAT);
    node->child[0] = stmt_sequence();
    match(UNTIL);
    node->child[1] = exp();

    return node;
}

// assign_stmt -> identifier := exp
TreeNode* assign_stmt()
{
    TreeNode* node = newStmtNode(AssignK);

    if (token == ID)
    {
        node->attr.name = copyString(tokenString);
    } 
    match(ID);
    match(ASSIGN);
    node->child[0] = exp();

    return node;
}

// read_stmt -> read identifier
TreeNode* read_stmt()
{
    TreeNode* node = newStmtNode(ReadK);

    match(READ);
    if (token == ID)
    {
        node->attr.name = copyString(tokenString);
    } 
    match(ID);

    return node;
}

// write_stmt -> write exp
TreeNode* write_stmt()
{
    TreeNode* node = newStmtNode(WriteK);

    match(WRITE);
    node->child[0] = exp();

    return node;
}

// exp -> simple_exp [ comparison_op simple_exp ]
// comparison_op -> = < | =
TreeNode* exp()
{
    TreeNode* node = simple_exp();

    if (token == LT || token == EQ)
    {
        TreeNode * opNode = newExpNode(OpK);

        opNode->attr.op = token;
        opNode->child[0] = node;
        node = opNode;
        match(token);
        node->child[1] = simple_exp();
    }

    return node;
}

// simple_exp -> term { addop term }
// addop -> + | -
TreeNode* simple_exp()
{
    TreeNode* node = term();

    while ((token == PLUS) || (token == MINUS))
    {
        TreeNode* opNode = newExpNode(OpK);

        opNode->attr.op = token;
        opNode->child[0] = node;
        node = opNode;
        match(token);
        node->child[1] = term();
    }

    return node;
}

// term -> factor { mulop factor }
// mulop -> * | /
TreeNode* term()
{
    TreeNode* node = factor();

    while ((token == TIMES) || (token == OVER))
    {
        TreeNode* opNode = newExpNode(OpK);

        opNode->attr.op = token;
        opNode->child[0] = node;
        node = opNode;
        match(token);
        node->child[1] = factor();
    }

    return node;
}

// factor -> ( exp ) | number | identifier
TreeNode* factor()
{
    TreeNode* node;

    switch (token)
    {   
    case LPAREN:
        match(LPAREN);
        node = exp();
        match(RPAREN);
        break;
    case NUM:
        node = newExpNode(ConstK);
        node->attr.val = atoi(tokenString);
        match(NUM);
        break;
    case ID:
        node = newExpNode(IdK);
        node->attr.name = copyString(tokenString);
        match(ID);
        break;
    default:
        syntaxError("unexpected token -> ");
        printToken(token,tokenString);
        token = getToken();
        break;
    }

    return node;
}

/*******************
 * public functions
*******************/

// program -> stmt_sequence
TreeNode * parse(void)
{  
    TreeNode* root;

    token = getToken();
    root = stmt_sequence();
    if (token != ENDFILE)
    {
        syntaxError("Code ends before file\n");
    }
    
    return root;
}