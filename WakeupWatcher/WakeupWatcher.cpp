// WakeupWatcher.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include "Config.h"
#include "utils.h"
#include "Client.h"

bool waitForSafetyFile(const Config& cfg) {
	std::fstream safe(cfg.SAFE_FILE, std::ios::out);
	safe.write("", 0);
	safe.close();
	
	Sleep(cfg.COUNTDOWN * 1000);

	safe.open(cfg.SAFE_FILE, std::ios::in);
	std::string content = std::string((std::istreambuf_iterator<char>(safe)),
		(std::istreambuf_iterator<char>()));
	safe.close();

	return std::wstring(content.begin(), content.end()) != cfg.KEYWORD;
}

int main()
{
	Config config("WakeupWatcher.cfg");
	Client Bot(&config);
    //takeCamPicture(config);

	if (waitForSafetyFile(config)) {
		waitForNetAccess();


	}
    
	

	//Channel c = Bot.getChannel("log");
    //c.sendImage("shot");

	//std::cout << c.sendMessage("test").response_code;
}
