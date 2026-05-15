#include <iostream>

#include "function_battle.h"

int main()
{
    int hp, defense, n;
    std::cin >> hp >> defense >> n;

    while (n-- && hp > 0)
    {
        int idx, weight;
        std::cin >> idx >> weight;

        if (idx == 0)
        {
            int power;
            std::cin >> power;

            Laptop laptop(weight, power);
            laptop.use(hp, defense);
        }
        else if (idx == 1)
        {
            int notes;
            std::cin >> notes;

            Notebook notebook(weight, notes);
            notebook.use(hp, defense);
        }
        else if (idx == 2)
        {
            GPU gpu(weight);
            gpu.use(hp, defense);
        }
        else if (idx == 3)
        {
            int smell;
            std::cin >> smell;

            Shoe shoe(weight, smell);
            shoe.use(hp, defense);
        }

        std::cout << "HP: " << hp << ", Defense: " << defense << '\n';
    }
}