#include <iostream>
#include <set>
#include <map>
#include <algorithm> 
#include <vector>
#include <ctime>
#include <cstdlib>

// A simple memory game where you see the board at the start and you then have to remember all of the numbers and where they are.
std::map<int, int> InitializeGameBoard(const bool& display) {
    std::map<int, int> gameBoard;
    std::vector<int> cards;

    if (display)
    {
        for (int i = 1; i <= 10; i++) {
            cards.push_back(-1);
            cards.push_back(-1);
        }
    }
    else {
        for (int i = 1; i <= 10; i++) {
                cards.push_back(i);
                cards.push_back(i);
         }

        // Shuffle the cards
        srand(static_cast<unsigned int>(time(nullptr)));
        std::random_shuffle(cards.begin(), cards.end());
    }

    // Populate the game board
    for (int i = 0; i < cards.size(); i++) {
        gameBoard.insert(std::pair<int, int>(i + 1, cards[i]));
    }

    return gameBoard;
}

// Function to display the game board
void DisplayGameBoard(const std::map<int, int>& gameBoard) {
    std::cout << "Game Board: " << std::endl;
    int count = 0;
    for (const auto& card : gameBoard) {
        if (count == 5)
        {
            std::cout << std::endl;
            count = 0;
        }
        if (card.first < 10)
        {
            std::cout << "| " << card.first << ": ";
        }
        else {
            std::cout << "|" << card.first << ": ";
        }
        if (card.second == -1) {
            std::cout << "X ";
        }
        else {
            std::cout << card.second << " ";
        }
        if (card.second < 10)
        {
            std::cout << " ";
        }
        count++;
    }
    std::cout << std::endl;
}

int main() {
    auto gameBoard = InitializeGameBoard(false);
    auto displayBoard = InitializeGameBoard(true);
    DisplayGameBoard(gameBoard);
    DisplayGameBoard(displayBoard);

    int tries = 5;
    int correctAnswers = 0;
    bool gameEnd = false;

    while (!gameEnd) {
        int number;
        int numberTwo;
        std::cout << "Please enter a number between (1-20): " << std::endl;
        std::cin >> number;
        std::cout << "Please enter the second number between (1-20)!" << std::endl;
        std::cin >> numberTwo;

        bool invalidInput = true;
        while (invalidInput)
        {
            if (number < 1 || number > 20) {
                std::cout << "Invalid number! Please enter a number between 1 and 20." << std::endl;
                std::cout << "Please enter a number between (1-20): " << std::endl;
                std::cin >> number;
            }
            else
            {
                invalidInput = false;
            }

            if (numberTwo < 1 || numberTwo > 20) {
                std::cout << "Invalid number! Please enter a number between 1 and 20." << std::endl;
                std::cout << "Please enter the second number between (1-20)!" << std::endl;
                std::cin >> numberTwo;
            }
            else
            {
                invalidInput = false;
            }

            if (number == numberTwo) {
                std::cout << "Invalid number! Please do not enter the same numbers!" << std::endl;
                std::cout << "Please enter a number between (1-20): " << std::endl;
                std::cin >> number;
                std::cout << "Please enter the second number between (1-20)!" << std::endl;
                std::cin >> numberTwo;
            }
        }

        if (displayBoard[number] != -1) {
            tries--;
            std::cout << "Wrong Answer! " << tries << " tries remaining." << std::endl;
        }
        else if (displayBoard[numberTwo] != -1) {
            tries--;
            std::cout << "Wrong Answer! " << tries << " tries remaining." << std::endl;
        }
        
        if (gameBoard[number] == gameBoard[numberTwo])
        {
            std::cout << "You have guessed correct!" << std::endl;
            correctAnswers++;
            displayBoard[number] = gameBoard[number];
            displayBoard[numberTwo] = gameBoard[numberTwo];

            DisplayGameBoard(displayBoard);

            if (correctAnswers >= 19)
            {
                std::cout << "You have won the game with " << tries << " tries remaining!" << std::endl;
                gameEnd = true;
            }
        }
        else {
            tries--;
            std::cout << "Wrong Answer! " << tries << " tries remaining." << std::endl;
            displayBoard[number] = gameBoard[number];
            displayBoard[numberTwo] = gameBoard[numberTwo];
            DisplayGameBoard(displayBoard);

            displayBoard[number] = -1;
            displayBoard[numberTwo] = -1;

            DisplayGameBoard(displayBoard);
        }

        if (tries <= 0)
        {
            gameEnd = true;
        }
    }

    return 0;
}