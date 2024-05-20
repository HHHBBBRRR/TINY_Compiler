#include "globals.h"
#include "scan.h"

#define BUFLEN 256
#define MAXRESERVED 8   // number of reserved words(keyword)

static struct
{
	char *str;
	TokenType tok;
} reservedWords[MAXRESERVED] = {
    {"if", IF}, 
    {"then", THEN}, 
    {"else", ELSE}, 
    {"end", END}, 
    {"repeat", REPEAT},
    {"until", UNTIL}, 
    {"read", READ}, 
    {"write", WRITE}
};

typedef enum
{
	START,
	INASSIGN,
	INCOMMENT,
	INNUM,
	INID,
	DONE
} StateType;

char tokenString[MAXTOKENLEN + 1];

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0;		 /* current position in LineBuf */
static int bufsize = 0;		 /* current size of buffer string */
static bool EOF_flag = false;