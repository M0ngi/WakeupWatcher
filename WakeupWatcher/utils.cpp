#include "utils.h"
#include <string>
#include <atlstr.h>
#include <gdiplus.h>
#include <windows.h>
#include <fstream>
#include "vfw.h"
#include "Config.h"
#include <chrono>
#include <wininet.h>
#include <iostream>

#pragma comment(lib,"Wininet.lib")
#pragma comment(lib, "cpprest141_2_10.lib")
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "Vfw32.lib")

using namespace Gdiplus;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

std::string readFileContent(std::string fName) {
	// Converts the file to PNG then read it's content
	// Camera results in BMP file always so we could convert the file directly without checking
	// Unless this changes in the future
	
	// Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CLSID encoderClsid;
	Gdiplus::Status  stat;
	Gdiplus::Image* image = new Gdiplus::Image((std::wstring(fName.begin(), fName.end()) + L".bmp").c_str());

	// Get the CLSID of the PNG encoder.
	GetEncoderClsid(L"image/png", &encoderClsid);

	stat = image->Save((std::wstring(fName.begin(), fName.end()) + L".png").c_str(), &encoderClsid, NULL);
	delete image; // Free to be able to delete

	std::ifstream file(fName + ".png", std::ios::in | std::ios::binary);
	std::string str = std::string((std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()));
	file.close();
	return str;
}

bool takeCamPicture(std::string file) {
	// create the preview window 
	HWND hCam = capCreateCaptureWindow(
		L"CAPTURE",
		WS_CHILD,
		0, 0, 0, 0,
		::GetDesktopWindow(), 0);

	if (capDriverConnect(hCam, 0))
	{
		capFileSaveDIB(hCam, std::wstring(file.begin(), file.end()).c_str());
		DestroyWindow(hCam);
		return true;
	}
	DestroyWindow(hCam);
	return false;
}

std::string getTimestamp() {
	const auto p1 = std::chrono::system_clock::now();

	return std::to_string( std::chrono::duration_cast<std::chrono::seconds>(
			p1.time_since_epoch()).count());
}

bool isConnected() {
	return InternetCheckConnection(L"http://google.com", FLAG_ICC_FORCE_CONNECTION, 0);
}

void waitForNetAccess() {
	while (!isConnected()) {
		Sleep(1000);
	}
}