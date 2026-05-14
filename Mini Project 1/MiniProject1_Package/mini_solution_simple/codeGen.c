#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGen.h"

static int nextTemp = 3;  // Start from r3
static int totalClock = 0;

int allocReg(void) {
    if (nextTemp > 7) {
        fprintf(stderr, "error: out of registers\n");
        exit(1);
    }
    return nextTemp++;
}

void freeReg(void) {
    if (nextTemp > 3)
        nextTemp--;
}

void emit(const char *op, int r1, int r2, int imm, int hasr2, int hasimm, int cc) {
    printf("%s ", op);
    printf("r%d", r1);
    if (hasr2)
        printf(", r%d", r2);
    else if (hasimm) {
        if (imm >= 0)
            printf(", %d", imm);
        else
            printf(", [%d]", -imm);  // negative means memory address
    }
    printf("\n");
    totalClock += cc;
}

int evaluateConst(BTNode *root) {
    if (root == NULL)
        return 0;
    
    if (root->data == INT)
        return atoi(root->lexeme);
    
    if (root->data == ADDSUB) {
        int l = evaluateConst(root->left);
        int r = evaluateConst(root->right);
        if (strcmp(root->lexeme, "+") == 0)
            return l + r;
        else
            return l - r;
    }
    
    if (root->data == MULDIV) {
        int l = evaluateConst(root->left);
        int r = evaluateConst(root->right);
        if (strcmp(root->lexeme, "*") == 0) {
            return l * r;
        } else if (strcmp(root->lexeme, "/") == 0) {
            if (r == 0)
                error(DIVZERO_CONST);
            return l / r;
        }
    }
    
    return 0;
}

// Returns the register holding the result
int genCode(BTNode *root) {
    if (root == NULL)
        return 0;
    
    int r1, r2, val;
    
    if (root->data == INT) {
        r1 = allocReg();
        val = atoi(root->lexeme);
        emit("MOV", r1, 0, val, 0, 1, 10);
        return r1;
    }
    
    if (root->data == ID) {
        // Find which register or memory this variable is in
        for (int i = 0; i < sbcount; i++) {
            if (strcmp(root->lexeme, table[i].name) == 0) {
                if (i == 0) return 0;  // x is in r0
                if (i == 1) return 1;  // y is in r1
                if (i == 2) return 2;  // z is in r2
                // Other variables: load from memory
                r1 = allocReg();
                emit("MOV", r1, 0, -(table[i].addr), 0, 1, 10);  // 10cc for memory access simplified
                return r1;
            }
        }
        return 0;
    }
    
    if (root->data == INCDEC) {
        // ++ID or --ID
        int idx = -1;
        for (int i = 0; i < sbcount; i++) {
            if (strcmp(root->left->lexeme, table[i].name) == 0) {
                idx = i;
                break;
            }
        }
        
        if (idx == 0) r1 = 0;  // x
        else if (idx == 1) r1 = 1;  // y
        else if (idx == 2) r1 = 2;  // z
        else {
            r1 = allocReg();
            emit("MOV", r1, 0, -(table[idx].addr), 0, 1, 10);
        }
        
        r2 = allocReg();
        emit("MOV", r2, 0, 1, 0, 1, 10);
        
        if (strcmp(root->lexeme, "++") == 0)
            emit("ADD", r1, r2, 0, 1, 0, 10);
        else
            emit("SUB", r1, r2, 0, 1, 0, 10);
        
        freeReg();
        return r1;
    }
    
    if (root->data == ASSIGN) {
        // ID = expr
        r2 = genCode(root->right);
        
        for (int i = 0; i < sbcount; i++) {
            if (strcmp(root->left->lexeme, table[i].name) == 0) {
                if (i == 0) {
                    emit("MOV", 0, r2, 0, 1, 0, 10);
                } else if (i == 1) {
                    emit("MOV", 1, r2, 0, 1, 0, 10);
                } else if (i == 2) {
                    emit("MOV", 2, r2, 0, 1, 0, 10);
                } else {
                    emit("MOV", r2, 0, -(table[i].addr), 0, 1, 10);  // store to memory
                }
                if (r2 >= 3)
                    freeReg();
                return (i < 3) ? i : r2;
            }
        }
        return r2;
    }
    
    if (root->data == ADDSUB_ASSIGN) {
        // ID += expr or ID -= expr
        r2 = genCode(root->right);
        
        for (int i = 0; i < sbcount; i++) {
            if (strcmp(root->left->lexeme, table[i].name) == 0) {
                if (i < 3) {
                    r1 = i;
                    if (strcmp(root->lexeme, "+=") == 0)
                        emit("ADD", r1, r2, 0, 1, 0, 10);
                    else
                        emit("SUB", r1, r2, 0, 1, 0, 10);
                } else {
                    r1 = allocReg();
                    emit("MOV", r1, 0, -(table[i].addr), 0, 1, 10);
                    if (strcmp(root->lexeme, "+=") == 0)
                        emit("ADD", r1, r2, 0, 1, 0, 10);
                    else
                        emit("SUB", r1, r2, 0, 1, 0, 10);
                    emit("MOV", r2, r1, 0, 1, 0, 10);
                }
                if (r2 >= 3)
                    freeReg();
                return r1;
            }
        }
        return r2;
    }
    
    if (root->data == MULDIV) {
        r1 = genCode(root->left);
        r2 = genCode(root->right);
        
        if (r1 >= 3) {
            // Move r1 to a temp
            int temp = allocReg();
            emit("MOV", temp, r1, 0, 1, 0, 10);
            freeReg();
            r1 = temp;
        } else {
            r1 = allocReg();
            emit("MOV", r1, (root->left->data == ID ? 0 : r1), 0, 1, 0, 10);
        }
        
        if (strcmp(root->lexeme, "*") == 0) {
            emit("MUL", r1, r2, 0, 1, 0, 30);
        } else {
            emit("DIV", r1, r2, 0, 1, 0, 50);
        }
        
        if (r2 >= 3)
            freeReg();
        
        return r1;
    }
    
    if (root->data == ADDSUB || root->data == AND || root->data == XOR || root->data == OR) {
        r1 = genCode(root->left);
        r2 = genCode(root->right);
        
        if (r1 >= 3) {
            int temp = allocReg();
            emit("MOV", temp, r1, 0, 1, 0, 10);
            freeReg();
            r1 = temp;
        } else {
            r1 = allocReg();
            emit("MOV", r1, r1, 0, 1, 0, 10);
        }
        
        if (root->data == ADDSUB) {
            if (strcmp(root->lexeme, "+") == 0)
                emit("ADD", r1, r2, 0, 1, 0, 10);
            else
                emit("SUB", r1, r2, 0, 1, 0, 10);
        } else if (root->data == AND) {
            emit("AND", r1, r2, 0, 1, 0, 10);
        } else if (root->data == XOR) {
            emit("XOR", r1, r2, 0, 1, 0, 10);
        } else if (root->data == OR) {
            emit("OR", r1, r2, 0, 1, 0, 10);
        }
        
        if (r2 >= 3)
            freeReg();
        
        return r1;
    }
    
    return 0;
}

void generateCode(BTNode *root) {
    nextTemp = 3;
    totalClock = 0;
    
    // Load initial x, y, z from memory
    printf("MOV r0, [0]\n");  // x
    printf("MOV r1, [4]\n");  // y
    printf("MOV r2, [8]\n");  // z
    totalClock += 30;
    
    if (root != NULL) {
        int result = genCode(root);
        
        // Finalize: ensure x, y, z are in r0, r1, r2
        if (result != 0 && result < 3) {
            // Result is in r0, r1, or r2 (which is correct)
        }
    }
    
    printf("EXIT 0\n");
    totalClock += 20;
    
    // Print total clock cycles
    printf("\n");
    printf("Total clock cycles: %d\n", totalClock);
}
