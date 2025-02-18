#include "Account.h"
#include <fstream>
#include <sstream>
using namespace std;

Account::Account(const string& iban, const Persona& intestatario, const string& fileRiferimento)
    : iban(iban), intestatario(intestatario), fileRiferimento(fileRiferimento), saldo(0) {
    createFile;
}

void Account::addTransaction(Transaction* transaction) {
    transazioni.emplace_back(transaction);
    transaction->apply(*this);
    saveToFile();
}

void Account::updateSaldo(double amount) {
    saldo += amount;
}

void Account::saveToFile() const {
    std::ofstream file(fileRiferimento, ios::app);
    if (file.is_open()) {
        for (const auto& transaction : transazioni) {
            transaction->save(file, saldo);
        }
    }
    file.close();
}

void Account::createFile() const {
    ofstream file(fileRiferimento);
    if (file.is_open()) {
        file << "Type,Amount,Current Balance\n";  // CSV Header
    }
    file.close();
}
