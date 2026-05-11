#include "LSSave.h"
#include <iostream>

using namespace std;

bool LSSave::CheckSave()
{
	return Save > 0;
}

void LSSave::ApplyDamage()
{
    Save--;
}

void LSSave::SetMaxSave(int InLength)
{
    MaxSave = 5 + InLength;
	Save = MaxSave;
}

void LSSave::PrintCurrentSave()
{
    cout << "Life : ";
    for (int i = 0; i < Save; i++)
    {
        cout << "бс";
    }
    for (int i = 0; i < MaxSave - Save; i++)
    {
        cout << "бр";
    }
}
