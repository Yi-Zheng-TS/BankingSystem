#pragma once
#include <string>

using namespace std;

struct AuditLogger {
	static void init(const string& path); // 设置日志文件路径
	static void login(const string& account, bool success, const string& reason = "");
	static void txn(const string& fromAcc, const string& toAcc, double amount, bool success, const string& reason = "");

private:
	static string& file();
	static void ensureHeader();
	static string nowTs(); // time_t 字符串
};
