#pragma once
#include <iostream>
#include <limits>
#include <string>
#include "BankingSystem.h"

class Menu {
public:
	static void mainMenu(BankingSystem& bank);
	static void printHeader(const string& title);
	static void pauseScreen();
};