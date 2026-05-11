#include "Game.h"
#include "RandomGenerator.h"
#include "iostream"

using namespace std;

Game::Game()
{
    
}

Game::~Game()
{
}

void Game::ResetGame()
{
    //게임 초기화
    answer = "";

    for (int i = 0; i < 26; i++)
    {
        answerAlpha[i] = false;
        usedAlpha[i] = false;
    }

    life = 10;
    isRuning = true;
    warningType = 0;

    isWin = false;

    //랜덤 생성
    RandomGenerator rg;
    rg.chooseRandomWord(awnser);
}

//화면 지우기
void Game::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


bool Game::isValid(string& str, int& type, bool isUsed[])
{
    //길이 검사
    if (str.length() > 1 || str.length() == 0)
    {
        //경고 메시지 타입
        type = 1;
        return false;
    }
    bool isAlpha = (('a' <= str[0] && str[0] <= 'z') || ('A' <= str[0] && str[0] <= 'Z'));

    //알파벳인지
    if (!isAlpha)
    {
        //경고 메시지 타입
        type = 2;
        return false;
    }

    //이미 사용된 알파벳인지
    int index = (str[0] -= 32) - 'A';
    if (isUsed[index])
    {
        //경고 메시지 타입
        type = 3;
        return false;
    }

    //전부 통과했으면 대문자로 변환
    if (('a' <= str[0] && str[0] <= 'z'))
    {
        str[0] -= 32;
    }

    type = 0;
    cout << "\n";
    return true;
}

void Game::printWarning(int type)
{
    switch (type)
    {
    case 0:
        cout << "\n";
        break;
    case 1:
        cout << "알파벳을 하나만 입력하세요\n";
        break;
    case 2:
        cout << "알파벳만 입력 가능합니다!\n";
        break;
    case 3:
        cout << "이미 입력한 알파벳입니다!\n";
        break;
    default:
        break;
    }
}

//정답 출력
void Game::printAnswer(bool arr[], string answer)
{
    for (int i = 0; i < answer.length(); i++)
    {
        int index = answer[i] - 'A';  // 문자 → 인덱스 변환

        //이 글자를 맞춘 적이 있는지
        if (arr[index])
        {
            cout << answer[i] << " ";
        }
        else
        {
            cout << "_ " << " ";
        }
    }
    cout << "\n";
}

//플레이어가 이미 사용한 알파벳 출력
void Game::printUsedAlpha(bool arr[])
{
    for (int i = 0; i < 26; i++)
    {
        if (arr[i])
        {
            char c = i + 'A';
            cout << c << " ";
        }
    }
}

void Game::CheckGameState()
{
    if (life <= 0)
    {
        clearScreen();
        cout << "G A M E O V E R\n";
        cout << "\n";
        cout << "정답 : " << awnser;
        cout << "\n";
        cout << "\n";
        cout << "계속하려면 아무거나 입력하세요";
        cout << "\n";
        cout << "\n";
        string tresh;
        cin >> tresh;
        clearScreen();
        ResetGame();
    }
    if (isWin)
    {
        clearScreen();
        cout << "\n";
        cout << "\n";
        cout << "정답을 맞추셨습니다!";
        cout << "\n";
        cout << "계속하려면 아무거나 입력하세요";
        cout << "\n";
        cout << "\n";
        string tresh;
        cin >> tresh;
        clearScreen();
        ResetGame();
    }

}

bool Game::CompleteAnswer(string answer, bool arr[])
{
    //true개수 = Awnser의 길이면 정답 맞춘거임
    //answer 범위기반 for문 돌려서 똑같은 알파벳이 중복되어도 상관없음
    int numberofRights = 0;
    for (char c : answer)
    {
        int index = c - 'A';
        if (arr[index] == true)
        {
            numberofRights++;
        }
    }
    if (numberofRights == answer.length()) return true;
    return false;
}

//정답 확인
void Game::checkAnswer(char input, bool arr[], string answer, int& life)
{
    bool isAnswer = false;
    //입력한 알파벳이 존재하는지 확인
    for (char c : answer)
    {
        if (c == input)
        {
            //있으면 정답 bool배열에 true표시
            isAnswer = true;
            int index = input - 'A';
            arr[index] = true;
        }
    }
    if (!isAnswer) life--;
    if(CompleteAnswer(answer, arr)) isWin = true;
    CheckGameState();
}



bool Game::CheckLife(int life)
{
    //사망 확인
    if (life <= 0)
    {
        return true;
    }
    return false;
}

void Game::PrintUI()
{
    clearScreen();
    //cout << awnser << "\n"; 디버그용
    cout << "<< HangMan >>\n";
    cout << " ------------------------\n";
    cout << "[규칙]\n";
    cout << "숨겨진 단어를 목숨이 0이되기 전까지 완성시켜야 합니다\n";
    cout << "매 턴마다 단 하나의 알파벳만 입력해야 합니다.\n";
    cout << "숨겨진 단어를 이루는 알파벡일 경우 위치가 표시됩니다.\n";
    cout << "-------------------------\n";
    cout << "남은 목숨:" << " " << life << "\n";
    cout << "-------------------------\n";
    cout << "사용한 알파벳";
    printUsedAlpha(usedAlpha);
    cout << "\n";
    cout << "\n";
    printAnswer(answerAlpha, awnser);
    cout << "\n";
    printWarning(warningType);
}



void Game::Play()
{
    ResetGame();
    while (isRuning)
    {

        PrintUI();
        string input;
        cout << "알파벳을 입력하세요 : " << flush;
        cin >> input;
        if (!isValid(input, warningType, usedAlpha))
        {
            continue;
        }
        else
        {
            char alphabat = input[0];
            int index = alphabat - 'A';
            usedAlpha[index] = true;

            //정답 확인 로직
            checkAnswer(alphabat, answerAlpha, awnser, life);

            ////단어 완성 검사
            //if (CompleteAnswer(awnser, answerAlpha))
            //{
            //    isWin = true;
            //}
        }
        //CheckGameState();
    }
}