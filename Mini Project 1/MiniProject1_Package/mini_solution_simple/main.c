#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parser.h"

int main() {
    initTable();
    printf(">> ");
    while (1) {
        statement();
    }
    return 0;
}
