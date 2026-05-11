#pragma once
#include <string>

class Game
{

public:
	Game();
	~Game();

	void ResetGame();
	void clearScreen();
	bool isValid(std::string& str, int& type, bool isUsed[]);
	void printWarning(int type);
	void printAnswer(bool arr[], std::string answer);
	void printUsedAlpha(bool arr[]);
	void checkAnswer(char input, bool arr[], std::string answer, int& life);
	bool CompleteAnswer(std::string answer, bool arr[]);
	bool CheckLife(int life);
	void PrintUI();
	void CheckGameState();
	void Play();

private:
	std::string answer;
	bool answerAlpha[26] = { false };
	bool usedAlpha[26] = { false };
	int life = 10;
	bool isRuning = true;
	int warningType = 0;
	bool isWin = false;

public:
	std::string awnser;
};

