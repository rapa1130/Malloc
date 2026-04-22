#pragma once
class LSSave
{
public:
	bool CheckSave();
	void ApplyDamage();
	void SetMaxSave(int InLength);
	void PrintCurrentSave();

protected:
	int MaxSave = 0;
	int Save = 0;
};

