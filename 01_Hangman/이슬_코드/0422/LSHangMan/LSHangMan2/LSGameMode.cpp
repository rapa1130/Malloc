#include "LSGameMode.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <random>
#include <algorithm>

#include "LSAnswer.h"
#include "LSSave.h"


using namespace std;

LSGameMode::LSGameMode()
{
    Answer = new LSAnswer();
    Save = new LSSave();
}

void LSGameMode::StartGame()
{
    if (Answer == nullptr)
    {
        system("cls");
        cout << "서늘하다. 무언가 잘못되었따. Answer is null";

        delete Answer;
        delete Save;
        return;
    }

    Answer->SetAnswerLength();

    if (Answer->GetAnswerLength() < 3 || Answer->GetAnswerLength() > 5)
    {
        system("cls");
        cout << "서늘하다. 무언가 잘못되었따. SetAnswerLength Err";

        delete Answer;
        delete Save;
        return;
    }

    Answer->SetAnswer();

    if (Answer->GetAnswer().size() != Answer->GetAnswerLength())
    {
        system("cls");
        cout << "서늘하다. 무언가 잘못되었따. SetAnswer Err";

        delete Answer;
        delete Save;
        return;
    }

    Save->SetMaxSave(Answer->GetAnswerLength());

    GameRun();
}

void LSGameMode::GameRun()
{
    const string AnswerString = Answer->GetAnswer();

    //정답이 나올 때까지 반복
    while (ClearCount != AnswerString.size())
    {
        //system("cls");

        //게임 오버면 아웃
        if (!Save->CheckSave())
        {
            PrintGameOver();

            delete Answer;
            delete Save;
            return;
        }


        //정답지 출력해놓기
        //cout << "정답 : " << answer << ' ';
        //cout << endl;

        char Input = '\0';
        while (Input == '\0')
        {
            //게임 메인 화면 출력
            PrintMainFlow();

            //인풋 받기
            Input = GetAvailableInput();
        }
  
        //정답 비교하기
        if (!Answer->CheckAnswer(Input, ClearCount))
        {
            Save->ApplyDamage();
        }

        if (ClearCount == Answer->GetAnswerLength())
        {
            PrintGameClear();

            delete Answer;
            delete Save;
            return;
        }
    }
}

char LSGameMode::GetAvailableInput()
{
    char Input;
    cin >> Input;
    if (Input < 'a' || Input>'z')
    {
        Input = '\0';
    }

    if (find(PastInputs.begin(), PastInputs.end(), Input) != PastInputs.end())
    {
        Input = '\0';
    }
    else
    {
        PastInputs.push_back(Input);
    }

    return Input;
}

void LSGameMode::PrintInputAnswerLength()
{
    system("cls");
    cout << "--------------------------------------------" << endl << endl;
    cout << "        글자 수를 입력하세요. (3~5)" << endl << endl;
    cout << "--------------------------------------------" << endl << endl;
    cout << "Input : ";
}

void LSGameMode::PrintGameOver()
{
    system("cls");
    cout << "--------------------------------------------" << endl << endl;
    cout << "게임 오버! : " << Answer->GetAnswer() << endl << endl;
    cout << "--------------------------------------------" << endl << endl;
}

void LSGameMode::PrintGameClear()
{
    system("cls");
    cout << "--------------------------------------------" << endl << endl;
    cout << "정답입니다! : " << Answer->GetAnswer() << endl << endl;
    cout << "--------------------------------------------" << endl << endl;
}

void LSGameMode::PrintMainFlow()
{
    system("cls");
    cout << "--------------------------------------------" << endl << endl;

    //현재까지 입력된 알파벳 출력
    cout << "Inputs : ";
    for (int i = 0; i < PastInputs.size(); i++)
    {
        cout << PastInputs[i] << " ";
    }

    //남은 목숨
    cout << endl;
    Save->PrintCurrentSave();

    cout << endl << endl << "      하나의 소문자 알파벳을 입력하세요." << endl;

    //현재까지 맞힌 정답
    Answer->PrintNowAnswer();

    cout << endl << endl << endl;
    cout << "--------------------------------------------" << endl << endl;
    cout << "Input : ";
}
