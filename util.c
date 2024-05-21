#include "util.h"

void printToken(TokenType token, const char *tokenString)
{
	switch (token)
	{
	case IF:
	case THEN:
	case ELSE:
	case END:
	case REPEAT:
	case UNTIL:
	case READ:
	case WRITE:
		fprintf(listing, "reserved word: %s\n", tokenString);
		break;
	case ASSIGN:
		fprintf(listing, ":=\n");
		break;
	case LT:
		fprintf(listing, "<\n");
		break;
	case EQ:
		fprintf(listing, "=\n");
		break;
	case LPAREN:
		fprintf(listing, "(\n");
		break;
	case RPAREN:
		fprintf(listing, ")\n");
		break;
	case SEMI:
		fprintf(listing, ";\n");
		break;
	case PLUS:
		fprintf(listing, "+\n");
		break;
	case MINUS:
		fprintf(listing, "-\n");
		break;
	case TIMES:
		fprintf(listing, "*\n");
		break;
	case OVER:
		fprintf(listing, "/\n");
		break;
	case ENDFILE:
		fprintf(listing, "EOF\n");
		break;
	case NUM:
		fprintf(listing, "NUM, val= %s\n", tokenString);
		break;
	case ID:
		fprintf(listing, "ID, name= %s\n", tokenString);
		break;
	case ERROR:
		fprintf(listing, "ERROR: %s\n", tokenString);
		break;
	default: /* should never happen */
		fprintf(listing, "Unknown token: %d\n", token);
	}
}

// create a new statement node for AST
TreeNode *newStmtNode(StmtKind kind)
{
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

	if (node == NULL)
	{
		fprintf(listing, "Out of memory error at line %d\n", lineno);
		return NULL;
	}

	for (int i = 0; i < MAXCHILDREN; i++)
	{
		node->child[i] = NULL;
	}
	node->sibling = NULL;
	node->nodekind = StmtK;
	node->kind.stmt = kind;
	node->lineno = lineno;

	return node;
}

// create a new expression node for AST
TreeNode *newExpNode(ExpKind kind)
{
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

	if (node == NULL)
	{
		fprintf(listing, "Out of memory error at line %d\n", lineno);
		return NULL;
	}

	for (int i = 0; i < MAXCHILDREN; i++)
	{
		node->child[i] = NULL;
	}

	node->sibling = NULL;
	node->nodekind = ExpK;
	node->kind.exp = kind;
	node->lineno = lineno;
	node->type = Void;

	return node;
}

char *copyString(char *s)
{
	int n;
	char *t;

	if (s == NULL)
	{
		return NULL;
	}

	n = strlen(s) + 1;
	t = malloc(n);
	if (t == NULL)
	{
		fprintf(listing, "Out of memory error at line %d\n", lineno);
		return NULL;
	}

	strcpy(t, s);

	return t;
}