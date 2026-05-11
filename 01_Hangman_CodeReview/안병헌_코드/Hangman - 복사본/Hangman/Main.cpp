#include<iostream>
#include"HangmanController.h"
#include"HangmanModel.h"
#include"HangmanView.h"
using namespace std;

int main()
{
	HangmanModel model;
	HangmanView view;
	HangmanControler controler(view, model);

	controler.Initialize();
	while (true)
	{
		controler.Run();
	}
}