#include "pch.h" // VS 生成预编译头
#include "../Account.h"  // 引入被测试的类
#pragma execution_character_set("utf-8")

// 账户相关
// 1. 测试创建账户成功
TEST(AccountTest, CreateAccountSuccess) {
	Account acc("03150004", "Amanda", "444444", "randomSalt", 1000.0, 0);
	EXPECT_EQ(acc.getAccountHolder(), "Amanda");
	EXPECT_DOUBLE_EQ(acc.getBalance(), 1000.0);
}

// 2. 测试 PIN 验证
TEST(AccountTest, PinVerificationWorks) {
	Account acc("03150003", "Sherry", "333333", "", 500.0, 0);
	acc.setPin("333333");
	EXPECT_TRUE(acc.verifyPin("333333")); // 正确 PIN
	EXPECT_FALSE(acc.verifyPin("444444")); // 错误 PIN
}

// 3. 测试 PIN 连续错误尝试锁定账户
TEST(AccountTest, PinLockedAfter3Fails) {
	Account acc("03150005", "Kelin", "555555", "randomSalt", 500.0, 0);
	EXPECT_FALSE(acc.verifyPin("000000"));
	EXPECT_FALSE(acc.verifyPin("111111"));
	EXPECT_FALSE(acc.verifyPin("222222"));
	EXPECT_EQ(acc.getStatus(), AccountStatus::FROZEN);
}


// 存款相关
// 4. 测试账户存款功能
TEST(AccountTest, DepositIncreaseBalance) {
	Account acc("03150001", "Neo", "111111", "", 1000.0, 0);
	acc.setPin("111111");
	acc.deposit(500.0);
	EXPECT_DOUBLE_EQ(acc.getBalance(), 1500.0);
}

// 5. 测试存款非法金额失败
TEST(AccountTest, DepositInvalidAmount) {
	Account acc("03150001", "Neo", "111111", "", 1000.0, 0);
	acc.setPin("111111");
	EXPECT_FALSE(acc.deposit(0));
	EXPECT_FALSE(acc.deposit(-100));
	EXPECT_DOUBLE_EQ(acc.getBalance(), 1000.0); // 余额不变
}

// 6. 测试取款功能
TEST(AccountTest, WithdrawDecreaseBalance) {
	Account acc("03150002", "Joucy", "222222", "", 2000.0, 0);
	acc.setPin("222222");
	bool success = acc.withdraw(500.0);
	EXPECT_TRUE(success);
	EXPECT_DOUBLE_EQ(acc.getBalance(), 1500.0);
}


// 7. 测试账户取款余额不足
TEST(AccountTest, WithdrawFailsIfInsufficientBalance) {
	Account acc("03150001", "Neo", "111111", "", 300.0, 0);
	acc.setPin("111111");
	bool success = acc.withdraw(500.0);
	EXPECT_FALSE(success);
	EXPECT_DOUBLE_EQ(acc.getBalance(), 300.0);
}

// 8. 测试取款非法金额失败
TEST(AccountTest, WithdrawFailsIfInvalidAmount) {
	Account acc("03150001", "Neo", "111111", "", 1000.0, 0);
	acc.setPin("111111");
	EXPECT_FALSE(acc.withdraw(0));
	EXPECT_FALSE(acc.withdraw(-200));
	EXPECT_DOUBLE_EQ(acc.getBalance(), 1000.0); // 余额不变
}

