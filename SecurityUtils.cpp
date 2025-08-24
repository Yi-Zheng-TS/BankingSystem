#include "SecurityUtils.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#pragma execution_character_set("utf-8")

using namespace std;

string SecurityUtils::sha256(const string& pin) {
	EVP_MD_CTX* context = EVP_MD_CTX_new();
	if (!context) {
		throw runtime_error("无法创建哈希上下文");
	}

	// 初始化 SHA-256 哈希操作
	if (1 != EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {
		EVP_MD_CTX_free(context);
		throw runtime_error("哈希初始化失败");
	}

	// 添加 PIN 数据到哈希计算
	if (1 != EVP_DigestUpdate(context, pin.c_str(), pin.size())) {
		EVP_MD_CTX_free(context);
		throw runtime_error("哈希更新失败");
	}

	// 获取哈希结果
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hashLength = 0;
	if (1 != EVP_DigestFinal_ex(context, hash, &hashLength)) {
		EVP_MD_CTX_free(context);
		throw runtime_error("哈希计算失败");
	}

	// 释放上下文资源
	EVP_MD_CTX_free(context);

	// 将二进制哈希转换为十六进制字符串
	ostringstream ss;
	for (unsigned int i = 0; i < hashLength; i++) {
		ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
	}

	return ss.str();
}

string SecurityUtils::randomSalt(size_t bytes) {
	// 创建一个字符创缓冲区 长度为bytes 初始内容为'\0'
	string buf(bytes, '\0');

	// 调用 OpenSSL 的 RAND_bytes 生成真随机字节 填充到 buf 里
	if (1 != RAND_bytes(reinterpret_cast<unsigned char*>(&buf[0]), (int)bytes)) {
		throw std::runtime_error("生成随机盐失败");
	}

	std::ostringstream ss;
	for (unsigned char c : buf) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
	}
	return ss.str(); // hex字符串
}