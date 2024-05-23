#include "globals.h"
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