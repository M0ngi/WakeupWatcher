#include "Config.h"
#include <fstream>
#include <string>
#include <iostream>
#include <locale>

using namespace std;


Config::Config(string name) {

	ifstream configFile;
	configFile.open(name);
	if (!configFile) {
		exit(-1);
	}
	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > conv;

	string tmp;
	while (getline(configFile, tmp)) {
		if (!tmp.find("token=")) TOKEN = conv.from_bytes(tmp.substr(6));
		else if (!tmp.find("pic_dir=")) PIC_DIR = conv.from_bytes(tmp.substr(8));
		else if (!tmp.find("safe=")) SAFE_FILE = conv.from_bytes(tmp.substr(5));
		else if (!tmp.find("keyword=")) KEYWORD = conv.from_bytes(tmp.substr(8));
		else if (!tmp.find("err_ch=")) ERR_CHANN = conv.from_bytes(tmp.substr(7));
		else if (!tmp.find("txt_ch=")) TXT_CHANN = conv.from_bytes(tmp.substr(7));
		else if (!tmp.find("img_ch=")) IMG_CHANN = conv.from_bytes(tmp.substr(7));
		else if (!tmp.find("log_ch=")) LOG_CHANN = conv.from_bytes(tmp.substr(7));
		else if (!tmp.find("count=")) COUNTDOWN = stoi(tmp.substr(6));
		else if (!tmp.find("desktop_freq=")) DESKTOP_FREQ = stoi(tmp.substr(13));
	}
}

