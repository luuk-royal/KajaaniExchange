// CPPCourse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string> // Added for to_string
using namespace std;

// https://stackoverflow.com/questions/2393345/how-to-append-text-to-a-text-file-in-c
static void appendLineToFile(string filepath, string line)
{
	std::ofstream file;
	//can't enable exception now because of gcc bug that raises ios_base::failure with useless message
	//file.exceptions(file.exceptions() | std::ios::failbit);
	file.open(filepath, std::ios::out | std::ios::app);
	if (file.fail())
		throw std::ios_base::failure("Reading the file failed!");

	//make sure write fails with exception if something is wrong
	file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

	file << line << std::endl;
}

void runGame() {
	// State that the game is ongoing for the game logic loop
	bool onGoing = true;
	// The amount of tries, hardcoded for now
	int tries = 5;
	// The random generation, do + 1 to prevent 0 from being a value (you could do -1 on the input but doing it here is easier to keep track off and cleaner)
	int cardNumber = (rand() % 9) + 1;
	int selectedColor = (rand() % 2) + 1;
	int selectedPattern = (rand() % 4) + 1;

	cout << "Try to guess the card in five tries!" << endl;
	cout << "Enter -1 to stop the game." << endl;

	// Debug only
	 cout << "cardNumber: " << cardNumber << ", cardColor: " << selectedColor << ", cardType: " << selectedPattern << endl;

	// Plater inputted variables
	int colorSelect;
	int patternSelect;
	int numberSelect;
	
	// State control of the current guess
	bool guessedColor = false;
	bool guessedPattern = false;
	bool guessedNumber = false;

	while (onGoing)
	{
		// Ask the user which color the card is.
		cout << "What color is the card?" << endl;
		cout << " 1) red" << endl;
		cout << " 2) black" << endl;
		cin >> colorSelect;

		// Back out of the game if the player cancels
		if (colorSelect == -1)
		{
			onGoing = false;
			break;
		}

		// Change the state based on the answer of the player
		if (colorSelect == selectedColor)
		{
			guessedColor = true;
		}
		else
		{
			guessedColor = false;
		}

		cout << "What pattern is on card?" << endl;
		cout << " 1) clover" << endl;
		cout << " 2) spades" << endl;
		cout << " 3) heart" << endl;
		cout << " 4) triangle" << endl;
		cin >> patternSelect;

		// Back out of the game if the player cancels
		if (patternSelect == -1)
		{
			onGoing = false;
			break;
		}

		// Change the state based on the answer of the player
		if (patternSelect == selectedPattern)
		{
			guessedPattern = true;
		}
		else
		{
			guessedPattern = false;
		}

		cout << "which number is on card? (1-9)" << endl;
		cin >> numberSelect;

		// Back out of the game if the player cancels
		if (numberSelect == -1)
		{
			onGoing = false;
			break;
		}

		// Change the state based on the answer of the player
		if (numberSelect == cardNumber)
		{
			guessedNumber = true;
		}
		else
		{
			guessedNumber = false;
		}

		if (guessedColor && guessedPattern && guessedNumber)
		{
			cout << "You have won the game in " << tries << " tries! Please enter your name" << endl;
			
			string name;
			cin >> name;
			string toWrite = name + " has guessed the answer in " + to_string(tries) + " tries";
			
			appendLineToFile("Scores.txt", toWrite);
			break;
		}
		
		// Give feedback to the player on how close they were towards towards guessing the right details.
		cout << "You have " << guessedColor + guessedPattern + guessedNumber << " out of 3" << endl;

		// Decrease amount of tries
		tries--;

		// Check if the player has run out of tries
		if (tries <= 0)
		{
			cout << "you have lost!" << endl;
			onGoing = false;
		}
	}
}

void runScores() {
	cout << "---------" << endl;
	// Create a text string, which is used to output the text file
	string myText;
	// Read from the text file
	ifstream MyReadFile("Scores.txt");
	// Use a while loop together with the getline() function to read the file line by line
	while (getline(MyReadFile, myText)) {
		// Output the text from the file
		cout << myText << endl;
	}
	// Close the file
	MyReadFile.close();
	cout << "---------" << endl;
}

int main()
{
	bool playing = true;
	int playerSelect;

	while (playing)
	{
		// Menu text
		cout << "Welcome to guess the card!" << endl;
		cout << "Please pick one of the options listed below:" << endl;
		cout << " 1) new game" << endl;
		cout << " 2) highscores" << endl;
		cout << " 0) quit game" << endl;

		// Always ask for the player to select a new option or the old input is used.
		cin >> playerSelect;

		// Switch for switching between game and highscore logic.
		switch (playerSelect)
		{
		case 1:
			runGame();
			break;
		case 2:
			runScores();
			break;
		case 0:
			playing = false;
			break;
		default:
			cout << "Invalid input, please type 1, 2, 3 or 0" << endl;
			break;
		}
	}

	return 0;
}