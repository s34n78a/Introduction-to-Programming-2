#include <iostream>
#include <algorithm>
#include <string>

class Item
{
public:
    Item(const std::string &name, int weight)
        : m_name(name), m_weight(weight)
    {
    }
    ~Item() = default;

    void damage(int &hp, int &defense);

private:
    const std::string m_name;
    const int m_weight;
};

class Laptop : public Item
{
public:
    Laptop(int weight, int power);
    ~Laptop() = default;
    void use(int &hp, int &defense); // first damage, then decrease hp by [power] regardless of defense
private:
    int m_power;
};

class Notebook : public Item
{
public:
    Notebook(int weight, int notes);
    ~Notebook() = default;
    void use(int &hp, int &defense); // first damage, then lower defense by [notes]
private:
    int m_notes;
};

class GPU : public Item
{
public:
    GPU(int weight);
    ~GPU() = default;
    void use(int &hp, int &defense); // first damage, then decrease hp by [defense] regardless of defense
};

class Shoe : public Item
{
public:
    Shoe(int weight, int smell);
    ~Shoe() = default;
    void use(int &hp, int &defense); // first damage, then hp and defense both reduced by [smell]
private:
    int m_smell;
};