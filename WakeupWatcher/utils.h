#pragma once
#include <string>
#include <atlstr.h>
#include <gdiplus.h>
#include <windows.h>
#include <fstream>
#include "vfw.h"
#include "Config.h"

using namespace Gdiplus;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

std::string readFileContent(std::string fName);

bool takeCamPicture(std::string file);

std::string getTimestamp();

bool isConnected();

void waitForNetAccess();