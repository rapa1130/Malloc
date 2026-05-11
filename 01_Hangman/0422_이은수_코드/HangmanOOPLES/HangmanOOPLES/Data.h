#pragma once
#include <random>

#define MIN 0
#define MAX 1000

class Data
{
public :
	Data();

	int GetLife() const;
	std::string GetGoalWord() const;
	bool CheckAnswer(const char c);
private:
	void SetSeed();
	std::string CreateRandomWord() const;
	static int GetRandomInt(std::mt19937 seed);

private:
	int m_life;
	int m_curCount;
	std::string m_goalWord;
	std::mt19937 m_seed;
	static const std::vector<std::string> g_middleSchoolWords;
};