#pragma once
#include "CommonHeader.h"

using namespace std;

class LSAnswer
{
public:
	void SetAnswerLength();
	void SetAnswer();
	int GetAnswerLength() { return AnswerLength; }
	string GetAnswer() { return Answer; }
	void PrintNowAnswer();
	bool CheckAnswer(const char Input, int& ClearCount);

protected:
	int AnswerLength = -1;
	string Answer;
	vector<char> NowAnswer;
};

