#include "Config.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;


Config::Config(string name) {

	ifstream configFile;
	configFile.open(name);
	if (!configFile) {
		exit(-1);
	}

	string tmp;
	while (getline(configFile, tmp)) {
		if (!tmp.find("token=")) TOKEN = tmp.substr(6);
		else if (!tmp.find("pic_dir=")) PIC_DIR = tmp.substr(8);
		else if (!tmp.find("safe=")) SAFE_FILE = tmp.substr(5);
		else if (!tmp.find("keyword=")) KEYWORD = tmp.substr(8);
		else if (!tmp.find("err_ch=")) ERR_CHANN = tmp.substr(7);
		else if (!tmp.find("txt_ch=")) TXT_CHANN = tmp.substr(7);
		else if (!tmp.find("img_ch=")) IMG_CHANN = tmp.substr(7);
		else if (!tmp.find("log_ch=")) LOG_CHANN = tmp.substr(7);
		else if (!tmp.find("count=")) COUNTDOWN = stoi(tmp.substr(6));
		else if (!tmp.find("desktop_freq=")) COUNTDOWN = stoi(tmp.substr(13);
	}
}

