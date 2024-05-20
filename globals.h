#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    /* Control */
    ENDFILE, ERROR,
    /* Keywords */
    IF, THEN, ELSE ,END, REPEAT, UNTIL, READ, WRITE,
    /*Identifier and Number */
    ID, NUM,
    /* Special symbols */
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
} TokenType;


#endif