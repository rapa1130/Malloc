#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <random>
#include <algorithm>

#include "GameMode.h"

using namespace std;

int main()
{
    GameMode* GM = new GameMode();

    GM->StartGame();
}