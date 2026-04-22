#pragma once
#include"HangmanView.h"
#include <conio.h>
class HangmanControler
{
public:
	HangmanControler(HangmanView& view, HangmanModel& model)
		:view(view), model(model), lastLoseScreen(false), lastWinScreen(false)
	{

	}

	void Initialize()
	{
		view.RenderMain(model);
	}
	void Run()
	{
		switch (model.GetGameState())
		{
		case GameState::Main:
			if (_kbhit())
			{
				char ch = _getch();
				model.ProcessInput(ch);
				if (model.GetGameState() == GameState::Main) {
					view.RenderMain(model);
				}
			}
			break;

		case GameState::Win:
			if (!lastWinScreen)
			{
				lastWinScreen = true;
				view.RenderWin(model);
			}
			if(_kbhit())
			{
				char ch = _getch();
				if (ch == 'y' || ch == 'Y')
				{
					lastWinScreen = false;
					model.Reset();
					view.RenderMain(model);
				}
			}
			break;

		case GameState::Lose:
			if (!lastLoseScreen)
			{
				lastLoseScreen = true;
				view.RenderLose(model);
			}
			if (_kbhit())
			{
				char ch = _getch();
				if (ch == 'y' || ch == 'Y')
				{
					lastLoseScreen = false;
					model.Reset();
					view.RenderMain(model);
				}
			}
			break;
		default:
			break;
		}
		

	}


	// -> 시작화면, 메인화면, 결과화면 등 여러개의 화면을 출력해야함.
	// 해당 화면 클래스를 만드는데, HangmanView라는 공통의 부모를 상속하도록 하자.
	// 그리고 Controller는 View를 이용해 화면을 표현하는데, 가상함수(다형성)를 이용해서 상황에 맞는 화면 출력을 진행해보자.

private:
	bool lastLoseScreen;
	bool lastWinScreen;

	HangmanView& view;
	HangmanModel& model;
};