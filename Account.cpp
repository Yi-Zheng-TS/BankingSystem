#include "Account.h"
#include"SecurityUtils.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#pragma execution_character_set("utf-8")

using namespace std;

Account::Account(const string& number, const string& holder, const string& pin, const string& salt, double initialBalance, int status)
: number_(number), holder_(holder), hashedPin_(pin),salt_(salt), balance_(initialBalance), status_(AccountStatus::ACTIVE), failedAttempts_(0) {
	/*hashedPin = hashPin(pin); // 这里不能再用hashPin()函数将十进制pin值变成哈希值 因为从文档中下载的值已经是哈希值*/
}

//string Account::hashPin(const string& pin) {
//	//unsigned char hash[SHA256_DIGEST_LENGTH];
//	//SHA256_CTX sha256;
//	//SHA256_Init(&sha256);
//	//SHA256_Update(&sha256, pin.c_str(), pin.size());
//	//SHA256_Final(hash, &sha256);
//
//	//stringstream ss;
//	//for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
//	//	ss << hex << setw(2) << setfill('0') << (int)hash[i];
//	//}
//	//return ss.str();
//
//	// 创建消息摘要上下文
//	EVP_MD_CTX* context = EVP_MD_CTX_new();
//	if (!context) {
//		throw runtime_error("无法创建哈希上下文");
//	}
//
//	// 初始化 SHA-256 哈希操作
//	if (1 != EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {
//		EVP_MD_CTX_free(context);
//		throw runtime_error("哈希初始化失败");
//	}
//
//	// 添加 PIN 数据到哈希计算
//	if (1 != EVP_DigestUpdate(context, pin.c_str(), pin.size())) {
//		EVP_MD_CTX_free(context);
//		throw runtime_error("哈希更新失败");
//	}
//
//	// 获取哈希结果
//	unsigned char hash[EVP_MAX_MD_SIZE];
//	unsigned int hashLength = 0;
//	if (1 != EVP_DigestFinal_ex(context, hash, &hashLength)) {
//		EVP_MD_CTX_free(context);
//		throw runtime_error("哈希计算失败");
//	}
//
//	// 释放上下文资源
//	EVP_MD_CTX_free(context);
//
//	// 确保我们得到的是 SHA256 哈希 (32字节)
//	if (hashLength != SHA256_DIGEST_LENGTH) {
//		throw runtime_error("无效的哈希长度");
//	}
//
//	// 将二进制哈希转换为十六进制字符串
//	stringstream ss;
//	for (unsigned int i = 0; i < hashLength; i++) {
//		ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
//	}
//
//	return ss.str();
//}

// 新增：带盐设置PIN
void Account::setPin(const string& pin) {
	salt_ = SecurityUtils::randomSalt(16);
	hashedPin_ = SecurityUtils::sha256(salt_ + pin);
	failedAttempts_ = 0; // 设置新PIN时清零
}

bool Account::verifyPin(const string& pin) {
	// 测试输入和存储的哈希值的长度
	//cout << "Stored: " << hashedPin << " (len=" << hashedPin.length() << ")\n";
	//for (unsigned char c : hashedPin) {
	//	cout << (int)c << " "; // 输出每个字符的 ASCII 值
	//}
	//cout << "\nInput : " << hashPin(pin) << " (len=" << hashPin(pin).length() << ")\n";

	if (status_ == AccountStatus::FROZEN || status_ == AccountStatus::CLOSED) {
		return false;
	}

	// 兼容：若旧账户没有盐(salt_ 为空) 视为旧版 提示迁移或按旧逻辑处理
	if (salt_.empty()) {
		// 若你仍有旧版 hashPin(pin) 的逻辑 可在此调用以兼容
		// 这里直接按“无盐但已有hash”的方式校验
		bool ok = (hashedPin_ == SecurityUtils::sha256(pin)); //旧数据兜底
		if (ok) {
			failedAttempts_ = 0;
		}
		else {
			if (++failedAttempts_ >= 3) status_ = AccountStatus::FROZEN;
		}
		return ok;
	}

	const string candidate = SecurityUtils::sha256(salt_ + pin);
	bool ok = (candidate == hashedPin_);
	if (ok) {
		failedAttempts_ = 0;
	}
	else {
		if (++failedAttempts_ >= 3) {
			status_ = AccountStatus::FROZEN; // 自动冻结
		}
	}

	return ok;

	//// 测试输入和存储的哈希值
	//string inputHash = hashPin(pin);
	//// 下边是测试代码
	////cout << "输入PIN哈希: " << inputHash << " 长度=" << inputHash.size() << endl;
	////cout << "存储PIN哈希: " << hashedPin << " 长度=" << hashedPin.size() << endl;
	
	/*return hashedPin_ == inputHash;*/
}

void Account::setSalt(const string& s) {
	salt_ = s;
}

void Account::setFailedAttempts(int n) {
	failedAttempts_ = n;
}

void Account::setHashedPin(const string& h) {
	hashedPin_ = h;
}

bool Account::deposit(double amount) {
	if (amount <= 0) return false;
	balance_ += amount;
	return true;
}

bool Account::withdraw(double amount) {
	if (amount <= 0 || amount > balance_) return false;
	balance_ -= amount;
	return true;
}

bool Account::transfer(Account& recipient, double amount) {
	if (amount <= 0 || amount > balance_) return false;
	balance_ -= amount;
	recipient.balance_ += amount;
	return true;
}

double Account::getBalance() const {
	return balance_;
}

const string& Account::getAccountNumber() const {
	return number_;
}

const string& Account::getAccountHolder() const {
	return holder_;
}

AccountStatus Account::getStatus() const {
	return status_;
}

const string& Account::getHashedPin() const {
	return hashedPin_;
}

const string& Account::getSalt() const{
	return salt_;
}

int Account::getFailedAttempts() const {
	return failedAttempts_;
}

void Account::setStatus(AccountStatus newStatus) {
	status_ = newStatus;
}

void Account::display() const {
	cout << "账户号码: " << number_ << endl;
	cout << "账户持有人: " << holder_ << endl;
	cout << "账户余额: £" << fixed << setprecision(2) << balance_ << endl;
	cout << "账户状态: ";
	switch (status_) {
	case AccountStatus::ACTIVE: cout << "正常"; break;
	case AccountStatus::FROZEN: cout << "冻结"; break;
	case AccountStatus::CLOSED: cout << "关闭"; break;
	}
	cout << endl;
}

