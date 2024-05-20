#include "globals.h"
#include "scan.h"

#define BUFLEN 256
#define MAXRESERVED 8 // number of reserved words(keyword)

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
    {"write", WRITE}};

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
static int linepos = 0;      /* current position in LineBuf */
static int bufsize = 0;      /* current size of buffer string */
static bool EOF_flag = false;

/*****************
 * private funcs *
 *****************/
static int getNextChar(void)
{
    if (linepos >= bufsize)     //need to get a new line from the source file
    {
        lineno++;
        if (fgets(lineBuf, BUFLEN - 1, source))
        {
            if (EchoSource == true)
            {
                fprintf(listing, "%4d: %s", lineno, lineBuf);
            }
            bufsize = strlen(lineBuf);
            linepos = 0;
        }
        else                // no more lines in the file
        {
            EOF_flag = true;
            return EOF;
        }
    }

    return lineBuf[linepos++];
}

// put a char "back" into the lineBuf
static void ungetNextChar(void)
{
	if (EOF_flag == false)
    {
        linepos--;
    }
}

static TokenType reservedLookup(char *s)
{
    for (int i = 0; i < MAXRESERVED; i++)
    {
        if (strcmp(s, reservedWords[i].str) == 0)
        {
            return reservedWords[i].tok;
        }   
    }
    
    return ID;
}

