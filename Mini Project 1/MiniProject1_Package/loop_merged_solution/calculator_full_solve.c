#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define MAXLEN   256
#define TBLSIZE  256
#define REGCOUNT 8
#define PRINTERR 0

typedef enum {
    UNKNOWN, END, ENDFILE,
    INT, ID,
    ADDSUB, MULDIV,
    ASSIGN, ADDSUB_ASSIGN, INCDEC,
    LPAREN, RPAREN,
    AND, OR, XOR
} TokenSet;

typedef enum {
    UNDEFINED, MISPAREN, NOTNUMID, NOTFOUND, RUNOUT,
    NOTLVAL, DIVZERO, SYNTAXERR
} ErrorType;

typedef struct {
    char name[MAXLEN];
    int  memAddr;
    int  defined;
} Symbol;

typedef struct _Node {
    TokenSet     data;
    char         lexeme[MAXLEN];
    struct _Node *left;
    struct _Node *right;
} BTNode;

/* globals */
static char     lexeme[MAXLEN];
static TokenSet curToken = UNKNOWN;
static int      sbcount  = 0;
static Symbol   table[TBLSIZE];
static int      nextMem  = 12;
static int      regUsed[REGCOUNT];
static int      hasError = 0;

#define OUTBUF 131072
static char outBuf[OUTBUF];
static int  outPos = 0;

/* ---------- emit (no commas) ---------- */
static void emit(const char *fmt, ...) {
    va_list ap; char tmp[512];
    va_start(ap, fmt); vsnprintf(tmp, sizeof(tmp), fmt, ap); va_end(ap);
    int len = (int)strlen(tmp);
    if (outPos + len < OUTBUF - 1) {
        memcpy(outBuf + outPos, tmp, len);
        outPos += len; outBuf[outPos] = '\0';
    }
}
static void emitMOV_RR(int d,int s){emit("MOV r%d r%d\n",d,s);}
static void emitMOV_RC(int d,int c){emit("MOV r%d %d\n",d,c);}
static void emitMOV_RM(int d,int a){emit("MOV r%d [%d]\n",d,a);}
static void emitMOV_MR(int a,int s){emit("MOV [%d] r%d\n",a,s);}
static void emitADD(int d,int s){emit("ADD r%d r%d\n",d,s);}
static void emitSUB(int d,int s){emit("SUB r%d r%d\n",d,s);}
static void emitMUL(int d,int s){emit("MUL r%d r%d\n",d,s);}
static void emitDIV(int d,int s){emit("DIV r%d r%d\n",d,s);}
static void emitAND(int d,int s){emit("AND r%d r%d\n",d,s);}
static void emitOR (int d,int s){emit("OR r%d r%d\n",d,s);}
static void emitXOR(int d,int s){emit("XOR r%d r%d\n",d,s);}

/* ---------- register allocator ---------- */
static int allocReg(void){
    int i; for(i=0;i<REGCOUNT;i++) if(!regUsed[i]){regUsed[i]=1;return i;}
    fprintf(stderr,"FATAL: out of registers\n"); exit(1);
}
static void freeReg(int r){if(r>=0&&r<REGCOUNT)regUsed[r]=0;}

/* ---------- symbol table ---------- */
static void initTable(void){
    strcpy(table[0].name,"x");table[0].memAddr=0; table[0].defined=1;
    strcpy(table[1].name,"y");table[1].memAddr=4; table[1].defined=1;
    strcpy(table[2].name,"z");table[2].memAddr=8; table[2].defined=1;
    sbcount=3; nextMem=12;
}
static int findSym(const char *n){
    int i; for(i=0;i<sbcount;i++) if(strcmp(table[i].name,n)==0)return i; return -1;
}
static int addSym(const char *n){
    if(sbcount>=TBLSIZE){hasError=1;return -1;}
    strcpy(table[sbcount].name,n); table[sbcount].memAddr=nextMem;
    table[sbcount].defined=0; nextMem+=4; return sbcount++;
}

/* ---------- lexer ---------- */
static TokenSet getToken(void){
    int i=0; char c;
    while((c=(char)fgetc(stdin))==' '||c=='\t');
    if(c==(char)EOF){lexeme[0]='\0';return ENDFILE;}
    if(c=='\n'){lexeme[0]='\0';return END;}
    if(isdigit((unsigned char)c)){
        lexeme[i++]=c;
        while(isdigit((unsigned char)(c=(char)fgetc(stdin)))&&i<MAXLEN-1)lexeme[i++]=c;
        ungetc(c,stdin); lexeme[i]='\0'; return INT;
    }
    if(isalpha((unsigned char)c)||c=='_'){
        lexeme[i++]=c;
        while((isalnum((unsigned char)(c=(char)fgetc(stdin)))||c=='_')&&i<MAXLEN-1)lexeme[i++]=c;
        ungetc(c,stdin); lexeme[i]='\0'; return ID;
    }
    if(c=='+'||c=='-'){
        char nx=(char)fgetc(stdin);
        if(nx==c){lexeme[0]=c;lexeme[1]=c;lexeme[2]='\0';return INCDEC;}
        if(nx=='='){lexeme[0]=c;lexeme[1]='=';lexeme[2]='\0';return ADDSUB_ASSIGN;}
        ungetc(nx,stdin); lexeme[0]=c;lexeme[1]='\0'; return ADDSUB;
    }
    if(c=='*'||c=='/'){lexeme[0]=c;lexeme[1]='\0';return MULDIV;}
    if(c=='='){strcpy(lexeme,"=");return ASSIGN;}
    if(c=='('){strcpy(lexeme,"(");return LPAREN;}
    if(c==')'){strcpy(lexeme,")");return RPAREN;}
    if(c=='&'){strcpy(lexeme,"&");return AND;}
    if(c=='|'){strcpy(lexeme,"|");return OR;}
    if(c=='^'){strcpy(lexeme,"^");return XOR;}
    lexeme[0]=c;lexeme[1]='\0'; return UNKNOWN;
}
static void advance(void){curToken=getToken();}
static int  match(TokenSet t){if(curToken==UNKNOWN)advance();return t==curToken;}
static char *getLexeme(void){return lexeme;}

/* ---------- error / drain ---------- */
static void drainLine(void){
    while(!match(END)&&!match(ENDFILE))advance();
    if(match(END))advance();
}
static void err(ErrorType e){(void)e; drainLine(); hasError=1;}

/* ---------- AST ---------- */
static BTNode *makeNode(TokenSet tok,const char *lex){
    BTNode *n=(BTNode*)malloc(sizeof(BTNode));
    n->data=tok; strcpy(n->lexeme,lex); n->left=n->right=NULL; return n;
}
static void freeTree(BTNode *r){if(!r)return;freeTree(r->left);freeTree(r->right);free(r);}

/* ---------- parser (forward decls) ---------- */
static BTNode *assign_expr(void);
static BTNode *or_expr(void);
static BTNode *xor_expr(void);
static BTNode *and_expr(void);
static BTNode *addsub_expr(void);
static BTNode *muldiv_expr(void);
static BTNode *unary_expr(void);
static BTNode *factor(void);
static BTNode *finish_from_id(BTNode *base);

static BTNode *factor(void){
    BTNode *retp;
    if(hasError)return makeNode(INT,"0");
    if(match(INT)){retp=makeNode(INT,getLexeme());advance();return retp;}
    if(match(ID)){retp=makeNode(ID,getLexeme());advance();return retp;}
    if(match(INCDEC)){
        char op[MAXLEN]; strcpy(op,getLexeme()); advance();
        if(!match(ID)){err(NOTNUMID);return makeNode(INT,"0");}
        retp=makeNode(INCDEC,op);
        retp->left=makeNode(ID,getLexeme()); advance(); return retp;
    }
    if(match(LPAREN)){
        advance(); retp=assign_expr();
        if(!hasError){if(match(RPAREN))advance();else err(MISPAREN);}
        return retp;
    }
    err(NOTNUMID); return makeNode(INT,"0");
}
static BTNode *unary_expr(void){
    BTNode *node;
    if(hasError)return makeNode(INT,"0");
    if(match(ADDSUB)){
        char op[MAXLEN]; strcpy(op,getLexeme());
        node=makeNode(ADDSUB,op); node->left=makeNode(INT,"0");
        advance(); node->right=unary_expr(); return node;
    }
    return factor();
}
static BTNode *muldiv_expr(void){
    BTNode *left,*node;
    if(hasError)return makeNode(INT,"0");
    left=unary_expr();
    while(!hasError&&match(MULDIV)){
        node=makeNode(MULDIV,getLexeme());advance();
        node->left=left;node->right=unary_expr();left=node;
    }
    return left;
}
static BTNode *addsub_expr(void){
    BTNode *left,*node;
    if(hasError)return makeNode(INT,"0");
    left=muldiv_expr();
    while(!hasError&&match(ADDSUB)){
        node=makeNode(ADDSUB,getLexeme());advance();
        node->left=left;node->right=muldiv_expr();left=node;
    }
    return left;
}
static BTNode *and_expr(void){
    BTNode *left,*node;
    if(hasError)return makeNode(INT,"0");
    left=addsub_expr();
    while(!hasError&&match(AND)){
        node=makeNode(AND,"&");advance();
        node->left=left;node->right=addsub_expr();left=node;
    }
    return left;
}
static BTNode *xor_expr(void){
    BTNode *left,*node;
    if(hasError)return makeNode(INT,"0");
    left=and_expr();
    while(!hasError&&match(XOR)){
        node=makeNode(XOR,"^");advance();
        node->left=left;node->right=and_expr();left=node;
    }
    return left;
}
static BTNode *or_expr(void){
    BTNode *left,*node;
    if(hasError)return makeNode(INT,"0");
    left=xor_expr();
    while(!hasError&&match(OR)){
        node=makeNode(OR,"|");advance();
        node->left=left;node->right=xor_expr();left=node;
    }
    return left;
}

/* finish binary chain when we already consumed an ID */
static BTNode *finish_from_id(BTNode *base){
    BTNode *left=base,*node;
    while(!hasError&&match(MULDIV)){
        node=makeNode(MULDIV,getLexeme());advance();
        node->left=left;node->right=unary_expr();left=node;
    }
    while(!hasError&&match(ADDSUB)){
        node=makeNode(ADDSUB,getLexeme());advance();
        node->left=left;node->right=muldiv_expr();left=node;
    }
    while(!hasError&&match(AND)){
        node=makeNode(AND,"&");advance();
        node->left=left;node->right=addsub_expr();left=node;
    }
    while(!hasError&&match(XOR)){
        node=makeNode(XOR,"^");advance();
        node->left=left;node->right=and_expr();left=node;
    }
    while(!hasError&&match(OR)){
        node=makeNode(OR,"|");advance();
        node->left=left;node->right=xor_expr();left=node;
    }
    return left;
}

static BTNode *assign_expr(void){
    BTNode *node,*lhs;
    char savedLex[MAXLEN],oplex[MAXLEN];
    if(hasError)return makeNode(INT,"0");
    if(match(ID)){
        strcpy(savedLex,getLexeme()); advance();
        if(match(ASSIGN)){
            lhs=makeNode(ID,savedLex); node=makeNode(ASSIGN,"=");
            advance(); node->left=lhs; node->right=assign_expr(); return node;
        }
        if(match(ADDSUB_ASSIGN)){
            strcpy(oplex,getLexeme());
            lhs=makeNode(ID,savedLex); node=makeNode(ADDSUB_ASSIGN,oplex);
            advance(); node->left=lhs; node->right=assign_expr(); return node;
        }
        return finish_from_id(makeNode(ID,savedLex));
    }
    return or_expr();
}

/* ---------- validation ---------- */
static int hasVariable(BTNode *r){
    if(!r)return 0;
    if(r->data==ID)return 1;
    return hasVariable(r->left)||hasVariable(r->right);
}
static int constEval(BTNode *r,int *res){
    int lv,rv;
    if(!r){*res=0;return 1;}
    if(r->data==INT){*res=atoi(r->lexeme);return 1;}
    if(r->data==ID||r->data==INCDEC)return 0;
    if(!constEval(r->left,&lv))return 0;
    if(!constEval(r->right,&rv))return 0;
    switch(r->data){
        case ADDSUB:*res=(r->lexeme[0]=='+')?lv+rv:lv-rv;return 1;
        case MULDIV:
            if(r->lexeme[0]=='*'){*res=lv*rv;return 1;}
            if(rv==0)return 0; *res=lv/rv;return 1;
        case AND:*res=lv&rv;return 1;
        case OR: *res=lv|rv;return 1;
        case XOR:*res=lv^rv;return 1;
        default:return 0;
    }
}
static int validateDiv(BTNode *r){
    int val;
    if(!r)return 0;
    if(r->data==MULDIV&&r->lexeme[0]=='/'){
        if(!hasVariable(r->right)){
            if(!constEval(r->right,&val)||val==0)return 1;
        }
    }
    return validateDiv(r->left)||validateDiv(r->right);
}
static int validateVars(BTNode *r){
    int idx;
    if(!r)return 0;
    if(r->data==ASSIGN){
        BTNode *lhs=r->left;
        if(!lhs||lhs->data!=ID)return 1;
        idx=findSym(lhs->lexeme);
        if(idx<0)idx=addSym(lhs->lexeme);
        if(idx<0)return 1;
        table[idx].defined=1;
        return validateVars(r->right);
    }
    if(r->data==ADDSUB_ASSIGN){
        BTNode *lhs=r->left;
        if(!lhs||lhs->data!=ID)return 1;
        idx=findSym(lhs->lexeme);
        if(idx<0||!table[idx].defined)return 1;
        return validateVars(r->right);
    }
    if(r->data==ID){
        idx=findSym(r->lexeme);
        if(idx<0||!table[idx].defined)return 1;
        return 0;
    }
    if(r->data==INCDEC){
        BTNode *id=r->left;
        if(!id||id->data!=ID)return 1;
        idx=findSym(id->lexeme);
        if(idx<0||!table[idx].defined)return 1;
        return 0;
    }
    return validateVars(r->left)||validateVars(r->right);
}

/* ---------- code generator ---------- */
static int codeGen(BTNode *root){
    int r,lReg,rReg,idx,one;
    BTNode *lhs,*id;
    if(!root||hasError)return 0;
    switch(root->data){
    case INT:
        r=allocReg(); emitMOV_RC(r,atoi(root->lexeme)); return r;
    case ID:
        idx=findSym(root->lexeme);
        r=allocReg(); emitMOV_RM(r,table[idx].memAddr); return r;
    case INCDEC:
        id=root->left; idx=findSym(id->lexeme);
        r=allocReg(); emitMOV_RM(r,table[idx].memAddr);
        one=allocReg(); emitMOV_RC(one,1);
        if(root->lexeme[0]=='+')emitADD(r,one); else emitSUB(r,one);
        freeReg(one); emitMOV_MR(table[idx].memAddr,r); return r;
    case ASSIGN:
        lhs=root->left; idx=findSym(lhs->lexeme);
        rReg=codeGen(root->right);
        if(hasError)return rReg;
        emitMOV_MR(table[idx].memAddr,rReg);
        return rReg;
    case ADDSUB_ASSIGN:
        lhs=root->left; idx=findSym(lhs->lexeme);
        lReg=allocReg(); emitMOV_RM(lReg,table[idx].memAddr);
        rReg=codeGen(root->right);
        if(hasError){freeReg(lReg);return rReg;}
        if(root->lexeme[0]=='+')emitADD(lReg,rReg); else emitSUB(lReg,rReg);
        freeReg(rReg); emitMOV_MR(table[idx].memAddr,lReg); return lReg;
    case ADDSUB: case MULDIV: case AND: case OR: case XOR:
        lReg=codeGen(root->left);
        if(hasError)return lReg;
        rReg=codeGen(root->right);
        if(hasError){freeReg(lReg);return rReg;}
        switch(root->data){
            case ADDSUB: if(root->lexeme[0]=='+')emitADD(lReg,rReg);
                         else emitSUB(lReg,rReg); break;
            case MULDIV: if(root->lexeme[0]=='*')emitMUL(lReg,rReg);
                         else emitDIV(lReg,rReg); break;
            case AND: emitAND(lReg,rReg); break;
            case OR:  emitOR (lReg,rReg); break;
            case XOR: emitXOR(lReg,rReg); break;
            default: break;
        }
        freeReg(rReg); return lReg;
    default: return 0;
    }
}

/* ---------- statement ---------- */
static void statement(void){
    BTNode *tree; int resultReg;
    if(match(ENDFILE)){
        memset(regUsed,0,sizeof(regUsed));
        emitMOV_RM(0,0); emitMOV_RM(1,4); emitMOV_RM(2,8);
        emit("EXIT 0\n");
        printf("%s",outBuf); exit(0);
    }
    if(match(END)){advance();return;}

    memset(regUsed,0,sizeof(regUsed));
    tree=assign_expr();

    if(hasError){freeTree(tree);printf("EXIT 1\n");exit(0);}
    if(!match(END)&&!match(ENDFILE)){
        freeTree(tree); drainLine(); printf("EXIT 1\n"); exit(0);
    }
    if(match(END))advance();

    if(validateVars(tree)){freeTree(tree);printf("EXIT 1\n");exit(0);}
    if(validateDiv(tree)){freeTree(tree);printf("EXIT 1\n");exit(0);}

    memset(regUsed,0,sizeof(regUsed));
    resultReg=codeGen(tree);
    if(hasError){freeTree(tree);printf("EXIT 1\n");exit(0);}
    freeReg(resultReg);
    freeTree(tree);
}

int main(void){
    initTable();
    while(1)statement();
    return 0;
}