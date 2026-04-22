#pragma once
#include <string>
#include <Windows.h>
#include <iostream>
#include <vector>
#include "Data.h"

#define AlphabetAX 1
#define AlphabetAY 17
#define InputOriginX 15
#define InputOriginY 14

class UI
{
public :
	void InitUI(Data &data);
	void DrawWins(std::string goalWord, int life);
	void DrawLose(std::string goalWord, int life);
	void Render(int life,std::string goalWord , const char inputChar, std::string message, int inputarr[]);
	static void GoToXY(int x, int y);

private :
	void ClearConsole();
	void DrawRule();
	void DrawLife(int life);
	void DrawInputArea(std::string goalWord, const char inputChar);
	void DrawLogs(int inputar[]);

	COORD GetCurrentCursorPos();

private :
	std::vector<COORD> m_goalWordPos;
};