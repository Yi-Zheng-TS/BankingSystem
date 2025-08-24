#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#pragma execution_character_set("utf-8")

Transaction::Transaction(string from, string to, double amt, TransactionType t)
: fromAccount(from), toAccount(to), amount(amt), type(t) {
	timestamp = time(nullptr);
	generateId();
}

void Transaction::generateId() {
	stringstream ss;
	ss << "TXN" << setfill('0') << setw(10) << timestamp % 10000000000;
	transactionId = ss.str();
}

// 设置从文件加载的属性
void Transaction::setTransactionId(const string& id) {
	transactionId = id;
}
void Transaction::setTimestamp(time_t ts) {
	timestamp = ts;
}

// 获取交易信息
string Transaction::getTransactionId() const {
	return transactionId;
}
string Transaction::getFromAccount() const {
	return fromAccount;
}
string Transaction::getToAccount() const {
	return toAccount;
}
double Transaction::getAmount() const {
	return amount;
}
TransactionType Transaction::getType() const {
	return type;
}
time_t Transaction::getTimestamp() const {
	return timestamp;
}

// 显示和获取信息
void Transaction::display() const {
	char timeStr[32];
	tm tm_local;
	//跨平台时间转换
	#if defined(_WIN32)
	localtime_s(&tm_local, &timestamp);
	#else
	localtime_r(&timestamp, &tm_local);
	#endif

	//strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime_r(&timestamp , &tm_local));
	strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tm_local);

	cout << "交易ID: " << transactionId << endl;
	cout << "时间: " << timeStr << endl;

	switch (type) {
	case TransactionType::DEPOSIT:
		cout << "类型: 存款" << endl;
		cout << "存入账户: " << toAccount << endl;
		break;
	case TransactionType::WITHDRAWAL:
		cout << "类型: 取款" << endl;
		cout << "取出账户: " << fromAccount << endl;
		break;
	case TransactionType::TRANSFER:
		cout << "类型: 转账" << endl;
		cout << "从账户: " << fromAccount << endl;
		cout << "到账户: " << toAccount << endl;
		break;
	default:
		cout << "类型: 未知(" << static_cast<int>(type) << ")" << endl;
		break;
	}

	cout << "金额: £" << fixed << setprecision(2) << amount << endl;
	/*cout << "金额: £" << getCurrencySymbol() << setprecision(2) << amount << endl;*/
}

string Transaction::getInfo() const {
	stringstream ss;
	ss << transactionId << ","
		<< fromAccount << ","
		<< toAccount << ","
		<< amount << ","
		<< static_cast<int>(type) << ","
		<< timestamp;
	return ss.str();
}