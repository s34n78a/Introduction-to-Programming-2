#pragma once
#include <string>
using namespace std;

class Term
{
public:
    virtual ~Term() = default;
    virtual Term *clone() const = 0;
    virtual void print() const = 0;
    virtual bool isNumber() const { return false; }
    virtual bool isFuncCall() const { return false; }
    virtual int asInt() const { return 0; }
    virtual string asStr() const { return ""; }
    virtual int arity() const { return 0; }
    virtual Term *arg(int i) const { return NULL; }
};

Term *makeNumber(int v);
Term *makeVariable(const string &name);
Term *makeFuncCall(const string &func, Term **args, int n);

Term *evaluate(const Term *t);