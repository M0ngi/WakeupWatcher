#include <string>

using std::string;

#pragma once
class Config
{
public:
	string TOKEN;
	string PIC_DIR;
	string SAFE_FILE;
	string KEYWORD;
	int DESKTOP_FREQ;
	int COUNTDOWN;
	string ERR_CHANN;
	string TXT_CHANN;
	string IMG_CHANN;
	string LOG_CHANN;

	Config(string);
};

