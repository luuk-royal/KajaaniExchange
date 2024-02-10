#include "DiceRollClass.h"

void DiceRollClass::DiceRoll(std::vector<int>& dice)
{
	for (size_t i = 0; i < 5; i++)
	{
		dice.push_back((rand() % 6) + 1);
	}
}
