#include <iostream>
#include "Menu.h"
#pragma execution_character_set("utf-8")

using namespace std;

void Menu::printHeader(const string& title) {
    cout << "\n===== " << title << " =====\n";
}

// 跨平台清屏
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 等待用户按回车
void Menu::pauseScreen() {
    cout << "\n按Enter键继续...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Menu::mainMenu(BankingSystem& bank) {
	int choice;

	while (true) {
        clearScreen();
		cout << "\n===== 银行账户管理系统 =====\n";
		cout << "1. 创建新账户\n";
        cout << "2. 存款\n";
        cout << "3. 取款\n";
        cout << "4. 转账\n";
        cout << "5. 查询余额\n";
        cout << "6. 显示交易记录\n";
        cout << "7. 显示所有账户\n";
        cout << "8. 账户管理\n";
        cout << "0. 退出系统\n";
        cout << "===========================\n";
        cout << "请选择操作: ";
        cin >> choice;
        cin.ignore(); // 防止回车影响 pauseScreen()

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效, 请重新输入!\n";
            continue;
        }

        switch (choice) {
        case 1: bank.createAccount(); break;
        case 2: bank.deposit(); break;
        case 3: bank.withdraw(); break;
        case 4: bank.transferInteractive(); break;
        case 5: bank.checkBalance(); break;
        case 6: bank.showTransactions(); break;
        case 7: bank.showAllAccounts(); break;
        case 8: bank.manageAccount(); break;
        case 0:
            cout << "感谢使用银行账户管理系统, 再见!\n";
            return;
        default:
            cout << "无效选择, 请重新输入!\n";
        }

        pauseScreen();
	}
}