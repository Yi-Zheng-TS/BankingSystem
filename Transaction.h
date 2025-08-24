#pragma once
#include <string>
#include <ctime>

using namespace std;

// 交易类型枚举
enum class TransactionType {
	DEPOSIT,
	WITHDRAWAL,
	TRANSFER
};

class Transaction {
private:
	string transactionId;
	string fromAccount;
	string toAccount;
	double amount;
	time_t timestamp;
	TransactionType type;

	void generateId();

public:
	Transaction(string from, string to, double amt, TransactionType t);

	// 设置从文件加载的属性
	void setTransactionId(const string& id);
	void setTimestamp(time_t ts);

	// 获取交易信息
	string getTransactionId() const;
	string getFromAccount() const;
	string getToAccount() const;
	double getAmount() const;
	TransactionType getType() const;
	time_t getTimestamp() const;

	// 显示和获取信息
	void display() const;
	string getInfo() const;
};