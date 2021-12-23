// WakeupWatcher.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <thread>

#include "Config.h"
#include "utils.h"
#include "Client.h"

#pragma warning(disable: 4996) // Suppress error C4996: 'getenv': This function or variable may be unsafe

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

void cameraThread(Channel imgC, Config cfg) {
	string fName;
	while (true) {
		fName = (cfg.PIC_DIR.length() != 0 ? std::string(cfg.PIC_DIR.begin(), cfg.PIC_DIR.end()) + "/" : "") + getTimestamp();
		takeCamPicture(fName+".bmp");

		imgC.sendImage(fName);

		Sleep(1000);
		std::remove((fName + ".bmp").c_str());
		std::remove((fName + ".png").c_str());
	}
}

void GPSThread(Channel txtC, Config cfg) {
	while (true) {
		std::cout << "hi2\n";
		Sleep(1000);
	}
}

int main()
{
	Config config("WakeupWatcher.cfg");
	Client Bot(&config);
	int channelCheck = Bot.checkChannels();

	if (waitForSafetyFile(config)) {
		waitForNetAccess();

		Channel logC = Bot.getChannel("log");
		Channel txtC = Bot.getChannel("txt");
		Channel imgC = Bot.getChannel("img");

		logC.sendMessage("**WakeupWatcher**" + std::string(std::getenv("username")) + " connected.");
		
		std::thread camHandler(cameraThread, imgC, config);
		std::thread gpsHandler(GPSThread, txtC, config);

		camHandler.join(); // To prevent main from exiting
	}
    
	

	//
    //c.sendImage("shot");
}
