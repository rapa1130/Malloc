#include "UI.h"

using namespace std;

void UI::InitUI(Data &data)
{
	int life = data.GetLife();
	std::string word = data.GetGoalWord();

	ClearConsole();
	DrawRule();
	DrawLife(life);
	DrawInputArea(word, '\0');
	std::cout << "                                                                                     ";
	GoToXY(InputOriginX, InputOriginY);
}
void UI::DrawWins(std::string goalWord, int life)
{
	ClearConsole();
	cout << endl;
	cout << "축하합니다!!! 정답을 맞추셨습니다!!" << endl;
	cout << "정답 단어 : " << goalWord << endl;
	cout << "남은 목숨 : " << life << endl;
	system("pause");
}
void UI::DrawLose(std::string goalWord, int life)
{
	ClearConsole();
	cout << endl;
	cout << "이런.... 그를 구하지 못했습니다" << endl;
	cout << "정답 단어 : " << goalWord << endl;
	cout << "남은 목숨 : " << life << endl;
	system("pause");
}
void UI::Render(int life, std::string goalWord , const char inputChar, std::string message, int inputarr[])
{
	DrawLife(life);
	DrawInputArea(goalWord, inputChar);
	if (message == "")
	{
		GoToXY(0, 15);
		std::cout << "                                                     " << std::endl;
	}
	else
	{
		GoToXY(0, InputOriginY + 1);
		std::cout << message << std::endl;
	}
	DrawLogs(inputarr);
	GoToXY(InputOriginX, InputOriginY);
}
void UI::ClearConsole()
{
	system("Cls");
}
void UI::DrawRule()
{
	cout << "<< Hangman >>" << endl;
	cout << "--------------------------" << endl;
	cout << "[규칙]" << endl;
	cout << "당신은 제한된 목숨을 가지고 숨겨진 단어를 완성시켜야 합니다." << endl;
	cout << "1.  매 턴마다 하나의 알파벳을 입력하세요." << endl;
	cout << "2-1.  숨겨진 단어에 해당 알파벳이 포함되는 경우, 해당 알파벳이 있는 위치가 드러납니다. 목숨은 차감되지 않습니다." << endl;
	cout << "2-2.  숨겨진 단어에 해당 알파벳이 포함되지 않는 경우, 목숨이 한 개 차감됩니다." << endl;
}
void UI::DrawLife(int life)
{
	GoToXY(0, 7);
	cout << "--------------------------" << endl;
	cout << "                                                  ";
	GoToXY(0, 8);
	cout << "남은 목숨 : ";
	for (int i = 0; i < life; i++)
	{
		cout << "♥";
	}
	cout << endl;
	cout << "--------------------------" << endl;
}
void UI::DrawInputArea(std::string goalWord, const char inputChar)
{
	GoToXY(2, 10);
	cout << endl;
	cout << endl;
	// 선택된 단어의 문자 개수에 따라 밑줄을 출력
	cout << " ";
	for (char c : goalWord)
	{
		COORD pos = GetCurrentCursorPos();
		pos.Y -= 1;
		m_goalWordPos.push_back(pos);
		cout << "_ ";
	}

	char upper = (char)std::toupper(inputChar); // 대문자로 변환
	for (int i = 0; i < goalWord.size(); i++)
	{
		// 같은 문자가 있으면
		if (upper == goalWord[i])
		{
			COORD pos = m_goalWordPos[i];
			GoToXY(pos.X, pos.Y);
			std::cout << goalWord[i];
			cout << endl;
		}
	}

	cout << endl;
	cout << endl;
	cout << "알파벳 입력 :                             " << endl;
	cout << endl;
}
void UI::DrawLogs(int inputar[])
{
	GoToXY(0, 16);
	cout << "--------------------------" << endl;
	cout << "[이미 사용한 알파벳]" << endl;

	// 알파벳 자리수에 맞는 위치 설정
	for (int i = 0; i < 26; i++)
	{
		int x = 3 + (i % 5) * 2;
		int y = 19 + (i / 5);
		// z만 예외로 y 옆에
		if (i == 25 && inputar[i] > 0)
		{
			x = 13;
			y = 23;
			GoToXY(x, y);
			cout << (char)(i + 65);
			break;
		}

		if (inputar[i] > 0)
		{
			GoToXY(x, y);
			cout << (char)(i + 65);
		}
	}
}
void UI::GoToXY(int x, int y)
{
	// 콘솔 출력 창에 대한 핸들을 얻습니다.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 이동할 좌표를 설정합니다.
	COORD pos;
	pos.X = (SHORT)x;
	pos.Y = (SHORT)y;

	// 커서 위치를 변경합니다.
	SetConsoleCursorPosition(hConsole, pos);
}
COORD UI::GetCurrentCursorPos()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		// 4. 성공 시 커서 위치 반환
		return csbi.dwCursorPosition;
	}
	return { 0, 0 };
}