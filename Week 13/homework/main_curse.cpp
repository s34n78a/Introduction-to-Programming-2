#include "function_curse.h"
#include <iostream>
#include <string>
using namespace std;

class TermParser
{
public:
    TermParser(const string &src)
        : src(src), pos(0)
    {
    }

    ~TermParser() = default;

    Term *parse()
    {
        Term *t = parseAtom();
        skipWS();
        return t;
    }

private:
    string src;
    int pos;

    void skipWS()
    {
        while (pos < (int)src.size() && isspace(src[pos]))
        {
            pos++;
        }
    }

    Term *parseAtom()
    {
        skipWS();

        // negative number
        if (src[pos] == '-')
        {
            pos++;
            string num;
            while (pos < src.size() && isdigit(src[pos]))
            {
                num += src[pos++];
            }
            return makeNumber(-stoi(num));
        }

        // pos is at the start of a number, start reading number
        if (isdigit(src[pos]))
        {
            string num;
            while (pos < src.size() && isdigit(src[pos]))
            {
                num += src[pos++];
            }
            return makeNumber(stoi(num));
        }

        // pos is at the start of a symbol
        if (isalpha(src[pos]))
        {
            string name;
            while (pos < src.size() && isalnum(src[pos]))
            {
                name += src[pos++];
            }

            skipWS();

            // is a function
            if (pos < src.size() && src[pos] == '(')
            {
                pos++;
                Term *buffer[64];
                int argc = 0;
                skipWS();

                if (pos < (int)src.size() && src[pos] == ')')
                {
                    pos++;
                    return makeFuncCall(name, buffer, 0);
                }

                while (true)
                {
                    buffer[argc++] = parseAtom();

                    skipWS();

                    if (pos < src.size() && src[pos] == ',')
                    {
                        pos++;
                        continue;
                    }
                    if (pos < src.size() && src[pos] == ')')
                    {
                        pos++;
                        break;
                    }

                    for (int i = 0; i < argc; i++)
                    {
                        delete buffer[i];
                    }
                }
                return makeFuncCall(name, buffer, argc);
            }

            // is a variable
            return makeVariable(name);
        }

        // shouldn't happen, input is all valid
        return nullptr;
    }
};

int main()
{
    int n;
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++)
    {
        string line;
        getline(cin, line);

        TermParser p(line);
        Term *input = p.parse();
        Term *result = evaluate(input);

        result->print();
        cout << "\n";

        delete result;
        delete input;
    }
}