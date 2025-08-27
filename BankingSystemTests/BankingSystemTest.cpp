#include "pch.h" // VS 生成预编译头
#include "../BankingSystem.h" // 引入被测试的类
#pragma execution_character_set("utf-8")

// 转账相关
// 9. 测试转账成功
static TEST(BankingSystemTest, TransferSuccess) {
	BankingSystem bank;

	Account accA("03150005", "Kelin", "555555", "saltA", 5000.0, 0);
	Account accB("03150006", "Jowi", "666666", "saltB", 1000.0, 0);

	bank.addAccount(accA);
	bank.addAccount(accB);

	EXPECT_TRUE(bank.transfer("03150005", "03150006", 2000, "555555"));

	EXPECT_DOUBLE_EQ(bank.findAccount("03150005")->getBalance(), 3000.0);
	EXPECT_DOUBLE_EQ(bank.findAccount("03150006")->getBalance(), 3000.0);
}

// 10. 测试余额不足时转账失败
static TEST(BankingSystemTest, TransferInsufficientBalance) {
	BankingSystem bank;

	Account accA("03150007", "Amanda", "777777", "saltA", 500.0, 0);
	Account accB("03150008", "Jingjing", "888888", "saltB", 1000.0, 0);

	bank.addAccount(accA);
	bank.addAccount(accB);

	EXPECT_FALSE(bank.transfer("03150007", "03150008", 2000, "777777"));

	EXPECT_DOUBLE_EQ(accA.getBalance(), 500.0);
	EXPECT_DOUBLE_EQ(accB.getBalance(), 1000.0);
}

// 11. 测试 PIN 错误
static TEST(BankingSystemTest, TransferFailsWithWrongPin) {
	BankingSystem bank;

	Account accA("03150009", "Eva", "999999", "saltE", 3000.0, 0);
	Account accB("03150010", "Frank", "111111", "saltF", 2000.0, 0);

	bank.addAccount(accA);
	bank.addAccount(accB);

	EXPECT_FALSE(bank.transfer("03150009", "03150010", 1000, "wrongPin"));

	EXPECT_DOUBLE_EQ(bank.findAccount("03150009")->getBalance(), 3000.0);
	EXPECT_DOUBLE_EQ(bank.findAccount("03150010")->getBalance(), 2000.0);
}