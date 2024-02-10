#include <vector>
#include "Character.h"

#pragma once
class RPGManager
{
public:
	void runMainGameLoop();
	void listTypes();
	bool ongoing;
private:
	// https://stackoverflow.com/questions/5313322/c-cast-to-derived-class
	Character& mainCharacterSelect();
	void battle(Character& heroCharacter);
	//std::vector<Character> partySelect();
};

