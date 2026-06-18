#include "Utility.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

namespace Utility {

std::string WStringToUTF8(const std::wstring& wstr)
{
	if (wstr.empty())
	{
		return std::string();
	}

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

	return strTo;
}

} // namespace Utility