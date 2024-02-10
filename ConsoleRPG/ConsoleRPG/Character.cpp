#include <iostream>
#include "Character.h"

// Typing: 
// Normal doesn't take or deal bonus damage 
// Flying beats Ground
// Ground beats Fire
// Fire beats Water
// Water beats Flying
Character::Character(const std::string& name, const int& hp, const int attack, const Types type) : name(name), hp(hp), maxHp(hp), attack(attack), type(type) {}

    void Character::displayInfo() const {
        std::cout << name << std::endl;
    }

    void Character::normalAttack(Character& target) {
        target.takeDamage(attack);
    }

    void Character::takeDamage(int damage) {
        hp -= damage;
        if (hp <= 0)
        {
            isDead = true;
        }

        displayAttackInfo(name, damage, hp);
    }

    void Character::resetHp() {
        hp = maxHp;
    }

    void Character::displayAttackInfo(std::string& name, int damage, int hp) {
        std::cout << "hit " << name << " for " << damage << ", " << hp << " left" << std::endl;
    }
