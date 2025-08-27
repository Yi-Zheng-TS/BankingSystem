#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include<openssl/evp.h>
#include<openssl/err.h>

using namespace std;

// 账户状态枚举
enum class AdminStatus {
	ACTIVE,
	FROZEN,
	CLOSED
};

class Admin {
private:
	string adminID;
	string adminPin;
	AdminStatus status;

public:
	Admin(string id, string pin);

	// 访问器
	string getAdminID() const;
	string getAdminPin() const;
	//AdminStatus getStatus() const;

	// 状态管理
	void setStatus(AdminStatus newStatus);
};
