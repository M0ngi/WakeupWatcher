#include <string>
#include <atlstr.h>
#include <gdiplus.h>
#include <windows.h>
#include <fstream>

#pragma comment(lib, "cpprest141_2_10.lib")
#pragma comment (lib,"Gdiplus.lib")

using std::wstring;
using namespace Gdiplus;

inline int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
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

inline std::string readFileContent(std::string fName) {
	// Converts the file to PNG then read it's content
	// Camera results in BMP file always so we could convert the file directly without checking
	// Unless this changes in the future
	
	// Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CLSID encoderClsid;
	Gdiplus::Status  stat;
	Gdiplus::Image* image = new Gdiplus::Image((wstring(fName.begin(), fName.end()) + L".bmp").c_str());

	// Get the CLSID of the PNG encoder.
	GetEncoderClsid(L"image/png", &encoderClsid);

	stat = image->Save((wstring(fName.begin(), fName.end()) + L".png").c_str(), &encoderClsid, NULL);

	std::ifstream file(fName + ".png", std::ios::in | std::ios::binary);
	std::string str = std::string((std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()));
	file.close();
	return str;
}