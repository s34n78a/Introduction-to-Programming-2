#ifndef __LEX__
#define __LEX__

#define MAXLEN 256

typedef enum {
    UNKNOWN, END, ENDFILE,
    INT, ID,
    ADDSUB, MULDIV,
    AND, OR, XOR,
    ASSIGN, ADDSUB_ASSIGN,
    INCDEC,
    LPAREN, RPAREN
} TokenSet;

extern int match(TokenSet token);
extern void advance(void);
extern char *getLexeme(void);

#endif
