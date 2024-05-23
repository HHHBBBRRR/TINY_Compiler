#include "analyze.h"
#include "symtab.h"

static int locatoin = 0;

static void typeError(TreeNode *node, char *message)
{
    fprintf(listing, "Type error at line %d: %s\n", node->lineno, message);
    Error = true;
}

static void traverse(TreeNode *node, void (*preProc)(TreeNode *), void (*postProc)(TreeNode *))
{
    if (node == NULL)
    {
        return;
    }

    preProc(node);
    for (int i = 0; i < MAXCHILDREN; i++)
    {
        traverse(node->child[i], preProc, postProc);
    }
    postProc(node);
    traverse(node->sibling, preProc, postProc);
}

static void nullProc(TreeNode *t) // do nothing
{
    return;
}

static void insertNode(TreeNode *node)
{
    switch (node->nodekind)
    {
    case StmtK:
        if (node->kind.stmt == AssignK || node->kind.stmt == ReadK)
        {
            if (st_lookup(node->attr.name) == -1)
            {
                st_insert(node->attr.name, node->lineno, locatoin++);
            }
            else
            {
                st_insert(node->attr.name, node->lineno, 0);
            }
        }
        break;
    case ExpK:
        if (node->kind.exp == IdK)
        {
            if (st_lookup(node->attr.name) == -1)
            {
                st_insert(node->attr.name, node->lineno, locatoin++);
            }
            else
            {
                st_insert(node->attr.name, node->lineno, 0);
            }
        }
        break;
    }
}

static void checkNode(TreeNode * node)
{
    switch (node->nodekind)
    {
    case StmtK:
        switch (node->kind.stmt)
        {
        case IfK:
            if (node->child[0]->type != Boolean)
            {
                typeError(node->child[0], "if condition is not boolean");
            }
            break;
        case AssignK:
            if (node->child[0]->type != Integer)
            {
                typeError(node->child[0], "assign type mismatch");
            }
            break;
        case WriteK:
            if (node->child[0]->type != Integer)
            {
                typeError(node->child[0], "write type mismatch");
            }
            break;
        case RepeatK:
            if (node->child[1]->type != Boolean)
            {
                typeError(node->child[1], "repeat condition is not Boolen");
            }
            break;
        }
        break;
    case ExpK:
        switch (node->kind.exp)
        {
        case OpK:
            if ((node->child[0]->type != Integer ) || (node->child[1]->type != Integer))
            {
                typeError(node,"Op applied to non-integer");
            }

            if (node->attr.op == LT || node->attr.op == EQ)
            {
                node->type = Boolean;
            }
            else
            {
                node->type = Integer;
            }       
            break;
        case ConstK:
        case IdK:
            node->type = Integer;
            break;
        }
    }
}

/*****************
 * public fucntions
 * **************/

void buildSymtab(TreeNode *root)
{
    traverse(root, insertNode, nullProc);

    if (TraceAnalyze)
    {
        fprintf(listing, "\nSymbol table:\n\n");
        printSymTab(listing);
    }
}

void typeCheck(TreeNode *root)
{
    traverse(root, nullProc, checkNode);
}