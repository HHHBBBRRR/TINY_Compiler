#ifndef SCAN_H
#define SCAN_H

#include "globals.h"

#define MAXTOKENLEN 40  // max length of token string

extern char tokenString[MAXTOKENLEN + 1];   // holds token string(add '\0' in the end)

TokenType getToken(void);

#endif