#pragma once
#include <string>

using namespace std;

struct SecurityUtils {
	static string sha256(const string& input);
	static string randomSalt(size_t bytes = 16); // 16字节盐 => 32位hex
};