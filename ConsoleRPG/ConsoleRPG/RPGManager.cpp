#include <iostream>
#include <vector>
#include "RPGManager.h"
// Give all other characters seperate files if theres time
#include "Character.h"
#include "Types.h"


// Takes more damage from water type
class FlyingCharacter : public Character {
public:
    FlyingCharacter(const std::string& name, const int& hp, const int& attack) : Character(name, hp, attack, Flying) {}

    void flyAttack(Character& target) const {
        if (target.type == Ground)
        {
            target.takeDamage(attack * 1.5);
        }
        else {
            target.takeDamage(attack);
        }
    }

    void listAttacks() {
        std::cout << "1) Normal attack - " << std::endl << "2) Flying attack" << std::endl;
    }
};

// Takes more damage from fire type
class WaterCharacter : public Character {
public:
    WaterCharacter(const std::string& name, const int& hp, const int& attack) : Character(name, hp, attack, Water) {}

    void waterAttack(Character& target) const {
        if (target.type == Flying)
        {
            target.takeDamage(attack * 1.5);
        }
        else {
            target.takeDamage(attack);
        }
    }

    void listAttacks() {
        std::cout << "1) Normal attack - " << std::endl << "2) Water attack" << std::endl;
    }
};

// Takes more damage from flying type
class GroundCharacter : public Character {
public:
    GroundCharacter(const std::string& name, const int& hp, const int& attack) : Character(name, hp, attack, Ground) {}

    void slamAttack(Character& target) const {
        if (target.type == Electric)
        {
            target.takeDamage(attack * 1.5);
        }
        else {
            target.takeDamage(attack);
        }
    }

    void listAttacks() {
        std::cout << "1) Normal attack - " << std::endl << "2) Slam attack" << std::endl;
    }
};

// Takes more damage from ground type
class ElectricCharacter : public Character {
public:
    ElectricCharacter(const std::string& name, const int& hp, const int& attack) : Character(name, hp, attack, Electric) {}

    void ShockingAttack(Character& target) const {
        if (target.type == Water)
        {
            target.takeDamage(attack * 1.5);
        }
        else {
            target.takeDamage(attack);
        }
    }

    void listAttacks() {
        std::cout << "1) Normal attack - " << std::endl << "2) Shocking attack" << std::endl;
    }
};


void RPGManager::runMainGameLoop()
{
    // The game is ongoing right now
    ongoing = true;

    // Clear the screen to start the rpg
    system("CLS");

    // Create the starting Party, is only one character at the moment.
    auto hero = mainCharacterSelect();

    while (ongoing) {
        hero.resetHp();
        battle(hero);
    }
}

void RPGManager::battle(Character& character) {
    Character enemy = Character("AmberWolf", (rand() % 6) + 1, (rand() % 6) + 1, Normal);
    int battleInput;
    bool ongoing = true;
    
    std::cout << "You are facing: ";
    enemy.displayInfo();
    std::cout << std::endl;
    
    // This isn't optimal or clean, but i can't figure out a way around it
    if (Flying)
    {
        FlyingCharacter& heroCharacter = dynamic_cast<FlyingCharacter&>(character);

        while (ongoing)
        {
            std::cout << "What will you do?" << std::endl;
            heroCharacter.listAttacks();

            std::cin >> battleInput;

            while (!std::cin.good() || battleInput < 1 || battleInput > 2)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please Select ability 1 or 2!" << std::endl;
                std::cin >> battleInput;
            }

            if (1)
            {
                heroCharacter.normalAttack(enemy);
            }
            else
            {
                heroCharacter.flyAttack(enemy);
            }

            if (enemy.isDead)
            {
                std::cout << "Congratulations you defeated " << enemy.name << "!" << std::endl;
                break;
            }

            std::cout << enemy.name << " ";
            enemy.normalAttack(heroCharacter);

            if (heroCharacter.isDead)
            {
                std::cout << "You were defeated, better luck next time!" << std::endl;
                break;
            }
        }
        
    }
    else if (Ground)
    {
        GroundCharacter& heroCharacter = dynamic_cast<GroundCharacter&>(character);

        while (ongoing)
        {
            std::cout << "What will you do?" << std::endl;
            heroCharacter.listAttacks();

            std::cin >> battleInput;

            while (!std::cin.good() || battleInput < 1 || battleInput > 2)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please Select ability 1 or 2!" << std::endl;
                std::cin >> battleInput;
            }

            if (1)
            {
                heroCharacter.normalAttack(enemy);
            }
            else
            {
                heroCharacter.slamAttack(enemy);
            }

            if (enemy.isDead)
            {
                std::cout << "Congratulations you defeated " << enemy.name << "!" << std::endl;
                break;
            }

            std::cout << enemy.name << " ";
            enemy.normalAttack(heroCharacter);

            if (heroCharacter.isDead)
            {
                std::cout << "You were defeated, better luck next time!" << std::endl;
                break;
            }
        }
    }
    else if (Electric) 
    {
        ElectricCharacter& heroCharacter = dynamic_cast<ElectricCharacter&>(character);

        while (ongoing)
        {
            std::cout << "What will you do?" << std::endl;
            heroCharacter.listAttacks();

            std::cin >> battleInput;

            while (!std::cin.good() || battleInput < 1 || battleInput > 2)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please Select ability 1 or 2!" << std::endl;
                std::cin >> battleInput;
            }

            if (1)
            {
                heroCharacter.normalAttack(enemy);
            }
            else
            {
                heroCharacter.ShockingAttack(enemy);
            }

            if (enemy.isDead)
            {
                std::cout << "Congratulations you defeated " << enemy.name << "!" << std::endl;
                break;
            }

            std::cout << enemy.name << " ";
            enemy.normalAttack(heroCharacter);

            if (heroCharacter.isDead)
            {
                std::cout << "You were defeated, better luck next time!" << std::endl;
                break;
            }
        }
    }
    else if (Water)
    {
        WaterCharacter& heroCharacter = dynamic_cast<WaterCharacter&>(character);

        while (ongoing)
        {
            std::cout << "What will you do?" << std::endl;
            heroCharacter.listAttacks();

            std::cin >> battleInput;

            while (!std::cin.good() || battleInput < 1 || battleInput > 2)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please Select ability 1 or 2!" << std::endl;
                std::cin >> battleInput;
            }

            if (1)
            {
                heroCharacter.normalAttack(enemy);
            }
            else
            {
                heroCharacter.waterAttack(enemy);
            }

            if (enemy.isDead)
            {
                std::cout << "Congratulations you defeated " << enemy.name << "!" << std::endl;
                break;
            }

            std::cout << enemy.name << " ";
            enemy.normalAttack(heroCharacter);

            if (heroCharacter.isDead)
            {
                std::cout << "You were defeated, better luck next time!" << std::endl;
                break;
            }
        }
    }
    else {
        Character& heroCharacter = character;

        while (ongoing)
        {
            std::cout << "What will you do?" << std::endl << "1) Normal attack" << std::endl;

            std::cin >> battleInput;

            while (!std::cin.good() || battleInput != 1)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please Select ability 1!" << std::endl;
                std::cin >> battleInput;
            }

            heroCharacter.normalAttack(enemy);

            if (enemy.isDead)
            {
                std::cout << "Congratulations you defeated " << enemy.name << "!" << std::endl;
                break;
            }

            std::cout << enemy.name << " ";
            enemy.normalAttack(heroCharacter);

            if (heroCharacter.isDead)
            {
                std::cout << "You were defeated, better luck next time!" << std::endl;
                break;
            }
        }
    }
}

// Will be callable during the main game loop to check typing.
void RPGManager::listTypes() {
    int listInput;
    std::cout << "Typing: " << std::endl;
    std::cout << "Normal doesn't take or deal bonus damage" << std::endl;
    std::cout << "Flying beats Ground" << std::endl;
    std::cout << "Ground beats Electricity" << std::endl;
    std::cout << "Electricity beats Water" << std::endl;
    std::cout << "Water beats Flying" << std::endl;

    std::cout << std::endl << "Please press 1 to continue" << std::endl;
    std::cin >> listInput;

    while (!std::cin.good() || listInput != 1)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please only enter the number 1 to continue!" << std::endl;
        std::cin >> listInput;
    }

    system("CLS");
}

Character& RPGManager::mainCharacterSelect() {
    int mcSelect;
    std::string mcName;
    bool repeat = true;

    while (repeat)
    {
        // Give options to select a starting character.
        std::cout << "Welcome to the party menu, please select your starter character:" << std::endl;
        std::cout << "Please pick one of the options listed below:" << std::endl;
        std::cout << " 1) check types" << std::endl;
        std::cout << " 2) Normal type" << std::endl;
        std::cout << " 3) Flying type" << std::endl;
        std::cout << " 4) Ground type" << std::endl;
        std::cout << " 5) Fire type" << std::endl;
        std::cout << " 6) Water type" << std::endl;
        std::cout << " 0) cancel character creation" << std::endl;
        std::cin >> mcSelect;
    
        if (mcSelect == 1)
        {
            listTypes();
        }

        if (mcSelect != 1)
        {
            repeat = false;
        }
    }

    while (!std::cin.good() || mcSelect > 6 || mcSelect < 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please only enter numbers between 0-6!" << std::endl;
        std::cin >> mcSelect;
    }

    std::cout << "Please enter the name of your character" << std::endl;
    std::cin >> mcName;

    while (!std::cin.good())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please only enter numbers between 0-6!" << std::endl;
        std::cin >> mcName;
    }

    // Avoid nested else if
    //https://stackoverflow.com/questions/5313322/c-cast-to-derived-class
    if (mcSelect == 2)
    {
        Character character = Character(mcName, 10, 2, Normal);
        return character;
    }

    if (mcSelect == 3)
    {
        FlyingCharacter character = FlyingCharacter(mcName, 10, Flying);
        return character; 
    }

    if (mcSelect == 4)
    {
        GroundCharacter character = GroundCharacter(mcName, 10, Flying);
        return character;
    }

    if (mcSelect == 5)
    {
        ElectricCharacter character = ElectricCharacter(mcName, 10, Flying);
        return character;
    }

    if (mcSelect == 6)
    {
        WaterCharacter character = WaterCharacter(mcName, 10, Flying);
        return character;
    }

    if (mcSelect == 0)
    {
        // Quit back to menu
        ongoing = false;
    }
}

// Add more characters to party if time allows
//std::vector<Character> RPGManager::partySelect()
//{
//    int partySelect;
//
//    // Give options to select a starting character.
//    std::cout << "Welcome to the party menu, please select your starter character:" << std::endl;
//    std::cout << "Please pick one of the options listed below:" << std::endl;
//    std::cout << " 1) check types" << std::endl;
//    std::cout << " 2) Normal type" << std::endl;
//    std::cout << " 3) Flying type" << std::endl;
//    std::cout << " 4) Ground type" << std::endl;
//    std::cout << " 5) Fire type" << std::endl;
//    std::cout << " 6) Water type" << std::endl;
//    std::cout << " 0) cancel character creation" << std::endl;
//    std::cin >> partySelect;
//
//    while (!std::cin.good() || partySelect > 6 || partySelect < 0)
//    {
//        std::cin.clear();
//        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//        std::cout << "Please only enter numbers between 0-6!" << std::endl;
//        std::cin >> partySelect;
//    }
//    // Avoid nested else if
//    if (partySelect == 1)
//    {
//        listTypes();
//    }
//
//    if (partySelect == 2)
//    {
//        listTypes();
//    }
//
//    if (partySelect == 3)
//    {
//        listTypes();
//    }
//
//    if (partySelect == 4)
//    {
//        listTypes();
//    }
//
//    if (partySelect == 5)
//    {
//        listTypes();
//    }
//
//    if (partySelect == 6)
//    {
//        listTypes();
//    }
//
//    if (partySelect == 0)
//    {
//        // Quit back to menu
//        ongoing = false;
//    }
//
//    return std::vector<Character>();
//}
