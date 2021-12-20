#include <string>

using std::wstring;
using std::string;

#pragma once
class Config
{
public:
	wstring TOKEN;
	wstring PIC_DIR;
	wstring SAFE_FILE;
	wstring KEYWORD;
	int DESKTOP_FREQ = 0;
	int COUNTDOWN = 0;
	wstring ERR_CHANN;
	wstring TXT_CHANN;
	wstring IMG_CHANN;
	wstring LOG_CHANN;

	Config(string);
};

