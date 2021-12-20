// WakeupWatcher.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "Client.h"
#include <string>
#include "Config.h"

#include "windows.h"
#include "vfw.h"
#include <cstdio>
#include <locale>

#pragma comment(lib, "Vfw32.lib")

void takeCamPicture(const Config& cfg) {
    // create the preview window 
    HWND hCam = capCreateCaptureWindow(
        L"Watcher",
        WS_CHILD,
        0, 0, 0, 0,
        ::GetDesktopWindow(), 0);

    if (capDriverConnect(hCam, 0))
    {
        capFileSaveDIB(hCam, L"shot.bmp");
    }
    else
    {

        printf("Check camera?");

    }
    DestroyWindow(hCam);

}

int main()
{
	Config config("WakeupWatcher.cfg");

    takeCamPicture(config);

    
	Client Bot(&config);

	Channel c = Bot.getChannel("log");
    c.sendImage("shot");

	//std::cout << c.sendMessage("test").response_code;
}
