#include <cstdlib>
#include <iostream>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include "Menu.h"
#include "BankingSystem.h"
#pragma execution_character_set("utf-8")

using namespace std;

int main() {
	// 设置控制台编码为UTF-8以支持中文
	system("chcp 65001 > nul");

	BankingSystem bank;

	cout << "欢迎使用银行账户管理系统！\n";

	Menu::mainMenu(bank);

	cout << "\n感谢使用银行账户管理系统，再见!\n";

	return 0;
}