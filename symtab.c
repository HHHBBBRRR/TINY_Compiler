#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

typedef struct LineListRec
{
    int lineno;
    struct LineListRec *next;
} *LineList;

typedef struct BucketListRec
{
    char *name;
    LineList lines;
    int memloc; /* memory location for variable */
    struct BucketListRec *next;
} *BucketList;

static BucketList hashTable[SIZE];

// hash function
static int hash(char *key)
{
    int temp = 0;

    for (int i = 0; key[i] != '\0'; i++)
    {
        temp = ((temp << 4) + key[i]) % SIZE;
    }

    return temp;
}

void st_insert(char *name, int lineno, int loc)
{
    int h = hash(name);
    BucketList l = hashTable[h];

    while ((l != NULL) && (strcmp(name, l->name) != 0))
    {
        l = l->next;
    }

    if (l == NULL) /* variable not yet in table */
    {
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList)malloc(sizeof(struct LineListRec));
        l->lines->lineno = lineno;
        l->memloc = loc;
        l->lines->next = NULL;
        l->next = hashTable[h];
        hashTable[h] = l;
    }
    else /* found in table, so just add line number */
    {
        LineList t = l->lines;

        while (t->next != NULL)
        {
            t = t->next;
        }
        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
    }
}

int st_lookup(char *name)
{
    int h = hash(name);
    BucketList l = hashTable[h];

    while ((l != NULL) && (strcmp(name, l->name) != 0))
    {
        l = l->next;
    }

    if (l == NULL)
    {
        return -1;
    }
    else
    {
        l->memloc;
    }
}

void printSymTab(FILE *listing)
{
    fprintf(listing, "Variable Name  Location   Line Numbers\n");
    fprintf(listing, "-------------  --------   ------------\n");

    for (int i = 0; i < SIZE; ++i)
    {
        if (hashTable[i] != NULL)
        {
            BucketList l = hashTable[i];
            
            while (l != NULL)
            {
                LineList t = l->lines;
                fprintf(listing, "%-14s ", l->name);
                fprintf(listing, "%-8d  ", l->memloc);
                while (t != NULL)
                {
                    fprintf(listing, "%4d ", t->lineno);
                    t = t->next;
                }
                fprintf(listing, "\n");
                l = l->next;
            }
        }
    }
}