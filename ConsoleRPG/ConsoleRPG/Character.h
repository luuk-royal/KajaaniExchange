#pragma once
#include "Types.h"
class Character
{
public:
    Character(const std::string& name, const int& hp, const int attack, const Types type);

    virtual void displayInfo() const;
    void normalAttack(Character& target);
    void takeDamage(int damage);
    void resetHp();

    bool isDead = false;
    std::string name;
    Types type;
    int hp;
    int maxHp;
    int attack;

private:
    void displayAttackInfo(std::string& name, int damage, int hp);
};

