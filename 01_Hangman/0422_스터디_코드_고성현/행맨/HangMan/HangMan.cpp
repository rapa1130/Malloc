#include <iostream>
#include <string>
#include <random>
#include "Game.h"

using namespace std;

//string words[10] = {
//"CAT",
//"DOG",
//"SUN",
//"MOON",
//"TREE",
//"BOOK",
//"CODE",
//"GAME",
//"ROCK",
//"WIND"
//};
//
//string awnser;
//bool answerAlpha[26] = { false };
//bool usedAlpha[26] = { false };
//int life = 10;
//bool isRuning = true;
//int warningType = 0;
//bool isWin = false;
//
//void chooseRandomWord(string words[], string& awnser)
//{
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<int> rand(0, 9);
//    awnser = words[rand(gen)];
//}
//
//void ResetGame()
//{
//    awnser = "";
//
//    for (int i = 0; i < 26; i++)
//    {
//        answerAlpha[i] = false;
//        usedAlpha[i] = false;
//    }
//
//    life = 10;
//    isRuning = true;
//    warningType = 0;
//
//    isWin = false;
//
//    chooseRandomWord(words, awnser);
//}
//
//
//void clearScreen()
//{
//#ifdef _WIN32
//    system("cls");
//#else
//    system("clear");
//#endif
//}
//
//bool isValid(string& str, int& type, bool isUsed[])
//{
//    if (str.length() > 1 || str.length() == 0)
//    {
//        type = 1;
//        return false;
//    }
//    bool isAlpha = (('a' <= str[0] && str[0] <= 'z') || ('A' <= str[0] && str[0] <= 'Z'));
//    if (!isAlpha)
//    {
//        type = 2;
//        return false;
//    }
//    int index = (str[0] -= 32) - 'A';
//    if (isUsed[index])
//    {
//        type = 3;
//        return false;
//    }
//    if (('a' <= str[0] && str[0] <= 'z'))
//    {
//        str[0] -= 32;
//    }
//
//    type = 0;
//    cout << "\n";
//    return true;
//}
//
//void printWarning(int type)
//{
//    switch (type)
//    {
//    case 0:
//        cout << "\n";
//        break;
//    case 1:
//        cout << "알파벳을 하나만 입력하세요\n";
//        break;
//    case 2:
//        cout << "알파벳만 입력 가능합니다!\n";
//        break;
//    case 3:
//        cout << "이미 입력한 알파벳입니다!\n";
//        break;
//    default:
//        break;
//    }
//}
//
//void printAnswer(bool arr[], string answer)
//{
//    for (int i = 0; i < answer.length(); i++)
//    {
//        int index = answer[i] - 'A';  // 문자 → 인덱스 변환
//
//        if (arr[index])
//        {
//            cout << answer[i] << " ";
//        }
//        else
//        {
//            cout << "_ " << " ";
//        }
//    }
//    cout << "\n";
//}
//
//void printUsedAlpha(bool arr[])
//{
//    for (int i = 0; i < 26; i++)
//    {
//        if (arr[i])
//        {
//            char c = i + 'A';
//            cout << c << " ";
//        }
//    }
//}
//
//void checkAnswer(char input, bool arr[], string answer, int& life)
//{
//    bool isAnswer = false;
//    for (char c : answer)
//    {
//        if (c == input)
//        {
//            isAnswer = true;
//            int index = input - 'A';
//            arr[index] = true;
//        }
//    }
//    if (!isAnswer) life--;
//}
//
//bool CompleteAnswer(string answer, bool arr[])
//{
//    int numberofRights = 0;
//    for (char c : answer)
//    {
//        int index = c - 'A';
//        if (arr[index] == true)
//        {
//            numberofRights++;
//        }
//    }
//    if (numberofRights == answer.length()) return true;
//    return false;
//}
//
//bool CheckLife(int life)
//{
//    if (life <= 0)
//    {
//        return true;
//    }
//    return false;
//}
//
///*
//0. 입력된 배열 중 무작위 단어 선택
//1. 알파벳 입력
//2. 입력한 문자 확인
//2-1. 입력한 길이가 1인가
//2-2. 입력한 문자가 알파벳인가
//2-3. 대, 소문자 확인 후 대문자로 변환
//3. 입력한 문자가 문자열에 있는지 확인
//4. 있으면 정답 문자열 표시
//5. 없으면 목숨 차감 후 재입력
//*/
//
//void PrintUI()
//{
//    clearScreen();
//    //cout << awnser << "\n"; 디버그용
//    cout << "<< HangMan >>\n";
//    cout << " ------------------------\n";
//    cout << "[규칙]\n";
//    cout << "숨겨진 단어를 목숨이 0이되기 전까지 완성시켜야 합니다\n";
//    cout << "매 턴마다 단 하나의 알파벳만 입력해야 합니다.\n";
//    cout << "숨겨진 단어를 이루는 알파벡일 경우 위치가 표시됩니다.\n";
//    cout << "-------------------------\n";
//    cout << "남은 목숨:" << " " << life << "\n";
//    cout << "-------------------------\n";
//    cout << "사용한 알파벳";
//    printUsedAlpha(usedAlpha);
//    cout << "\n";
//    cout << "\n";
//    printAnswer(answerAlpha, awnser);
//    cout << "\n";
//    printWarning(warningType);
//}
//
//void CheckGameState()
//{
//    if (life <= 0)
//    {
//        clearScreen();
//        cout << "G A M E O V E R\n";
//        cout << "\n";
//        cout << "정답 : " << awnser;
//        cout << "\n";
//        cout << "\n";
//        cout << "계속하려면 아무거나 입력하세요";
//        cout << "\n";
//        cout << "\n";
//        string tresh;
//        cin >> tresh;
//        clearScreen();
//        ResetGame();
//    }
//    if (isWin)
//    {
//        clearScreen();
//        cout << "\n";
//        cout << "\n";
//        cout << "정답을 맞추셨습니다!";
//        cout << "\n";
//        cout << "계속하려면 아무거나 입력하세요";
//        cout << "\n";
//        cout << "\n";
//        string tresh;
//        cin >> tresh;
//        clearScreen();
//        ResetGame();
//    }
//}

int main()
{

    //chooseRandomWord(words, awnser);
    //while (isRuning)
    //{

    //    PrintUI();
    //    string input;
    //    cout << "알파벳을 입력하세요 : " << flush;
    //    cin >> input;
    //    if (!isValid(input, warningType, usedAlpha))
    //    {
    //        continue;
    //    }
    //    else
    //    {
    //        char alphabat = input[0];
    //        int index = alphabat - 'A';
    //        usedAlpha[index] = true;

    //        //정답 확인 로직
    //        checkAnswer(alphabat, answerAlpha, awnser, life);

    //        //단어 완성 검사
    //        if (CompleteAnswer(awnser, answerAlpha))
    //        {
    //            isWin = true;
    //        }
    //    } 
    //    CheckGameState();
    //}
    Game gm;
    gm.Play();
    return 0;
}
