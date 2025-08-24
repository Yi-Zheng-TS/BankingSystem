#include "AuditLogger.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <mutex>

using namespace std;

static string g_audit_file;
static once_flag g_header_once;

string& AuditLogger::file() {
	return g_audit_file;
}

void AuditLogger::init(const string& path) {
	g_audit_file = path;
	ensureHeader();
}

void AuditLogger::ensureHeader() {
	call_once(g_header_once, [] {
		ifstream in(g_audit_file);
		if (in.good() && in.peek() != ifstream::traits_type::eof()) return;
		ofstream out(g_audit_file, ios::app);
		out << "# Timestamp,Event,Account,Peer,Amount,Success,Reason\n";
		});
}

string AuditLogger::nowTs() {
	time_t t = time(nullptr);
	return to_string((long long)t);
}

// 登录审计
void AuditLogger::login(const string& account, bool success, const string& reason) {
	ensureHeader();
	ofstream out(file(), ios::app);
	out << nowTs() << ",LOGIN_" << (success ? "OK" : "FAIL") << ","
		<< account << ",," << 0 << "," << (success ? 1 : 0) << "," << reason << "\n";
}

// 交易审计
void AuditLogger::txn(const string& fromAcc, const string& toAcc, double amount, bool success, const string& reason) {
	ensureHeader();
	ofstream out(file(), ios::app);
	out << nowTs() << ",TXN_" << (success ? "OK" : "FAIL") << ","
		<< fromAcc << "," << toAcc << "," << amount << "," << (success ? 1 : 0) << "," << reason << "\n";
}