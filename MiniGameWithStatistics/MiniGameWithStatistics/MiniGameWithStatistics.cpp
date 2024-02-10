// MiniGameWithStatistics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
// https://github.com/nlohmann/json version in project: 3.11.2
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

struct Score {
	string name;
	int highScore;
};

std::vector<Score> scores;

void readJsonFile() {
	std::ifstream f("scores.json");
	if (!f)
	{
		throw std::exception("Could not open file, does the file exist?");
	}
	else {
		json data = json::parse(f);

		for (json var : data["scores"])
		{
			if (!var["name"].is_string())
			{
				throw std::exception("Wrong input format, check answer of question: " + var["question"]);
			}

			if (!var["highScore"].is_number_integer())
			{
				throw std::exception("Wrong input format, check answer of question: " + var["question"]);
			}
			scores.push_back({ var["name"], var["highScore"] });
		}
	}
}

void writeJsonFile(vector<Score>& writeScores) {
	json writeObject = json::array();

	for (Score score : writeScores)
	{
		writeObject.push_back({ { "name", score.name }, { "highScore", score.highScore} });
	}
	
	std::ofstream o("scores.json");
	if (!o)
	{
		throw std::exception("Could not open file, does the file exist?");
	}
	else {
		o << "{ \"scores\": " << writeObject << " }" << endl;
	}
}

int main()
{
	try
	{
		readJsonFile();
	}
	catch (const std::exception& e)
	{
		e.what();
	}

	bool gameOn = true;
	int guess;
	int highScore = 0;
	
	while (gameOn) {
		int randomNumber = (rand() % 2) + 1;
		// Debug only:
		// cout << randomNumber << endl;

		cout << "Guess 1 or 2 untill you fail!" << endl;
		cin >> guess;
		while (!cin.good())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please only enter numbers!" << endl;
			cin >> guess;
		}

		if (guess < 1 || guess > 2)
		{
			cout << "Please only enter 1 or 2!" << endl;
		}
		else
		{
			if (guess == randomNumber)
			{
				cout << "You guessed correct!" << endl;
				highScore += 1;
			}
			else {
				cout << "You guessed wrong!" << endl;
				gameOn = false;
			}
		}
	}

	string name;
	cout << "Please enter your name" << endl;
	cin >> name;

	Score finalScore = {name, highScore};

	scores.push_back(finalScore);
	
	cout << "\nHigh Scores: \n";
	Score temp;

	if (scores.size() == 0)
	{

	} else 
	{
		for (size_t i = 0; i < scores.size(); i++)
		{
			for (size_t j = i + 1; j < scores.size(); j++)
			{
				if (scores[i].highScore < scores[j].highScore) {
					temp = scores[i];
					scores[i] = scores[j];
					scores[j] = temp;
				}
			}
		}
	}

	if (scores.size() < 10)
	{
		for (size_t i = 0; i < scores.size(); i++)
		{
			cout << i << "> " << scores[i].name << ": " << scores[i].highScore << endl;
		}
	} else
	{
		for (size_t i = 0; i < 10; i++)
		{
			cout << i << "> " << scores[i].name << ": " << scores[i].highScore << endl;
		}
	}

	try
	{
		writeJsonFile(scores);
	}
	catch (const std::exception& e)
	{
		e.what();
	}

	bool searchScore = true;
	while (searchScore) {
		cout << "1: Find score by name" << endl;
		cout << "2: Find name by score" << endl;
		cout << "3: Exit game" << endl;
		cin >> guess;
		while (!cin.good())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please only enter numbers!" << endl;
			cin >> guess;
		}

		if (guess < 1 || guess > 3)
		{
			cout << "Please only enter 1 or 2!" << endl;
		}
		else
		{
			if (guess == 1)
			{
				cout << "What name to search for?" << endl;
				cin >> name;
				while (!cin.good())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Please only valid text" << endl;
					cin >> name;
				}

				cout << "Results:" << endl;

				for  (Score var : scores)
				{
					if (var.name == name)
					{
						cout << var.name << ": " << var.highScore << endl;
					}
				}

				cout << "End of Results:" << endl;

			}
			else if (guess == 2)
			{
				cout << "What score to search for?" << endl;
				cin >> guess;
				while (!cin.good())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Please only enter numbers!" << endl;
					cin >> guess;
				}

				cout << "Results:" << endl;

				for (Score var : scores)
				{
					if (var.highScore == guess)
					{
						cout << var.name << ": " << var.highScore << endl;
					}
				}

				cout << "End of Results:" << endl;
			}
			else if (guess == 3) {
				searchScore = false;
			}
			else {
				cout << "please enter a number between 1 and 3!" << endl;
			}
		}
	}

	return 0;
}
