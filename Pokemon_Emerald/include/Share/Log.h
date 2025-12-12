#pragma once

#include "../Gameinfo.h"

enum class ELogPrintType
{
	Console,
	VisualOutput,
	All
};

class CLog
{
private:
	static vector<string> mvecLog;
	static HANDLE hConsole;
	
public:
	static bool Init();
	static void Destroy();

public:
	static void PrintLog(const string& Text, ELogPrintType PrintType = ELogPrintType::All);
	static void SaveLog();

};

