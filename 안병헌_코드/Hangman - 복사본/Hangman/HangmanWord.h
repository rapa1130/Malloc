#pragma once
#include<iostream>
#include <random>
#include"WordData.h"
#include<vector>
#include<string.h>

class HangmanWord
{
public:
	void PickRandomWord()
	{
		if (g_nowWordCount <= 0) {
			throw "There is no word left";
		}
		int randomIndex = GetRandomWordIndex();
		nowWord = g_words[randomIndex];
		std::swap(g_words[randomIndex], g_words[g_nowWordCount-1]);
		g_nowWordCount--;
		wordLength = strlen(nowWord.word);
	}

	void ResetWordContainer()
	{
		g_nowWordCount = WORD_TOTAL_COUNT;
	}
	
	std::vector<int>  Judge(char ch) const
	{
		std::vector<int> matchIndexs;
		const char* str = nowWord.word;
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (str[i] == ch)
			{
				matchIndexs.push_back(i);
			}
		}
		return matchIndexs;
	}
	char GetLetterAt(int index) const
	{
		return nowWord.word[index];
	}

	const char* GetCategory() const
	{
		return nowWord.category;
	}
	const char* GetWord() const
	{
		return nowWord.word;
	}
	int GetWordLength() const
	{
		return wordLength;
	}
private:

	int GetRandomWordIndex()
	{
		std::random_device rd;  
		unsigned long long seed = rd();
		std::mt19937 gen(seed);
		std::uniform_int_distribution<int> dist(0,g_nowWordCount-1);
		return dist(gen);
	}
	WordInfo nowWord;

	int wordLength;
};

