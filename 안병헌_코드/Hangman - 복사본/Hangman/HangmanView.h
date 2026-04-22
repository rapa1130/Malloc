#pragma once
#include"HangmanModel.h"
#include<Windows.h>
#include <iomanip>
#include"HangmanGraphicData.h"


enum COLOR{
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VOILET,
	DAKR_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};


class HangmanView
{
public:
	HangmanView()
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	
	void RenderMain(const HangmanModel& model)
	{
		ResetScreen();
		RenderWrongLetters(2, 22, model);
		RenderMatchedLetters(2, 8, model);
		RenderCategory(2, 2, model);
		RenderResultMessage(2, 11, model);

		RenderRound(47, 26, model);
		RenderHangman(40, 2, model);
	}
	void RenderWin(const HangmanModel& model)
	{
		ResetScreen();
		RenderHangmanLogo(33, 0);
		RenderWinMessage(2, 18);

	}
	void RenderLose(const HangmanModel& model)
	{
		ResetScreen();
		RenderHangmanArt(2, 2);
		RenderLoseMessage(38, 9);
	}
	
private:
	void RenderWinMessage(int x, int y)
	{
		gotoxy(x, y);
		SetConsoleTextAttribute(hConsole, COLOR::DARK_GREEN);
		std::cout << "H A N G  M A N.";

		gotoxy(x, ++y);
		SetConsoleTextAttribute(hConsole, COLOR::WHITE);
		std::cout << "You win";

		gotoxy(x, y += 7);

		SetConsoleTextAttribute(hConsole, COLOR::DARK_GRAY);
		std::cout << "if you want to replay";
		gotoxy(x, ++y);
		std::cout << "press 'y'";
	}
	void RenderLoseMessage(int x, int y)
	{
		gotoxy(x, y);
		SetConsoleTextAttribute(hConsole, COLOR::DARK_RED);
		std::cout << "H A N G  M A N.";

		gotoxy(x, y+=7);
		SetConsoleTextAttribute(hConsole, COLOR::GRAY);
		std::cout << "if you want to replay";
		gotoxy(x, ++y);
		std::cout << "press 'y'";
	}

	void RenderHangmanLogo(int x, int y)
	{
		SetConsoleTextAttribute(hConsole, COLOR::DARK_GRAY);
		for (int i = 0; i < HANGMAN_ART2.size(); i++)
		{
			gotoxy(x, y++);
			std::cout << HANGMAN_ART2[i];
		}
	}

	void RenderHangmanArt(int x, int y)
	{
		SetConsoleTextAttribute(hConsole, COLOR::DARK_GRAY);
		for (int i = 0; i < HANGMAN_ART.size(); i++)
		{
			gotoxy(x, y++);
			std::cout << HANGMAN_ART[i];
		}
	}
	void RenderWrongLetters(int x, int y, const HangmanModel& model)
	{
		gotoxy(x, y);
		SetConsoleTextAttribute(hConsole, COLOR::DARK_GRAY);
		std::cout << "Wrong Alphabets";
		
		gotoxy(x, y+=2);
		const bool* hasWrongEntered = model.GetHasWrongEnteredArr();

		for (int i = 0; i < ALPHABET_NUM; i++)
		{
			if (i == 6) gotoxy(x, ++y);
			else if (i == 14) gotoxy(x, ++y );
			else if (i == 21) gotoxy(x, ++y );

			if (hasWrongEntered[i])
			{
				SetConsoleTextAttribute(hConsole, COLOR::WHITE);
				std::cout <<  (char)('a' + i) <<" ";
			}
			else
			{
				SetConsoleTextAttribute(hConsole, COLOR::DARK_GRAY);
				std::cout <<  (char)('a' + i)<<" ";
			}
		}
	}
	
	void RenderMatchedLetters(int x, int y, const HangmanModel& model)
	{
		HangmanWord hangmanWord = model.GetWord();
		std::vector<bool> matchLetters = model.GetMatchLetters();

		SetConsoleTextAttribute(hConsole, COLOR::DARK_GRAY);
		gotoxy(x, y);
		std::cout << "Matched Alphabets";

		SetConsoleTextAttribute(hConsole, COLOR::WHITE);
		gotoxy(x, y+1);
		for (int i = 0; i < hangmanWord.GetWordLength(); i++)
		{
			std::cout << (matchLetters[i] ? hangmanWord.GetWord()[i] : '_');
			std::cout << ' ';
		}
	}

	void RenderCategory(int x, int y, const HangmanModel& model)
	{
		HangmanWord hangmanWord = model.GetWord();
		const char* category = hangmanWord.GetCategory();
		gotoxy(x, y);
		SetConsoleTextAttribute(hConsole, COLOR::DARK_GRAY);
		std::cout << "Category";

		SetConsoleTextAttribute(hConsole, COLOR::WHITE);
		gotoxy(x, y+1);
		std::cout << category;
	}

	void RenderHangman(int x, int y, const HangmanModel& model)
	{
		using namespace std;
		SetConsoleTextAttribute(hConsole, COLOR::WHITE | FOREGROUND_INTENSITY);

		vector<string> nowLevelHangman2DVec = HANGMAN_ASCII[model.GetNowHaningDegree()];
		for (int j = 0; j < nowLevelHangman2DVec.size(); j++)
		{
			gotoxy(x, y++);
			cout << nowLevelHangman2DVec[j];
		}
	}

	void RenderRound(int x, int y, const HangmanModel& model)
	{
		gotoxy(x, y);
		SetConsoleTextAttribute(hConsole, COLOR::DARK_GRAY);
		std::cout << std::setw(5) << std::right<< "Round";

		
		gotoxy(x, ++y);
		SetConsoleTextAttribute(hConsole, COLOR::WHITE);
		std::cout << std::setw(5) << std::right << model.GetNowRound();
	}

	void RenderResultMessage(int x, int y, const HangmanModel& model)
	{
		MatchingResult result = model.GetMatchingResult();
		gotoxy(x, y);

		switch (result)
		{
		case MatchingResult::DEFAULT:
			break;
		case MatchingResult::DUPLICATE_ALPHABET:
			SetConsoleTextAttribute(hConsole, COLOR::DARK_RED);
			std::cout << "You already enter that.";
			break;
		case MatchingResult::MATCH_ALPHABET:
			break;
		case MatchingResult::NOT_ALPHABET:
			SetConsoleTextAttribute(hConsole, COLOR::DARK_RED);
			std::cout << "You have to enter 'Alphabet'.";
			break;
		case MatchingResult::WRONG_ALPHABET:
			break;
		default:
			break;
		}
	}

	void ResetScreen()
	{
		system("cls");
	}

	void gotoxy(int x, int y) {
		COORD pos = { (SHORT)x, (SHORT)y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		CONSOLE_CURSOR_INFO cursorInfo = { 0, };
		cursorInfo.dwSize = 1; 
		cursorInfo.bVisible = FALSE; 
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	}

	HANDLE hConsole;
};

