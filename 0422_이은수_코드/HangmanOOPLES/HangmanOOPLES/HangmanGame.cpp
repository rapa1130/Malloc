#include "HangmanGame.h"

HangmanGame::HangmanGame()
{
	m_data = new Data();
	m_ui = new UI();
	m_inputChar = '\0';
	m_isGameComplete = false;
}

HangmanGame::~HangmanGame()
{
	delete m_data;
	delete m_ui;
}

void HangmanGame::GameStart()
{
	bool isAppRunning = true;

	while (isAppRunning)
	{
		m_ui->InitUI(*m_data);
		GameLoop();

		// 게임이 끝난 후 처리
		std::cout << "\n게임을 다시 하시겠습니까? (Y/N): ";
		char choice;
		std::cin >> choice;

		if (std::tolower(choice) == 'y')
		{
			ResetGame(); // 데이터 초기화 후 루프 반복
		}
		else
		{
			isAppRunning = false;
		}
	}
}

void HangmanGame::GameLoop()
{
	// 게임 완료 플래그가 세워질 때까지만 반복
	while (!m_isGameComplete)
	{
		ProcessingInput();
		ProcessingGameLogic();
		ProcessingRender();
	}
}

void HangmanGame::ProcessingInput()
{
	std::string s;
	// s[0]에 바로 넣기보다 안전하게 문자열로 입력받음
	if (!(std::cin >> s)) return;

	if (CheckValidateInput(s[0]))
	{
		m_inputChar = s[0];
		m_upper = (char)std::toupper(m_inputChar);
		m_message = "";
		m_inputAlphabets[(int)s[0] - 'a']++;
	}
	else
	{
		m_inputChar = '\0';
		m_upper = '\0';
		m_message = "영문 알파벳 26자(소문자)만 입력할 수 있습니다.";
	}
	std::cin.ignore(1000, '\n');
}

void HangmanGame::ProcessingGameLogic()
{
	// 유효하지 않은 입력은 로직 건너뜀
	if (m_upper == '\0') return;

	int checkIndex = m_upper - 'A';
	// 이미 입력한 문자인지 체크
	if (m_inputAlphabets[m_inputChar - 'a'] > 1) return;

	std::string goalWord = m_data->GetGoalWord();
	int life = m_data->GetLife();

	if (m_data->CheckAnswer(m_upper))
	{
		m_ui->DrawWins(goalWord, m_data->GetLife());
		m_isGameComplete = true; // 루프 탈출 조건
	}
	else if (m_data->GetLife() <= 0)
	{
		m_ui->DrawLose(goalWord, 0);
		m_isGameComplete = true; // 루프 탈출 조건
	}
}

void HangmanGame::ProcessingRender()
{
	if (m_isGameComplete) return; // 게임 종료 시에는 중복 렌더링 방지

	int life = m_data->GetLife();
	std::string goalWord = m_data->GetGoalWord();
	m_ui->Render(life, goalWord, m_inputChar, m_message, m_inputAlphabets);
}

bool HangmanGame::CheckValidateInput(char c)
{
	return (c >= 'a' && c <= 'z');
}

void HangmanGame::ResetGame()
{
	// 기존 데이터 메모리 해제 및 재생성
	delete m_data;
	m_data = new Data();

	// 모든 상태 변수 초기화
	for (int i = 0; i < 26; ++i) m_inputAlphabets[i] = 0;
	m_isGameComplete = false;
	m_inputChar = '\0';
	m_upper = '\0';
	m_message = "";
}