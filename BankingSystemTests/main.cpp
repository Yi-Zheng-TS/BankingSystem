#include <gtest/gtest.h>
#pragma execution_character_set("utf-8")

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}