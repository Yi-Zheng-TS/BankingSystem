#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include<openssl/evp.h>
#include<openssl/err.h>

using namespace std;

// 账户状态枚举
enum class AccountStatus {
	ACTIVE,
	FROZEN,
	CLOSED
};

class Account {
private:
	string number_;
	string holder_;
	double balance_;
	string hashedPin_;
	string salt_;
	AccountStatus status_;
	int failedAttempts_; // 连续失败次数

public:
	Account(const string& number, const string& holder, const string& pin, const string& salt, double initialBalance = 0.0, int status = 0);

	// 新增：带盐设置PIN
	void setPin(const string& pin);

	// 新版校验：失败累加, >=3 次自动冻结
	bool verifyPin(const string& pin);

	// PIN码管理
	/*static string hashPin(const string& pin);*/
	/*bool verifyPin(const string& pin);*/
	void setHashedPin(const string& h);

	void setSalt(const string& s);
	void setFailedAttempts(int n);

	// 账户操作
	bool deposit(double amount);
	bool withdraw(double amount);
	bool transfer(Account& recipient, double amount);

	// 访问器
	double getBalance() const;
	const string& getAccountNumber() const;
	const string& getAccountHolder() const;
	AccountStatus getStatus() const;
	const string& getHashedPin() const;
	const string& getSalt() const;
	int getFailedAttempts() const;

	// 状态管理
	void setStatus(AccountStatus newStatus);

	// 显示信息
	void display() const;
};
