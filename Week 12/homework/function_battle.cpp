#include <iostream>

#include "function_battle.h"

void Item::damage(int &hp, int &defense) {
    if (m_weight > defense && hp > 0)
    {
        std::cout << m_name << " hit\n";
        hp -= m_weight - defense;

        if (hp < 0)
        {
            hp = 0;
        }
    }
    else
    {
        std::cout << m_name << " blocked\n";
        return;
    }
}

Laptop::Laptop(int weight, int power) : m_power(power), Item("Laptop", weight) {}

void Laptop::use(int &hp, int &defense) { // first damage, then decrease hp by [power] regardless of defense
    damage(hp, defense);
    if (hp <= 0)
    {
        return;
    }
    
    hp -= m_power;
    
    if (hp < 0)
    {
        hp = 0;
    }
}
    
Notebook::Notebook(int weight, int notes) : m_notes(notes), Item("Notebook", weight) {}

void Notebook::use(int &hp, int &defense) { // first damage, then lower defense by [notes]
    damage(hp, defense);
    if (hp <= 0)
    {
        return;
    }

    defense -= m_notes;

    if (defense < 0)
    {
        defense = 0;
    }

}
    
GPU::GPU(int weight) : Item("GPU", weight) {}

void GPU::use(int &hp, int &defense) { // first damage, then decrease hp by [defense] regardless of defense
    damage(hp, defense);
    if (hp <= 0)
    {
        return;
    }

    hp -= defense;

    if (hp < 0)
    {
        hp = 0;
    }
}
    
Shoe::Shoe(int weight, int smell) : m_smell(smell), Item("Shoe", weight) {}

void Shoe::use(int &hp, int &defense) { // first damage, then hp and defense both reduced by [smell]
    damage(hp, defense);
    if (hp <= 0)
    {
        return;
    }

    hp -= m_smell;
    defense -= m_smell;

    if (hp < 0)
    {
        hp = 0;
    }

    if (defense < 0)
    {
        defense = 0;
    }
}
    