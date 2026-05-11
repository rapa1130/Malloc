#pragma once
#include<vector>
#include"HangmanWord.h"
#include<algorithm>
#include<set>
#define ALPHABET_NUM 26


enum MatchingResult
{
	DUPLICATE_ALPHABET,
	NOT_ALPHABET,
	WRONG_ALPHABET,
	MATCH_ALPHABET,
	DEFAULT
};

enum GameState
{
	Win,
	Lose,
	Main,
};

class HangmanModel
{
public:
	HangmanModel()
		:round(0),MaxHangingDegree(8),nowHangingDegree(0)
	{
		GotoNextRound();
	}

	void Reset()
	{
		round = 1;
		nowHangingDegree = 0;
		word.ResetWordContainer();
		word.PickRandomWord();
		nowState = GameState::Main;
		nowMatchingResult = MatchingResult::DEFAULT;
		matchLetters.clear();
		matchLetters.resize(word.GetWordLength(), false);
		std::fill_n(hasWrongEnteredArr, ALPHABET_NUM, false);
	}

	void GotoNextRound()
	{
		if (round == 3)
		{
			nowState = GameState::Win;
		}
		else
		{
			nowState = GameState::Main;
			nowMatchingResult = MatchingResult::DEFAULT;
			word.PickRandomWord();
			nowHangingDegree = 0;
			matchLetters.clear();
			matchLetters.resize(word.GetWordLength(), false);
			round++;
			std::fill_n(hasWrongEnteredArr, ALPHABET_NUM, false);
		}
	}

	void AddHangingDegree()
	{
		nowHangingDegree++;
	}
	
	bool AreAllLettersMatch() const
	{
		for (auto isLetterMatch : matchLetters) if (!isLetterMatch) return false;
		return true;
	}


	void ProcessInput(char ch)
	{
		if (!isalpha(ch))
		{
			nowMatchingResult = MatchingResult::NOT_ALPHABET;
			return;
		}
		if (WasAlreadyEnterd(ch))
		{
			nowMatchingResult = MatchingResult::DUPLICATE_ALPHABET;
			return;
		}

		std::vector<int> matchList = word.Judge(ch);
		if (matchList.empty())
		{
			hasWrongEnteredArr[tolower(ch) - 'a'] = true;
			AddHangingDegree();

			if (nowHangingDegree == MaxHangingDegree)
			{
				nowState = GameState::Lose;
			}
			else
			{
				nowMatchingResult = MatchingResult::WRONG_ALPHABET;
			}
		}
		else
		{
			nowMatchingResult = MatchingResult::MATCH_ALPHABET;
			for (int i = 0; i < matchLetters.size(); i++)
			{
				if (word.GetLetterAt(i) == ch) matchLetters[i] = true;
			}
			
			if (AreAllLettersMatch())
			{
				GotoNextRound();
			}
		}
	}

	std::vector<bool> GetMatchLetters() const
	{
		return matchLetters;
	}
	const bool* GetHasWrongEnteredArr() const
	{
		return hasWrongEnteredArr;
	}

	MatchingResult GetMatchingResult() const
	{
		return nowMatchingResult;
	}
	GameState GetGameState() const
	{
		return nowState;
	}
	HangmanWord GetWord() const
	{
		return word;
	}
	int GetNowRound() const
	{
		return round;
	}
	
	int GetNowHaningDegree() const
	{
		return nowHangingDegree;
	}
	
	int GetMaxHangingDegree() const
	{
		return MaxHangingDegree;
	}
private:

	bool WasAlreadyEnterd(char ch) const
	{
		if (hasWrongEnteredArr[tolower(ch) - 'a']) return true;
		for (int i = 0; i < matchLetters.size(); i++)
		{
			if (matchLetters[i])
			{
				char nowLetter = word.GetLetterAt(i);
				if (nowLetter == ch) return true;
			}
		}
		return false;
	}

	int nowHangingDegree;
	int MaxHangingDegree;
	int round;
	
	HangmanWord word;
	std::vector<bool> matchLetters;
	bool hasWrongEnteredArr[ALPHABET_NUM];
	
	MatchingResult nowMatchingResult;
	GameState nowState;
};