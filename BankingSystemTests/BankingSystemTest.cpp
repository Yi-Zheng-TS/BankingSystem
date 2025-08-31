#include "pch.h" // VS 生成预编译头
#include "../BankingSystem.h" // 引入被测试的类
#include"../SecurityUtils.h"
#pragma execution_character_set("utf-8")

using namespace std;

// 转账相关
// 9. 测试转账成功
TEST(BankingSystemTest, TransferSuccess) {
    BankingSystem bank;

    // 创建账户对象（外部对象只是用来传进去，后续不用再检查它们）
    Account accA("03150005", "Kelin", "555555", "salt", 5000.0, 0);
    Account accB("03150006", "Jowi", "666666", "salt", 1000.0, 0);

    // 添加账户到银行系统
    bank.addAccount(accA);
    bank.addAccount(accB);

    // 必须通过 findAccount 获取系统内部的账户副本
    Account* foundAccA = bank.findAccount("03150005");
    Account* foundAccB = bank.findAccount("03150006");

    ASSERT_NE(foundAccA, nullptr);
    ASSERT_NE(foundAccB, nullptr);

    // 初始余额
    EXPECT_DOUBLE_EQ(foundAccA->getBalance(), 5000.0);
    EXPECT_DOUBLE_EQ(foundAccB->getBalance(), 1000.0);

    // PIN 校验
    EXPECT_TRUE(foundAccA->verifyPin("555555"));
    EXPECT_TRUE(foundAccB->verifyPin("666666"));

    // 执行转账
    bool result = bank.transfer("03150005", "03150006", 2000, "555555");
    EXPECT_TRUE(result);

    // 转账后余额（必须用内部账户对象检查）
    EXPECT_DOUBLE_EQ(foundAccA->getBalance(), 3000.0);
    EXPECT_DOUBLE_EQ(foundAccB->getBalance(), 3000.0);

    //BankingSystem bank;

    //// 创建账户对象
    //string pin = "555555";
    //string salt = "";
    //string hashedPin = SecurityUtils::sha256(pin + salt);
    //Account accA("03150005", "Kelin", hashedPin, "salt", 500.0, 0);
    //Account accB("03150006", "Jowi", "666666", "salt", 100.0, 0);
    //cout << accA.getHashedPin() << endl;

    //// 添加前检查余额
    //cout << "Debug: Before adding, Account A balance: " << accA.getBalance() << endl;
    //cout << "Debug: Before adding, Account B balance: " << accB.getBalance() << endl;

    //// 添加账户到银行系统
    //bank.addAccount(accA);
    //bank.addAccount(accB);

    //// 添加后立即检查余额
    //Account* foundAccA = bank.findAccount("03150005");
    //Account* foundAccB = bank.findAccount("03150006");

    //if (foundAccA && foundAccB) {
    //    cout << "Debug: After adding, Account A balance: " << foundAccA->getBalance() << endl;
    //    cout << "Debug: After adding, Account B balance: " << foundAccB->getBalance() << endl;
    //    cout << "Debug: Account A status: " << static_cast<int>(foundAccA->getStatus()) << endl;
    //    cout << "Debug: Account B status: " << static_cast<int>(foundAccB->getStatus()) << endl;

    //    // 验证 PIN
    //    cout << "Debug: PIN verification for Account A: " << foundAccA->verifyPin("555555") << endl;
    //}
    //else {
    //    cout << "Debug: One or both accounts not found after adding!" << endl;
    //    if (!foundAccA) cout << "Debug: Account A not found" << endl;
    //    if (!foundAccB) cout << "Debug: Account B not found" << endl;
    //}

    //// 执行转账
    //bool result = bank.transfer("03150005", "03150006", 200, "555555");
    //cout << "Debug: Transfer result: " << result << endl;

    //// 检查转账后余额
    //Account* afterAccA = bank.findAccount("03150005");
    //Account* afterAccB = bank.findAccount("03150006");

    //if (afterAccA && afterAccB) {
    //    cout << "Debug: After transfer, Account A balance: " << afterAccA->getBalance() << endl;
    //    cout << "Debug: After transfer, Account B balance: " << afterAccB->getBalance() << endl;
    //}

    //EXPECT_TRUE(result);
    //if (afterAccA) EXPECT_DOUBLE_EQ(afterAccA->getBalance(), 300.0);
    //if (afterAccB) EXPECT_DOUBLE_EQ(afterAccB->getBalance(), 300.0);
}

// 10. 测试余额不足时转账失败
TEST(BankingSystemTest, TransferInsufficientBalance) {
    BankingSystem bank;

    Account accA("03150007", "Amanda", "777777", "saltA", 500.0, 0);
    Account accB("03150008", "Jingjing", "888888", "saltB", 1000.0, 0);

    bank.addAccount(accA);
    bank.addAccount(accB);

    bool result = bank.transfer("03150007", "03150008", 2000, "777777");


    EXPECT_FALSE(result);
    EXPECT_DOUBLE_EQ(accA.getBalance(), 500.0);
    EXPECT_DOUBLE_EQ(accB.getBalance(), 1000.0);
}

// 11. 测试 PIN 错误
TEST(BankingSystemTest, TransferFailsWithWrongPin) {
    BankingSystem bank;

    Account accA("03150009", "Eva", "999999", "saltE", 3000.0, 0);
    Account accB("03150010", "Frank", "111111", "saltF", 2000.0, 0);

    bank.addAccount(accA);
    bank.addAccount(accB);

    bool result = bank.transfer("03150009", "03150010", 1000, "wrongPin");

    EXPECT_FALSE(result);
    EXPECT_DOUBLE_EQ(accA.getBalance(), 3000.0);
    EXPECT_DOUBLE_EQ(accB.getBalance(), 2000.0);
}

// 12. 测试转账金额为 0 或负数
TEST(BankingSystemTest, TransferZeroOrNegativeAmount) {
    BankingSystem bank;

    Account accA("03150016", "Leo", "777777", "saltL", 3000.0, 0);
    Account accB("03150017", "Mia", "888888", "saltM", 2000.0, 0);

    bank.addAccount(accA);
    bank.addAccount(accB);

    // 转账金额为 0
    bool result1 = bank.transfer("03150016", "03150017", 0, "777777");

    // 转账金额为负数
    bool result2 = bank.transfer("03150016", "03150017", -100, "777777");

    EXPECT_FALSE(result1);
    EXPECT_FALSE(result2);
    EXPECT_DOUBLE_EQ(accA.getBalance(), 3000.0);
    EXPECT_DOUBLE_EQ(accB.getBalance(), 2000.0);
}