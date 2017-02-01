#pragma once

#define RUNNING_WINDOWS
#include <iostream>
#include <string>
#include <algorithm>

namespace Utils {
#ifdef RUNNING_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <Shlwapi.h>
	inline std::string GetWorkingDirectory() {
		HMODULE hModule = GetModuleHandle(nullptr);
		if (hModule) {
			char path[256];
			GetModuleFileNameA(hModule, path, sizeof(path));
			PathRemoveFileSpecA(path);
			strcat_s(path, "\\");
			return std::string(path);
		}
		return "";
	}
#elif defined RUNNING_LINUX
	#include <unistd.h>
	inline std::string GetWorkingDirectory() {
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != nullptr) {
			return std::string(cwd) + std::string("/");
		}
		return "";
	}
#endif

	inline void ReadQuotedString(std::stringstream& stream, std::string& str) {
		stream >> str;
		if (str.at(0) == '"') {
			while (str.at(str.length() - 1) != '"' || !stream.eof()) {
				std::string str2;
				stream >> str2;
				str.append(" " + str2);
			}
		}
		str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
	}
}