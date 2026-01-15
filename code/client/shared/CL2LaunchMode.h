#pragma once

inline int GetCLNumber()
{
	static auto clNumber = ([]()
	{
#ifndef IS_FXSERVER
		const wchar_t* cmdLine = GetCommandLineW();
		const wchar_t* clPos = wcsstr(cmdLine, L"-cl");
		
		if (clPos != nullptr)
		{
			clPos += 3;
			wchar_t* endPtr;
			long num = wcstol(clPos, &endPtr, 10);
			
			if (endPtr != clPos && num >= 2)
			{
				return (int)num;
			}
		}
#endif

		return 0;
	})();

	return clNumber;
}

inline bool IsCLX()
{
	return GetCLNumber() >= 2;
}

namespace launch
{
inline bool IsSDKGuest()
{
	static auto isSdkGuest = ([]()
	{
#ifndef IS_FXSERVER
		if (getenv("CitizenFX_SDK_Guest"))
		{
			return true;
		}
#endif

		return false;
	})();

	return isSdkGuest;
}

inline bool IsFXNode()
{
	static auto isFXNode = ([]()
	{
#ifndef IS_FXSERVER
		if (wcsstr(GetCommandLineW(), L"--start-node") != nullptr)
		{
			return true;
		}
#endif

		return false;
	})();

	return isFXNode;
}

inline bool IsSDK()
{
	static auto isSdk = ([]()
	{
#ifndef IS_FXSERVER
		if (wcsstr(GetCommandLineW(), L"fxdk") != nullptr && !IsSDKGuest())
		{
			return true;
		}
#endif

		return false;
	})();

	return isSdk;
}

inline const std::string& GetLaunchModeKey()
{
	static thread_local std::string launchKey = ([]()
	{
		if (IsSDK() || IsSDKGuest())
		{
			return "fxdk";
		}
		else if (IsCLX())
		{
			int clNum = GetCLNumber();
			return "cl" + std::to_string(clNum);
		}

		return "";
	})();

	return launchKey;
}

inline std::string GetPrefixedLaunchModeKey(std::string_view prefix)
{
	const auto& lm = GetLaunchModeKey();

	if (lm.empty())
	{
		return "";
	}

	return std::string{ prefix } + lm;
}

inline const std::string& GetProductKey()
{
	static thread_local std::string launchKey = ([]()
	{
#ifdef IS_FXSERVER
		return "SV";
#elif defined(GTA_FIVE) || defined(IS_RDR3)
		return "CFXGame";
#else
		return "CFX";
#endif
	})();

	return launchKey;
}
}
