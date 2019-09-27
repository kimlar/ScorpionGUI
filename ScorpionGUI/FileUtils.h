#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#define RUNNING_WINDOWS
//#define RUNNING_LINUX

namespace FileUtils
{
    #ifdef RUNNING_WINDOWS
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Win32 code blob:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//

    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <Shlwapi.h> // NOTE: Must also be linked with: shlwapi.lib
    inline std::string GetWorkingDirectory()
    {
		HMODULE hModule = GetModuleHandle(nullptr);
        if (hModule)
        {
            char path[256];
            GetModuleFileName(hModule,path,sizeof(path));
            PathRemoveFileSpec(path);
			strcat_s(path, "\\");
			
			return std::string(path);			
        }
        return "";
    }

	//
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// End of code blob
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    #elif defined RUNNING_LINUX
    #include <unistd.h>
    inline std::string GetWorkingDirectory()
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr)
        {
            return std::string(cwd) + std::string("/");
        }
        return "";
    }
    #endif
}
