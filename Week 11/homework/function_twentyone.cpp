#include <iostream>
#include <string>
#include "function_twentyone.h"
using namespace std;

Guest::Guest() {
    name = "";
    money = 0;
    skill = 0;
}

Guest::Guest(string s, int m, int ski) {
    name = s;
    money = m;
    skill = ski;
}

Guest::~Guest() {
    name = "";
    money = 0;
    skill = 0;
}

string Guest::get_name() {
    return name;
}

int Guest::get_money() {
    return money;
}

int Guest::get_skill() {
    return skill;   
}

void Guest::Win(int m) {
    money += m;
}

Casino::Casino() {
    fee = 0;
    guest_num = 0;
    income = 0;
    list_num = 0;
    for (int i = 0; i < 1009; i++)
    {
        guest[i] = NULL;
        blacklist[i] = "";
    }
}

Casino::~Casino() {
    for (int i = 0; i < guest_num; i++)
    {
        delete guest[i];
        guest[i] = NULL;
    }

    for (int i = 0; i < list_num; i++)
    {
        blacklist[i] = "";
    }
}

void Casino::Enterance(int f) {
    fee = f;
}

void Casino::GuestEnter(string s, int m, int ski) {
    for (int i = 0; i < list_num; i++)
    {
        if (blacklist[i] == s)
        {
            return;
        }
    }
    
    for (int i = 0; i < guest_num; i++)
    {
        if (guest[i] != NULL && guest[i]->get_name() == s)
        {
            return;
        }
    }

    if (m <= fee)
    {
        blacklist[list_num] = s;
        list_num++;
        income += m;
        return;
    }
    
    guest[guest_num] = new Guest(s, m-fee, ski);
    guest_num++;
    income += fee;
}

void Casino::Win(string s, int m) {
    for (int i = 0; i < list_num; i++)
    {
        if (blacklist[i] == s)
        {
            return;
        }
    }

    int guest_index = -1;
    for (int i = 0; i < guest_num; i++)
    {
        if (guest[i] != NULL && guest[i]->get_name() == s)
        {
            guest_index = i;
            break;
        }
    }
    
    if (guest_index == -1)
    {
        return;
    }

    if (guest[guest_index]->get_money() <= - m)
    {
        income += guest[guest_index]->get_money();
        delete guest[guest_index];
        for (int i = guest_index; i < guest_num - 1; i++)        {
            guest[i] = guest[i+1];
        }
        guest_num --;
        guest[guest_num] = NULL;

        blacklist[list_num] = s;
        list_num++;
        return;
    }
    

    guest[guest_index]->Win(m);
    income -= m;

    // Check if cheating (positive win exceeding 2x skill)
    if (m > 0 && m > guest[guest_index]->get_skill()*2)
    {
        delete guest[guest_index];
        for (int i = guest_index; i < guest_num - 1; i++)
        {
            guest[i] = guest[i+1];
        }
        guest_num --;
        guest[guest_num] = NULL;

        blacklist[list_num] = s;
        list_num++;
    }
}

void Casino::EndDay() {
    for (int i = 0; i < guest_num; i++)
    {
        delete guest[i];
        guest[i] = NULL;
    }
    guest_num = 0;

    //cout << income << endl;
}

void Casino::Result() {
    cout << income << endl;
    
    for (int i = 0; i < list_num; ++ i) {
        cout << blacklist[i] << endl;
    }
}
