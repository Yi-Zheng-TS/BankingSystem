#include "Admin.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#pragma execution_character_set("utf-8")

using namespace std;

Admin::Admin(string id, string pin)
	: adminID(id), adminPin(pin), status(AdminStatus::ACTIVE) {
};

string Admin::getAdminID() const {
	return adminID;
}

string Admin::getAdminPin() const {
	return adminPin;
}

//AdminStatus Admin::getStatus() const {
//	return status;
//}

// ????
void Admin::setStatus(AdminStatus newStatus) {
	status = newStatus;
}