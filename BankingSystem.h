#pragma once
#include "Account.h"
#include "Transaction.h"
#include "Admin.h"
#include "Menu.h"
#include <vector>
#include <string>
#include <fstream>

class BankingSystem {
private:
	vector<Account> accounts;
	vector<Transaction> transactions;
	vector<Admin> admins;
	const string ACCOUNTS_FILE = "accounts.txt";
	const string TRANSACTIONS_FILE = "transactions.txt";
	const string ADMINS_FILE = "admins.txt";

	void loadData();
	void saveData();

public:
	BankingSystem();
	~BankingSystem();

	// 用户账户操作
	void createAccount();
	void addAccount(const Account& acc);
	Account* findAccount(const string& accountNumber);
	Admin* findAdmin(const string& adminID);

	// 交易操作
	void deposit();
	void withdraw();
	//void transfer();
	bool transfer(const string& fromNumber, const string& toNumber, double amount, const string& pin);
	void transferInteractive();
	void checkBalance();

	// 信息查询
	void showTransactions();
	void showAllAccounts();
	void manageAccount();
};