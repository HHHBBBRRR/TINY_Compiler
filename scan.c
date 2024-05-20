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
    if (linepos >= bufsize) // need to get a new line from the source file
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
        else // no more lines in the file
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

TokenType getToken(void)
{
    bool save; // save the token?
    int tokenStringIndex = 0;
    TokenType currentToken;  // current token type which will be returned
    StateType state = START; // current state of the finite automaton

    while (state != DONE)
    {
        int ch = getNextChar();
        save = true;

        switch (state)
        {
        case START:
            if (isdigit(ch))
            {
                state = INNUM;
            }
            else if (isalpha(ch))
            {
                state = INID;
            }
            else if (ch == ':')
            {
                state = INASSIGN;
            }
            else if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
            {
                save = false;
            }
            else if (ch == '{')
            {
                save = false;
                state = INCOMMENT;
            }
            else
            {
                state = DONE;
                switch (ch)
                {
                case EOF:
                    save = false;
                    currentToken = ENDFILE;
                    break;
                case '=':
                    currentToken = EQ;
                    break;
                case '<':
                    currentToken = LT;
                    break;
                case '+':
                    currentToken = PLUS;
                    break;
                case '-':
                    currentToken = MINUS;
                    break;
                case '*':
                    currentToken = TIMES;
                    break;
                case '/':
                    currentToken = OVER;
                    break;
                case '(':
                    currentToken = LPAREN;
                    break;
                case ')':
                    currentToken = RPAREN;
                    break;
                case ';':
                    currentToken = SEMI;
                    break;
                default:
                    currentToken = ERROR;
                    break;
                }
            }
        case INCOMMENT:
            save = false;
            if (ch == EOF)
            {
                state = DONE;
                currentToken = ENDFILE;
            }
            else if (ch == '}')
            {
                state = START;
            }
            break;
        case INASSIGN:
            state = DONE;
            if (ch == '=')
            {
                currentToken = ASSIGN;
            }
            else
            {
                ungetNextChar();
                save = false;
                currentToken = ERROR;
            }
            break;
        case INNUM:
            if (!isdigit(ch))
            {
                ungetNextChar();
                save = false;
                state = DONE;
                currentToken = NUM;
            }
            break;
        case INID:
            if (!isalpha(ch))
            {
                ungetNextChar();
                save = false;
                state = DONE;
                currentToken = ID;
            }
            break;
        case DONE: // fall through, should never happen
        default:
            fprintf(listing, "Scanner Bug: state= %d\n", state);
            state = DONE;
            currentToken = ERROR;
            break;
        }

        if (save == true && tokenStringIndex <= MAXTOKENLEN)
        {
            tokenString[tokenStringIndex++] = (char)ch;
        }

        if (state == DONE)
        {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
            {
                currentToken = reservedLookup(tokenString);
            }
        }  
    }

    if (TraceScan)
    {
        fprintf(listing, "\t%d: ", lineno);
        printToken(currentToken, tokenString);
    }

    return currentToken;
}