# 🏦 Banking Management System

A C++ console-based **Banking Management System** built as a personal project to practice **software engineering fundamentals, secure coding, and system design**.  
The system simulates a real-world banking application with account management, transaction handling, and security features such as salted PIN hashing.

---

## ✨ Features

- **Account Management**
  - Create, freeze, unfreeze, and close accounts
  - Store account data persistently in local files (CSV-like format)

- **Authentication & Security**
  - PIN codes stored as **SHA-256 salted hashes**
  - Prevents rainbow table attacks
  - Admin operations protected by PIN verification
  - (Planned) Lock accounts after multiple failed login attempts

- **Transactions**
  - Deposit, withdraw, transfer between accounts
  - Automatic transaction recording with timestamp
  - Validation for account status and sufficient balance

- **Audit Logging**
  - Records all login attempts and transactions
  - Provides traceability for both successful and failed operations

---

## 🛠️ Technologies Used
- **C++17**
- **OpenSSL** (for secure hashing)
- Standard Template Library (STL): `vector`, `map`, `fstream`, `sstream`
- File-based persistence (accounts & transactions stored in `.txt` files)

---

## 📂 Project Structure
BankingSystem/
│── Account.h / Account.cpp # Account entity with hashed PIN + salt
│── BankingSystem.h / BankingSystem.cpp # Core banking logic
│── Transaction.h / Transaction.cpp # Transaction model
│── AuditLogger.h / AuditLogger.cpp # Security & logging
│── main.cpp # Entry point


🔒 Security Features

PIN hashing with SHA-256 + Salt

Audit logging of all activities

Planned improvements:

Limit PIN retry attempts

Encryption of stored files

📖 Learning Goals

This project helped me practice:

Secure data handling in C++

File persistence & parsing

Object-oriented design

Error handling & logging

Version control with Git & GitHub

📌 Future Improvements

Add unit tests (GoogleTest)

GUI version (Qt / ImGui)

Database integration (SQLite / PostgreSQL)

Networking for multi-user support
│── accounts.txt # Saved accounts
│── transactions.txt # Saved transactions
│── audit_log.txt # Audit log
