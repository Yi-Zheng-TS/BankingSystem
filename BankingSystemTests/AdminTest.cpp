#include "pch.h" // VS 生成预编译头
#include "../Account.h"  // 引入被测试的类
#include "../Admin.h" // 引入被测试的类
#pragma execution_character_set("utf-8")

// 管理员操作
// 13. 测试管理员冻结账户
TEST(AdminTest, FreezeAccount) {
    Account acc("03150011", "Nick", "222222", "salt", 1000.0, 0);
    acc.setStatus(AccountStatus::FROZEN);
    EXPECT_EQ(acc.getStatus(), AccountStatus::FROZEN);
}

// 14. 测试管理员解冻账户
TEST(AdminTest, UnfreezeAccount) {
    Account acc("03150019", "Olivia", "1234", "salt", 1000.0, 0);
    acc.setStatus(AccountStatus::ACTIVE);
    EXPECT_EQ(acc.getStatus(), AccountStatus::ACTIVE);
}

// 15. 测试管理员关闭账户
TEST(AdminTest, CloseAccount) {
    Account acc("03150020", "Paul", "1234", "salt", 1000.0, 0);
    acc.setStatus(AccountStatus::CLOSED);
    EXPECT_EQ(acc.getStatus(), AccountStatus::CLOSED);
}