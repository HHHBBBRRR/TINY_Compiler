#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    ENDFILE,  // when get to the end of file
    ERROR,    // find something wrong
    /* Keywords */
    IF, THEN, ELSE ,END, REPEAT, UNTIL, READ, WRITE,
    /*Identifier and Number (need multi-char)*/
    ID, NUM,
    /* Special symbols */
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
} TokenType;

extern FILE* source;    /* source code text file */
extern FILE* listing;   /* listing output text file */

extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern bool EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

#endif