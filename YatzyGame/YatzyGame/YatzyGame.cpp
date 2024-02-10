// YatzyGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include "DiceRollClass.h"

void printScore(std::map<int, int>& input) {
	std::cout << "Score> ";

	for  (auto var : input)
	{
		if (var.second == -1)
		{

		} else
		{
			std::cout << var.first << ":" << var.second << " ";
		}
	}

	std::cout << "<" << std::endl;
}

void MiniYatzy() {
	DiceRollClass diceRollClass;
	std::map<int, int> score;

	for (size_t i = 1; i < 7; i++)
	{
		score.insert({ i, -1 });
	}

	std::map<int, int> amount;
	
	for (size_t i = 1; i < 7; i++)
	{
		amount.insert({ i, 0 });
	}

	int counter = 0;
	while (counter < 6)
	{
		for (size_t i = 1; i < 7; i++)
		{
			amount.at(i) = 0;
		}

		counter++;
		std::vector<int> rolledDice;

		diceRollClass.DiceRoll(rolledDice);

		std::cout << "You threw: ";
		for (auto it = rolledDice.begin(); it != rolledDice.end(); it++) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;

		int numberChoice;
		std::cout << "Select one of the numbers you want to fill in" << std::endl;
		std::cin >> numberChoice;
		
		bool isNotEmpty = score.at(numberChoice) != -1;

		while (!std::cin.good() || numberChoice < 1 || numberChoice > 6 || isNotEmpty)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (isNotEmpty)
			{
				std::cout << "Please Pick another number that hasn't been chosen before!" << std::endl;
			}
			else
			{
				std::cout << "Please only enter numbers between 1 and 6!" << std::endl;
			}
			std::cin >> numberChoice;
			isNotEmpty = score.at(numberChoice) != -1;
		}
		
		for (size_t i = 0; i < rolledDice.size(); i++)
		{
			++amount.at(rolledDice[i]);
		}

		
		score.at(numberChoice) = amount.at(numberChoice);

		printScore(score);
	}

	int sum = 0;

	for (size_t i = 1; i < score.size(); i++)
	{
		sum += score.at(i);
	}

	std::cout << "You final score is: " << sum << std::endl;
}

int main()
{
	bool ongoing = true;
	int menuSelect;

	while (ongoing)
	{
		std::cout << "Welcome to MiniYatzy!" << std::endl;
		std::cout << "Please pick one of the options listed below:" << std::endl;
		std::cout << " 1) start MiniYatzy" << std::endl;
		std::cout << " 0) quit" << std::endl;
		std::cin >> menuSelect;

		while (!std::cin.good())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Please only enter numbers!" << std::endl;
			std::cin >> menuSelect;
		}

		if (menuSelect == 1)
		{
			
			std::cout << "Starting, try to get the most numbers of a kind!" << std::endl;
			
			MiniYatzy();

		}
		else if (menuSelect == 0) {
			ongoing = false;
		}
	}
}