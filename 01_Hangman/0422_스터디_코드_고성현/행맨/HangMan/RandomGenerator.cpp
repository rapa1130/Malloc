#include "RandomGenerator.h"
#include "string"
#include <random>

using namespace std;


RandomGenerator::RandomGenerator()
{
}

RandomGenerator::~RandomGenerator()
{
}

void RandomGenerator::chooseRandomWord(string& word)
{
    std::string words[10] = { "CAT", "DOG", "SUN", "MOON", "TREE", "BOOK", "CODE", "GAME", "ROCK", "WIND" };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> rand(0, 9);
    word = words[rand(gen)];
}