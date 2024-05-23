#ifndef ANALYZE_H
#define ANALYZE_H

#include "globals.h"

void buildSymtab(TreeNode *root); /* build sym table */
void typeCheck(TreeNode *root); /* type check tree */

#endif