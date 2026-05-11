#include "LSAnswer.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <random>
#include <algorithm>

using namespace std;

void LSAnswer::SetAnswerLength()
{
    while (AnswerLength < 3 || AnswerLength > 5)
    {
        system("cls");
        cout << "--------------------------------------------" << endl << endl;
        cout << "        글자 수를 입력하세요. (3~5)" << endl << endl;
        cout << "--------------------------------------------" << endl << endl;
        cout << "Input : ";

        if (!(cin >> AnswerLength))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            AnswerLength = -1;
        }
    }
}

void LSAnswer::SetAnswer()
{
    if (AnswerLength < 3 || AnswerLength > 5)
        return;

    ///////////////////////////////파일로 단어 불러오기 (여기는 AI썼음!)
    string fileName = "words" + to_string(AnswerLength) + ".txt";
    std::ifstream file(fileName);
    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    std::vector<std::string> words;
    std::stringstream ss(content);
    std::string word;

    while (std::getline(ss, word, ',')) {
        // 앞뒤 공백 제거
        word.erase(0, word.find_first_not_of(" \n\r"));
        word.erase(word.find_last_not_of(" \n\r") + 1);
        words.push_back(word);
    }

    // 확인 출력
    //for (const auto& w : words) {
    //    std::cout << w << "\n";
    //}
    ///////////////////////////////////////////////////////////////

    /////////////////////////////////정답 단어 고르기
    // 난수 생성기 초기화 (seed 값으로 random_device 사용)
    std::random_device rd;  // 하드웨어 엔트로피를 이용한 시드

    unsigned long long seed = rd();

    std::mt19937 gen(seed); // Mersenne Twister 19937 엔진

    // 특정 범위의 정수 난수 생성 (0 ~ 99)
    std::uniform_int_distribution<int> dist(0, words.size() - 1);

    //const string answer = "fly";
    Answer = words[dist(gen)];

    for (int i = 0; i < AnswerLength; i++)
    {
        NowAnswer.push_back('_');
    }
}

void LSAnswer::PrintNowAnswer()
{
    cout << endl << "                 ";
    for (int i = 0; i < 5 - AnswerLength; i++)
    {
        cout << " ";
    }

    for (char c : NowAnswer)
    {
        cout << c << ' ';
    }
}

bool LSAnswer::CheckAnswer(const char Input, int& ClearCount)
{
    for (int i = 0; i < AnswerLength; i++)
    {
        if (Answer[i] == Input)
        {
            NowAnswer[i] = Input;
            ClearCount++;
            return true;
        }
    }
    return false;
}
