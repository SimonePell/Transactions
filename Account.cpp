#include "Account.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

Account::Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento)
    : iban(iban), intestatario(intestatario), fileRiferimento(fileRiferimento), saldo(0) {
    std::string directory = "TRANSACTION/Accounts/";
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }
    createFile();
}

void Account::addTransaction(Transaction* transaction) {
    transazioni.emplace_back(std::unique_ptr<Transaction>(transaction));
    transaction->apply(*this);
    saveToFile();
}

void Account::updateSaldo(double amount) {
    saldo += amount;
}

void Account::saveToFile() const {
    std::ofstream file(fileRiferimento, std::ios::app);
    if (file.is_open()) {
        for (const auto& transaction : transazioni) {
            transaction->save(file, saldo);
        }
    }
    file.close();
}

void Account::createFile() const {
    std::ofstream file(fileRiferimento);
    if (file.is_open()) {
        file << "Type,Amount,Current Balance\n";  // CSV Header
    }
    file.close();
}

double Account::getSaldo() const {
    return saldo;
}

std::string Account::getIban() const {
    return iban;
}

std::string Account::getNome() const {
    return intestatario.getNome();
}

std::string Account::getCognome() const {
    return intestatario.getCognome();
}

std::string Account::getCodicefiscale() const {
    return intestatario.getCodicefiscale();
}
