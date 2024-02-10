#include <iostream>
#include <fstream>
#include <utility>
#include <list>
#include <string> // Added for to_string
// https://github.com/nlohmann/json version in project: 3.11.2
#include <json.hpp>
#include <vector>
using json = nlohmann::json;

struct Answer {
	std::string answer;
	bool isAnswer;
};

struct Question {
	std::string question;
	std::vector<Answer> answers;
};

std::vector<Question> questions;

/* 
* JSON structure for questions:
* 
{
   "questions":[
      {
         "question":"Hello World",
         "answers":[
            {
               "answer":"7",
               "isAnswer":true
            },
            {
               "answer":"9",
               "isAnswer":false
            }
         ]
      }
   ]
}

possible good source of doc? https://developer.lsst.io/cpp/api-docs.html
*/
void readJsonFile() {
    std::ifstream f("quiz.json");
	if (!f)
	{
		throw std::exception("Could not open file, does the file exist?");
	} else {
		json data = json::parse(f);

		for (json var : data["questions"])
		{
			if (!var["question"].is_string())
			{
				throw std::exception("Wrong input format, check answer of question: " + var["question"]);
			}

			std::vector<Answer> answers;

			for (json answer : var["answers"]) {
				if (!answer["isAnswer"].is_boolean() || !answer["answer"].is_string())
				{
					throw std::exception("Wrong input format, check answer of question: " + var["question"]);
				}
				answers.push_back({ answer["answer"], answer["isAnswer"] });
			}

			questions.push_back({var["question"], answers});
		}
	}
}

void runQuiz() {
	int counter = 1;
	int answerCounter;
	int playerInput;
	bool notAnswered;
	int goodAnswers = 0;

	std::cout << "The quiz starts now!" << std::endl;

	for (Question question : questions)
	{
		notAnswered = true;
		std::cout << "question " << counter << ": " << question.question << std::endl << std::endl;

		while (notAnswered)
		{


			answerCounter = 0;
			for (Answer answer : question.answers) {
				std::cout << answerCounter << "> " << answer.answer << std::endl;
				answerCounter++;
			}
			
			std::cin >> playerInput;

			while (!std::cin.good())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Please only enter numbers!" << std::endl;
				std::cin >> playerInput;
			}
			

			if (playerInput > (question.answers.size() - 1))
			{
				std::cout << "That isn't an answer, please try again" << std::endl;
			}
			else if (question.answers[playerInput].isAnswer)
			{
				std::cout << "You got the right answer, congrats!" << std::endl;
				notAnswered = false;
				goodAnswers++;
			}
			else
			{
				std::cout << "You got the wrong answer, better luck next time!" << std::endl;
				notAnswered = false;
			}
			

		}
		
		counter++;
	}

	std::cout << "\nYou finihed the quiz with " << goodAnswers << " out of " << questions.size() << " correct answers!\n\n" << std::endl;
}

int main()
{
	std::cout << "Welcome to Quiz World! \nChecking file..." << std::endl;
	try
	{
		readJsonFile();
	}
	catch (const std::exception& e)
	{
		printf(e.what());
	}

	std::cout << "Checking files done, moving to menu!" << std::endl;
	bool ongoing = true;
	int menuSelect;

	while (ongoing)
	{
		std::cout << "Welcome to Quiz World's main menu" << std::endl;
		std::cout << "Please pick one of the options listed below:" << std::endl;
		std::cout << " 1) start quiz" << std::endl;
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
			runQuiz();
		}
		else if (menuSelect == 0) {
			ongoing = false;
		}
	}
}