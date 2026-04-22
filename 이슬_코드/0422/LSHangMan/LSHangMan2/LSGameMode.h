#pragma once
#include "CommonHeader.h"

using namespace std;

class LSGameMode
{
public:
	LSGameMode();
	void StartGame();

protected:
	void GameRun();
	char GetAvailableInput();
	void PrintInputAnswerLength();
	void PrintGameOver();
	void PrintGameClear();
	void PrintMainFlow();

protected:
	class LSAnswer* Answer;
	class LSSave* Save;
	int ClearCount = 0;
	vector<char> PastInputs;
};

