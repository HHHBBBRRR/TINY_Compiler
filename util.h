#ifndef UTIL_H
#define UTIL_H

#include "globals.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char *tokenString);

TreeNode *newStmtNode(StmtKind kind);
TreeNode *newExpNode(ExpKind kind);
char *copyString(char *s);

#endif
