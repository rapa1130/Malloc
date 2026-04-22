#pragma once
#include "Data.h"
#include "UI.h"
#include <string>

class HangmanGame
{
public :
	HangmanGame();
	~HangmanGame();

	void GameStart();		// 게임 실행

private:
	void GameLoop();
	void ProcessingInput();
	void ProcessingGameLogic();
	void ProcessingRender();
	bool CheckValidateInput(char c);
	void ResetGame();

private:
	bool m_isGameComplete = false;
	char m_inputChar;
	char m_upper;
	int m_inputAlphabets[26] = { 0, };
	std::string m_message = "";
	Data* m_data;
	UI* m_ui;
};