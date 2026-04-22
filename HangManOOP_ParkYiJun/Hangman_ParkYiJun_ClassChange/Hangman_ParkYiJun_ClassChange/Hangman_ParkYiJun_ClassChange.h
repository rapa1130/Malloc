#pragma once

constexpr int MaxTries = 6; // 최대로 시도 가능한 횟수
constexpr int MaxWordLength = 100; // 단어의 최대 길이
constexpr int MaxTriedLetters = 26; // 알파벳의 갯수


class HangmanGame
{
public:
    HangmanGame(const char* InWord);
    void Play();               // 전체 게임 실행

private:
    bool IsGameWon() const;     // 게임 승리 조건
    bool IsGameOver() const;   // 게임 종료 조건
    void PrintUseWord() const;  // 사용한 알파벳 출력
    void PrintStatus() const;     // 현재 상태 출력
    void PrintHangman() const;    // 행맨 그림 출력
    void PrintWordState() const;  // 단어 상태 출력
    bool IsAlreadyTried(char Letter) const; // 이미 시도한 알파벳인 경우
    void HandleInput();           // 사용자 입력 및 처리

private:
    int WordLength; // 단어의 길이
    int TriedCount; // 시도한 횟수
    int WrongGuessCount; // 틀린 횟수

    bool bGuessed[MaxWordLength]; // 
    char TriedLetters[MaxTriedLetters]; // 시도한 영단어 출력을 위한 변수
    char Word[MaxWordLength]; // 단어
    bool wrongAlphabet; // 이미 나온 알파벳이거나 잘못 입력한 경우를 위한 변수
};