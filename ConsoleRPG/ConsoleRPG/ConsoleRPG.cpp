#include <iostream>
#include "RPGManager.h"

int main() {
    bool gameAlive = true;
    int menuSelect;
    RPGManager gameManager = RPGManager();

    while (gameAlive)
    {
        std::cout << "Welcome to ConsoleRPG!" << std::endl;
        std::cout << "Please pick one of the options listed below:" << std::endl;
        std::cout << " 1) start ConsoleRPG" << std::endl;
        std::cout << " 2) check types" << std::endl;
        std::cout << " 0) quit" << std::endl;
        std::cin >> menuSelect;

        while (!std::cin.good() || menuSelect > 2 || menuSelect < 0)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please only enter numbers between 0-2!" << std::endl;
            std::cin >> menuSelect;
        }

        // Avoid nested else if
        if (menuSelect == 1)
        {
            gameManager.runMainGameLoop();
        }
        
        if (menuSelect == 2)
        {
            gameManager.listTypes();
        }
        
        if (menuSelect == 0)
        {
            gameAlive = false;
        }
    }

    return 0;
}