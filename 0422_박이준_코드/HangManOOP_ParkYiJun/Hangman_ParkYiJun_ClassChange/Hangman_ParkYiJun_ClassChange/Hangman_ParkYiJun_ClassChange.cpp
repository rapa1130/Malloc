#include "Hangman_ParkYiJun_ClassChange.h"
#include <cstring>
#include <iostream>
#include <windows.h>

// 필요한 헤더 불러오기

HangmanGame::HangmanGame(const char* InWord) { // 행맨 게임 클래스 제작
    strcpy_s(Word, InWord); // 단어 복사
    WordLength = strlen(InWord); // 단어 길이

    TriedCount = 0; // 시도 횟수
    WrongGuessCount = 0; // 틀린 횟수 
    wrongAlphabet = false; // 틀린 알파벳이 있을 경우 

    for (int i = 0; i < MaxWordLength; i++) {
        bGuessed[i] = false; // 추측해야 하는 단어를 모두 false로
    }

    for (int i = 0; i < MaxTriedLetters; i++) {
        TriedLetters[i] = '\0'; // 시도해야하는 영단어를 모두 Null로 설정
    }
}

void HangmanGame::Play() {
    while (!IsGameOver()) {
        PrintStatus(); // 행맨, 사용단어, 단어 상태 출력
        HandleInput(); // 알파벳 입력을 위한 핸들
    }
    
    if (IsGameWon()) {
        std::cout << "You Win!! \n\n"; // 승리 시
    }
    else { // 패배 시, 단어를 알려주도록 설계
        std::cout << "You Lose..\n\n";
        std::cout << "Word : " << Word << "\n\n";
    }

}

bool HangmanGame::IsGameWon() const {
    for(int i=0;i<WordLength;i++) 
    {
        if(bGuessed[i] == false) // 추측해야 하는 단어에 없는 경우
            return false;
    } 
    return true; // 알파벳이 있는 경우
}// 게임 승리 조건

bool HangmanGame::IsGameOver() const {
    return (WrongGuessCount >= MaxTries) || IsGameWon(); // 게임이 끝날 경우 (틀린횟수 >= 최대횟수 || 게임이 이겼을 경우)
}// 게임 종료 조건

void HangmanGame::PrintStatus() const {
    PrintHangman(); // 행맨 출력
    PrintUseWord(); // 사용 단어 출력
    PrintWordState(); // 단어 상태 출력(입력)
}// 현재 상태 출력

void HangmanGame::PrintHangman() const {
    std::cout << "|\t HangMan :" << (MaxTries - WrongGuessCount) << "번의 기회가 주어집니다\n";
    std::cout << "|\t 알파벳 한 개만 입력하여 주십시오\n";
    std::cout << "|\t 여러 알파벳을 입력하면 제일 앞의 알파벳만 입력됩니다\n";
    std::cout << "|-------------------------------------------------------------|\n\n";


}// 행맨 그림 출력

void HangmanGame::PrintWordState() const {
    for (int i = 0; i < WordLength; i++) { // 입력한 단어 출력
        if (bGuessed[i]) {
            std::cout << Word[i] << " "; // 맞힌 영단어 있을 시, Word[i]출력 후 띄어쓰기
        }
        else { // 맞히지 못한 단어 출력
            std::cout << "_ ";
        }
    }
    std::cout << std::endl;
}// 단어 상태 출력

void HangmanGame::PrintUseWord() const {
    std::cout << "|-----------시도했던 알파벳들: \n[ "; // 사용한 영단어 출력
    for (int i = 0; i < TriedCount; i++) {
        std::cout << TriedLetters[i] << " ";
        if (i != 0 && i % 4 == 0) { // 0~4까지 출력 후 (5번) 이 후 줄 내림
            std::cout << std::endl << "  ";
        }
    }
    std::cout << "]\n";
    std::cout << "|-----------------------------|\n\n";
}
bool HangmanGame::IsAlreadyTried(char Letter) const {
    for (int i = 0; i <TriedCount; i++) {
        if (TriedLetters[i] == Letter) { // 입력한 단어가 이미 실행된 경우
            return true;
        }
    } 
    return false;
}
void HangmanGame::HandleInput() {
    std::string inputStr;
    if (wrongAlphabet == true) { // 전에 cls로 지운 경우 (잘못된 입력) 출력
        std::cout << "입력하지 않은 소문자 알파벳 하나로만 다시 입력해주세요\n";
    }

    std::cout << "\t 정답 : ";
    std::cin >> inputStr; // 입력받기
    if (inputStr.length() != 1) { // 한글자만 입력
        wrongAlphabet = true; // 아니면 입력 오류
        system("cls");
        return;
    }
    char input = inputStr[0];

    wrongAlphabet = false; 
    if (IsAlreadyTried(input)) {  // 이미 시도한 경우인지 체크
        std::cout << "Already tried\n";
        wrongAlphabet = true;
        system("cls");
        return;
    }
    TriedLetters[TriedCount++] = input;

    bool found = false;
    for (int i = 0; i < WordLength; i++) { // 0부터 단어 길이만큼
        if (Word[i] == input) { // Word[i]번째와 알파벳이 같은 경우
            bGuessed[i] = true;  // 추측한 단어 true로 교환
            found = true; 
        }
    }

    if (!found) {
        WrongGuessCount++;
    }

    system("cls");
}// 사용자 입력 및 처리