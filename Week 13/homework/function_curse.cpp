#include "function_curse.h"
#include <iostream>
#include <string>

#include <cassert>

using namespace std;

class NumberTerm : public Term {
    int v;
public:
    explicit NumberTerm(int v) : v(v) {}
    Term *clone() const { return new NumberTerm(v); }
    void print() const { cout << v; }
    bool isNumber() const { return true; }
    int asInt() const { return v; }
};

class VariableTerm : public Term {
    string name;
public:
    explicit VariableTerm(const string &s) : name(s) {}
    Term *clone() const { return new VariableTerm(name); }
    void print() const { cout << name; }
    string asStr() const { return name; }
};

class FuncCallTerm : public Term {
    string func;
    Term **children;
    int n;
public:
    FuncCallTerm(const string &f, Term **args, int cnt) : func(f), children(NULL), n(cnt) {
        if (n > 0) {
            children = new Term*[n];
            for (int i = 0; i < n; ++i) children[i] = args[i];
        } else {
            children = NULL;
        }
    }

    ~FuncCallTerm() {
        for (int i = 0; i < n; ++i) delete children[i];
        delete[] children;
    }

    Term *clone() const {
        Term **tmp = NULL;
        if (n > 0) {
            tmp = new Term*[n];
            for (int i = 0; i < n; ++i) tmp[i] = children[i]->clone();
        }
        Term *res = new FuncCallTerm(func, tmp, n);
        delete[] tmp;
        return res;
    }

    void print() const {
        cout << func << "(";
        for (int i = 0; i < n; ++i) {
            children[i]->print();
            if (i + 1 < n) cout << ",";
        }
        cout << ")";
    }

    bool isFuncCall() const { return true; }
    string asStr() const { return func; }
    int arity() const { return n; }
    Term *arg(int i) const { assert(i >= 0 && i < n); return children[i]; }
};

Term *makeNumber(int v) {
    return new NumberTerm(v);
}

Term *makeVariable(const string &name) {
    return new VariableTerm(name);
}

Term *makeFuncCall(const string &func, Term **args, int n) {
    return new FuncCallTerm(func, args, n);
}

Term *evaluate(const Term *t) {
    if (!t->isFuncCall()) {
        return t->clone();
    }

    int n = t->arity();
    Term **evaluated = NULL;
    if (n > 0) {
        evaluated = new Term*[n];
        for (int i = 0; i < n; ++i) evaluated[i] = evaluate(t->arg(i));
    }

    bool allNum = true;
    for (int i = 0; i < n; ++i) {
        if (!evaluated[i]->isNumber()) { allNum = false; break; }
    }

    string f = t->asStr();
    if (allNum) {
        if (f == "add") {
            long long sum = 0;
            for (int i = 0; i < n; ++i) { sum += evaluated[i]->asInt(); delete evaluated[i]; }
            delete[] evaluated;
            return makeNumber((int)sum);
        } else if (f == "mul") {
            long long prod = 1;
            for (int i = 0; i < n; ++i) { prod *= evaluated[i]->asInt(); delete evaluated[i]; }
            delete[] evaluated;
            return makeNumber((int)prod);
        }
    }

    // Not all numbers: build a new function call from evaluated children
    Term *res = makeFuncCall(f, evaluated, n);
    // makeFuncCall copied the pointers into its own storage; free the temporary array
    delete[] evaluated;
    return res;
}
