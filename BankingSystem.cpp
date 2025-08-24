#include "Menu.h"
#include "Account.h"
#include"AuditLogger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <limits>
#pragma execution_character_set("utf-8")

BankingSystem::BankingSystem() {
	/*cout << "[DEBUG] 正在加载数据..." << endl;*/
	loadData();
	//AuditLogger::init("audit_log.csv");  // 此功能暂时不用
}
BankingSystem::~BankingSystem() {
	/*cout << "[DEBUG] 正在析构并保存数据..." << endl;*/
	saveData();
}

void BankingSystem::loadData() {
	ifstream accFile(ACCOUNTS_FILE);
	ifstream txnFile(TRANSACTIONS_FILE);
	ifstream admFile(ADMINS_FILE);

	auto trim = [](string& s) {
		s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !isspace(ch);
			}));
		s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !isspace(ch);
			}).base(), s.end());
		};

	//if (accFile) {
	//	string number, holder, pin, statusStr;
	//	double balance;
	//	int statusInt;

	//	while (accFile >> number >> holder >> pin >> balance >> statusInt) {
	//		Account acc(number, holder, pin, balance);
	//		acc.setStatus(static_cast<AccountStatus>(statusInt));
	//		accounts.push_back(acc);
	//	}
	//}

	if (accFile.is_open()) {
		string line;
		/*bool isFirstLine = true;*/

		while (getline(accFile, line)) {
			// 效果空行和注释行
			if (line.empty() || line[0] == '#')
				continue;

			//// 跳过第一行表头
			//if (isFirstLine) {
			//	isFirstLine = false;
			//	continue;
			//}

			istringstream ss(line);
			//string number, holder, pin;
			//double balance;
			//int statusInt;

			//if (ss >> number >> holder >> pin >> balance >> statusInt) {
			//	Account acc(number, holder, pin, balance);
			//	acc.setStatus(static_cast<AccountStatus>(statusInt));
			//	accounts.push_back(acc);
			//}
			//else {
			//	cerr << "警告: 无法解析账户记录: " << line << endl;
			//}
	//		string token;
	//		vector<string> fields;

	//		while (getline(ss, token, ',')) {
	//			fields.push_back(token);
	//		}

	//		if (fields.size() != 5) {
	//			cerr << "警告: 无效的账户记录格式: " << line << endl;
	//			continue;
	//		}

	//		try {
	//			string number = fields[0];
	//			string holder = fields[1];
	//			string pin = fields[2];
	//			/*pin.erase(remove_if(pin.begin(), pin.end(), ::isspace), pin.end());*/
	//			double balance = stod(fields[3]);
	//			int statusInt = stoi(fields[4]);
	//			trim(number);
	//			trim(holder);
	//			trim(pin);

	//			Account acc(number, holder, pin, balance);
	//			acc.setStatus(static_cast<AccountStatus>(statusInt));
	//			accounts.push_back(acc);
	//		}
	//		catch (const exception& e) {
	//			cerr << "解析账户记录失败: " << line << "\n错误: " << e.what() << endl;
	//		}
	//	}
	//}
	//else {
	//	cerr << "错误: 无法打开账户文件: " << ACCOUNTS_FILE << endl;
	//}
			string accountNumber, accountHolder, hashedPin, salt, balanceStr, statusStr;

			getline(ss, accountNumber, ',');
			getline(ss, accountHolder, ',');
			getline(ss, hashedPin, ',');
			getline(ss, salt, ',');
			getline(ss, balanceStr, ',');
			getline(ss, statusStr, ',');
			//// 下边是打印所有文档中的哈希值
			//cout << hashedPin << endl;

			// 去掉可能得 \t
			hashedPin.erase(remove(hashedPin.begin(), hashedPin.end(), '\r'), hashedPin.end());

			double balance = stod(balanceStr);
			int status = stoi(statusStr);

			Account acc(accountNumber, accountHolder, hashedPin, salt, balance);
			acc.setStatus(static_cast<AccountStatus>(status));
			accounts.push_back(acc);
		}
		accFile.close();
		cout << "账户数据读取成功" << endl;
	}
	else {
		cerr << "错误: 无法读取账户数据文件" << endl;
	}

	if (txnFile.is_open()) {
		string line;
		bool isFirstLine = true;

		while (getline(txnFile, line)) {
			 //跳过空行和注释行
			if (line.empty() || line[0] == '#') {
				continue;
			}

			// 跳过第一行表头
			//if (isFirstLine) {
			//	isFirstLine = false;
			//	continue;
			//}

			// 使用字符串流解析 CSV 行
			istringstream ss(line);
			string token;
			vector<string> fields;

			// 按逗号分割字段
			while (getline(ss, token, ',')) {
				fields.push_back(token);
			}

			// 检查字段数量是否合法 (6个字段)
			if (fields.size() != 6) {
				cerr << "警告: 无效的交易记录格式: " << line << endl;
				continue;
			}

			try {
				// 解析各个字段
				const string& txnId = fields[0];
				const string& fromAccount = fields[1];
				const string& toAccount = fields[2];

				// 解析金额
				double amount = stod(fields[3]);

				// 解析交易类型 (转换为枚举)
				int typeInt = stoi(fields[4]);

				// 解析时间戳
				time_t timestamp = stoll(fields[5]);

				// 验证交易类型值
				if (typeInt < 0 || typeInt > 2) {
					throw out_of_range("无效的交易类型值");
				}

				TransactionType type = static_cast<TransactionType>(typeInt);

				// 创建交易对象
				Transaction txn(fromAccount, toAccount, amount, type);

				// 设置从文件加载的额外属性
				txn.setTransactionId(txnId);
				txn.setTimestamp(timestamp);

				// 添加到交易记录列表
				transactions.push_back(txn);
			}
			catch (const exception& e) {
				cerr << "解析交易记录失败: " << line << "\n错误: " << e.what() << endl;
			}
		}
	}
	else {
		cerr << "错误: 无法打开账户文件: " << TRANSACTIONS_FILE << endl;
	}

	if (admFile.is_open()) {
		string line;

		while (getline(admFile, line)) {
			//跳过空行和注释行
			if (line.empty() || line[0] == '#') {
				continue;
			}

			// 使用字符串流解析 CSV 行
			istringstream ss(line);

			// 按逗号分割字段
			string admId, admPin, admStatusStr;
			getline(ss, admId, ',');
			getline(ss, admPin, ',');
			getline(ss, admStatusStr, ',');

			int status = stoi(admStatusStr);

			Admin adm(admId, admPin);
			adm.setStatus(static_cast<AdminStatus>(status));
			admins.push_back(adm);
		}
		admFile.close();
	}
}

void BankingSystem::saveData() {
	ofstream accFile(ACCOUNTS_FILE, ios::trunc);
	ofstream txnFile(TRANSACTIONS_FILE, ios::trunc);

	// 保存账户数据
	if (accFile.is_open()) {
		accFile << "# 账户信息: 账号,户主,PIN哈希,Salt值,余额,状态\n";
		for (const auto& acc : accounts) {
			accFile << acc.getAccountNumber() << ","
				<< acc.getAccountHolder() << ","
				<< acc.getHashedPin() << ","
				<< acc.getSalt() << ","
				<< acc.getBalance() << ","
				<< static_cast<int>(acc.getStatus()) << "\n";
		}
		accFile.flush(); // 强制刷新
		accFile.close(); // 明确关闭
		cout << "账户数据保存成功: " << ACCOUNTS_FILE << endl;
	}
	else {
		cerr << "错误: 无法保存账户数据文件: " << ACCOUNTS_FILE << endl;
	}

	// 保存交易数据
	if (txnFile.is_open()) {
		//txnFile << "# Transaction Details: TransactionID,FromAccount,ToAccount,Amount,Type,Timestamp\n";
		txnFile << "# 交易信息: 交易ID,汇款账户,目的账户,交易额,交易类型,交易时间\n";
		for (const auto& txn : transactions) {
			txnFile << txn.getTransactionId() << ","
				<< txn.getFromAccount() << ","
				<< txn.getToAccount() << ","
				<< txn.getAmount() << ","
				<< static_cast<int>(txn.getType()) << ","
				<< txn.getTimestamp() << "\n";
		}
		txnFile.flush(); // 强制刷新
		txnFile.close(); // 明确关闭
		cout << "交易记录保存成功: " << TRANSACTIONS_FILE << endl;
	}
	else {
		cerr << "错误: 无法保存交易记录文件: " << TRANSACTIONS_FILE << endl;
	}
}

// 账户操作
void BankingSystem::createAccount() {
	string number, holder, pin;

	Menu::printHeader("创建账户");

	cout << "请输入账户号码: ";
	cin >> number;

	// 检查账户是否已存在
	if (findAccount(number)) {
		cout << "账户已存在!\n";
		return;
	}

	cout << "请输入账户持有人姓名: ";
	cin.ignore();
	getline(cin, holder);

	cout << "请设置6位数字PIN码: ";
	cin >> pin;

	// 验证PIN码格式
	if (pin.length() != 6 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
		cout << "PIN码必须是6位数字!\n";
		return;
	}
	
	double initialBlance = 0.0;
	cout << "请输入初始存款金额: ";
	cin >> initialBlance;

	if (initialBlance < 0) {
		cout << "初始存款不能为负数!\n";
		return;
	}

	// 创建新账户
	Account newAccount(number, holder, "", "", initialBlance, 0);
	newAccount.setPin(pin);
	accounts.push_back(newAccount);

	// 记录初始存款交易
	transactions.emplace_back("", number, initialBlance, TransactionType::DEPOSIT);

	cout << "\n账户创建成功!\n";
	newAccount.display();
}

Account* BankingSystem::findAccount(const string& accountNumber) {
	for (auto& acc : accounts) {
		if (acc.getAccountNumber() == accountNumber) {
			return &acc;
		}
	}
	return nullptr;
}

Admin* BankingSystem::findAdmin(const string& adminID) {
	for (auto& adm : admins) {
		if (adm.getAdminID() == adminID) {
			return &adm;
		}
	}
	return nullptr;
}

// 交易操作
void BankingSystem::deposit() {
	string number, pin;
	double amount;

	Menu::printHeader("存款操作");
	cout << "请输入账户号码: ";
	cin >> number;

	Account* account = findAccount(number);
	if (!account) {
		cout << "账户不存在!\n";
		return;
	}

	if (account->getStatus() != AccountStatus::ACTIVE) {
		cout << "账户状态异常，无法操作!\n";
		return;
	}

	cout << "请输入PIN码: ";
	cin >> pin;

	if (!account->verifyPin(pin)) {
		cout << "PIN码错误!\n";
		return;
	}

	cout << "请输入存款金额: ";
	cin >> amount;

	if (account->deposit(amount)) {
		// 记录存款交易
		transactions.emplace_back("", number, amount, TransactionType::DEPOSIT);
		cout << "存款成功! 当前余额: £"
			<< fixed << setprecision(2)
			<< account->getBalance() << endl;
	}
	else {
		cout << "存款失败! 金额无效.\n";
	}
}

void BankingSystem::withdraw() {
	string number, pin;
	double amount;

	Menu::printHeader("取款操作");
	cout << "请输入账户号码: ";
	cin >> number;

	Account* account = findAccount(number);
	if (!account) {
		cout << "账户不存在!\n";
		return;
	}

	if (account->getStatus() != AccountStatus::ACTIVE) {
		cout << "账户状态异常，无法操作!\n";
		return;
	}

	cout << "请输入PIN码: ";
	cin >> pin;

	if (!account->verifyPin(pin)) {
		cout << "PIN码错误!\n";
		return;
	}

	cout << "请输入取款金额: ";
	cin >> amount;

	if (account->withdraw(amount)) {
		// 记录取款交易
		transactions.emplace_back(number, "", amount, TransactionType::WITHDRAWAL);
		cout << "取款成功! 当前余额: £"
			<< fixed << setprecision(2)
			<< account->getBalance() << endl;
	}
	else {
		cout << "取款失败! 金额无效或余额不足.\n";
	}

	//cout << "按任意键继续...";
	cin.get();
	return;
}

void BankingSystem::transfer() {
	string fromNumber, toNumber, pin;
	double amount;

	Menu::printHeader("转账管理");
	cout << "请输入转出账户号码: ";
	cin >> fromNumber;

	Account* fromAccount = findAccount(fromNumber);
	if (!fromAccount) {
		cout << "转出账户不存在!\n";
		return;
	}

	if (fromAccount->getStatus() != AccountStatus::ACTIVE) {
		cout << "转出账户状态异常，无法操作!\n";
		return;
	}

	cout << "请输入PIN码: ";
	cin >> pin;

	if (!fromAccount->verifyPin(pin)) {
		cout << "PIN码错误!\n";
		return;
	}

	cout << "请输入转入账户号码: ";
	cin >> toNumber;

	Account* toAccount = findAccount(toNumber);
	if (!toAccount) {
		cout << "转入账户不存在!\n";
		return;
	}

	if (toAccount->getStatus() != AccountStatus::ACTIVE) {
		cout << "转入账户状态异常，无法操作!\n";
		return;
	}

	if (fromNumber == toNumber) {
		cout << "不能转账到同一账户!\n";
		return;
	}

	cout << "请输入转账金额: ";
	cin >> amount;

	if (fromAccount->transfer(*toAccount, amount)) {
		// 记录转账交易
		transactions.emplace_back(fromNumber, toNumber, amount, TransactionType::TRANSFER);
		cout << "转账成功!\n";
		cout << "转出账户余额: £"
			<< fixed << setprecision(2)
			<< fromAccount->getBalance() << endl;
		cout << "转入账户余额: £"
			<< fixed << setprecision(2)
			<< toAccount->getBalance() << endl;
	}
	else {
		cout << "转账失败! 金额无效或余额不足.\n";
	}
}

void BankingSystem::checkBalance() {
	string number, pin;

	Menu::printHeader("余额查询");
	cout << "请输入账户号码: ";
	cin >> number;

	Account* account = findAccount(number);
	if (!account) {
		cout << "账户不存在!\n";
		return;
	}

	cout << "请输入PIN码: ";
	cin >> pin;

	if (!account->verifyPin(pin)) {
		cout << "PIN码错误!\n";
		return;
	}

	account->display();
}

// 信息查询
void BankingSystem::showTransactions() {
	Menu::printHeader("Record");

	if (transactions.empty()) {
		cout << "暂无交易记录\n";
		return;
	}

	// 按时间倒序排列（最近的交易在前）
	vector<Transaction> sortedTransactions = transactions;
	sort(sortedTransactions.begin(), sortedTransactions.end(), [](const Transaction& a, const Transaction& b) {
		return a.getTimestamp() > b.getTimestamp();
		});

	for (const auto& txn : sortedTransactions) {
		txn.display();
		cout << "------------------------------------\n";
	}
}

void BankingSystem::showAllAccounts() {
	Menu::printHeader("账户信息");

	if (accounts.empty()) {
		cout << "暂无账户信息\n";
		return;
	}

	for (const auto& acc : accounts) {
		acc.display();
		cout << "------------------------------------\n";
	}
}

void BankingSystem::manageAccount() {
	string number, pin;

	Menu::printHeader("账户管理");
	cout << "请输入账户号码: ";
	cin >> number;

	Account* account = findAccount(number);
	if (!account) {
		cout << "账户不存在!\n";
		return;
	}

	//// 管理员验证
	//string adminPin;
	//cout << "请输入管理员PIN码: ";
	//cin >> adminPin;

	//// 简单管理员验证（实际应用中应更安全）
	//if (adminPin != "admin123") {
	//	cout << "管理员验证失败!\n";
	//	return;
	//}

	// 管理员验证
	string adm_id, adm_pin;
	cout << "请输入管理员ID: ";
	cin >> adm_id;
	Admin* admin = findAdmin(adm_id);
	if (!admin) {
		cout << "管理员不存在!\n";
		return;
	}

	cout << "请输入管理员PIN码: ";
	cin >> adm_pin;
	if (admin->getAdminPin() != adm_pin) {
		cout << "管理员验证失败!\n";
		return;
	}

	int choice;
	cout << "\n当前账户状态: ";
	switch (account->getStatus()) {
	case AccountStatus::ACTIVE: cout << "正常"; break;
	case AccountStatus::FROZEN: cout << "冻结"; break;
	case AccountStatus::CLOSED: cout << "关闭"; break;
	}
	cout << "\n\n";

	cout << "请选择操作:\n";
	cout << "1. 冻结账户\n";
	cout << "2. 解冻账户\n";
	cout << "3. 关闭账户\n";
	cout << "0. 返回\n";
	cout << "选择: ";
	cin >> choice;

	switch(choice) {
	case 1:
		account->setStatus(AccountStatus::FROZEN);
		cout << "账户已冻结\n";
		break;
	case 2:
		account->setStatus(AccountStatus::ACTIVE);
		cout << "账户已解冻\n";
		break;
	case 3:
		account->setStatus(AccountStatus::CLOSED);
		cout << "账户已关闭\n";
		break;
	case 0:
		return;
	default:
		cout << "无效选择\n";
	}
}