#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <random>
#include <algorithm>

#include "LSGameMode.h"

using namespace std;

int main()
{
    LSGameMode* GM = new LSGameMode();

    GM->StartGame();
}