#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"

static TokenSet getToken(void);
static TokenSet curToken = UNKNOWN;
static char lexeme[MAXLEN];

TokenSet getToken(void) {
    int i = 0;
    char c = '\0';

    // Skip whitespace
    while ((c = fgetc(stdin)) == ' ' || c == '\t');

    // Integer
    if (isdigit(c)) {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isdigit(c) && i < MAXLEN) {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    }

    // Multi-char operators and single-char ops
    if (c == '+') {
        c = fgetc(stdin);
        if (c == '+') {
            strcpy(lexeme, "++");
            return INCDEC;
        } else if (c == '=') {
            strcpy(lexeme, "+=");
            return ADDSUB_ASSIGN;
        } else {
            ungetc(c, stdin);
            strcpy(lexeme, "+");
            return ADDSUB;
        }
    } else if (c == '-') {
        c = fgetc(stdin);
        if (c == '-') {
            strcpy(lexeme, "--");
            return INCDEC;
        } else if (c == '=') {
            strcpy(lexeme, "-=");
            return ADDSUB_ASSIGN;
        } else {
            ungetc(c, stdin);
            strcpy(lexeme, "-");
            return ADDSUB;
        }
    } else if (c == '*') {
        strcpy(lexeme, "*");
        return MULDIV;
    } else if (c == '/') {
        strcpy(lexeme, "/");
        return MULDIV;
    } else if (c == '&') {
        strcpy(lexeme, "&");
        return AND;
    } else if (c == '|') {
        strcpy(lexeme, "|");
        return OR;
    } else if (c == '^') {
        strcpy(lexeme, "^");
        return XOR;
    } else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    } else if (c == '\n') {
        lexeme[0] = '\0';
        return END;
    } else if (c == EOF) {
        return ENDFILE;
    }

    // Identifier: must start with letter or underscore, then alphanumeric or underscore
    if (isalpha(c) || c == '_') {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while ((isalnum(c) || c == '_') && i < MAXLEN) {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;
    }

    return UNKNOWN;
}

void advance(void) {
    curToken = getToken();
}

int match(TokenSet token) {
    if (curToken == UNKNOWN)
        advance();
    return token == curToken;
}

char *getLexeme(void) {
    return lexeme;
}
