#include "pch.h" // VS 生成预编译头
#include "../Account.h"  // 引入被测试的类
#pragma execution_character_set("utf-8")

// 测试账户存款功能
TEST(AccountTest, DepositIncreaseBalance) {
	Account acc("03150001", "Neo", "111111", "", 1000.0, 0);
	acc.setPin("111111");
	acc.deposit(500.0);
	EXPECT_DOUBLE_EQ(acc.getBalance(), 1500.0);
}

// 测试取款功能
TEST(AccountTest, WithdrawDecreaseBalance) {
	Account acc("03150002", "Joucy", "222222", "", 2000.0, 0);
	acc.setPin("222222");
	bool success = acc.withdraw(500.0);
	EXPECT_TRUE(success);
	EXPECT_DOUBLE_EQ(acc.getBalance(), 1500.0);
}


// 测试账户取款余额不足
TEST(AccountTest, WithdrawFailsIfInsufficientBalance) {
	Account acc("03150001", "Neo", "111111", "", 300.0, 0);
	acc.setPin("111111");
	bool success = acc.withdraw(500.0);
	EXPECT_FALSE(success);
	EXPECT_DOUBLE_EQ(acc.getBalance(), 300.0);
}

// 测试 PIN 验证
TEST(AccountTest, PinVerificationWorks) {
	Account acc("03150003", "Sherry", "333333", "", 500.0, 0);
	acc.setPin("333333");
	EXPECT_TRUE(acc.verifyPin("333333")); // 正确 PIN
	EXPECT_FALSE(acc.verifyPin("444444")); // 错误 PIN
}