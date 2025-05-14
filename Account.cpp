#include "Account.h"
#include "Deposit.h"
#include "Withdrawal.h"
#include "Transaction.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <cctype>
#include <algorithm>
#include <memory>
#include <iomanip>

namespace fs = std::filesystem;

Account::Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento)
: iban(iban), intestatario(intestatario), fileRiferimento(fileRiferimento), saldo(0) {}

Account::~Account() {}

void Account::addTransaction(std::unique_ptr<Transaction> transaction, const std::string& filePath) {
    transaction->apply(*this, filePath);
    transazioni.push_back(transaction->clone());
    saveToAccountFile();
}

void Account::updateSaldo(double amount) {
    saldo += amount;
}

void Account::saveToAccountFile() const {
    std::ofstream file(fileRiferimento, std::ios::trunc);
    if (file.is_open()) {
        file << iban << "\n"
             << intestatario.getNome() << "\n"
             << intestatario.getCognome() << "\n"
             << intestatario.getCodicefiscale() << "\n"
             << saldo << "\n";
        file.close();
    } else {
        throw std::runtime_error("Errore nell'apertura del file per salvare l'account");
    }
}

Account Account::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string iban, nome, cognome, codicefiscale;
    double saldo;

    if (!file.is_open()) {
        throw std::runtime_error("Errore: impossibile aprire il file dell'account: " + filePath);
    }

    std::getline(file, iban);
    std::getline(file, nome);
    std::getline(file, cognome);
    std::getline(file, codicefiscale);
    file >> saldo;
    file.ignore();
    file.close();

    Persona persona(nome, cognome, codicefiscale);
    Account account(iban, persona, filePath);
    account.updateSaldo(saldo);

    std::ifstream transFile(TRANSACTIONS_PATH);
    if (!transFile.is_open()) {
        throw std::runtime_error("Errore: impossibile aprire il file delle transazioni.");
    }

    std::string line;
    while (std::getline(transFile, line)) {
        std::istringstream iss(line);
        std::string transIban, transType, amountStr, timeStr, desc, lastModStr;

        std::getline(iss, transIban, ',');
        std::getline(iss, transType, ',');
        std::getline(iss, amountStr, ',');
        std::getline(iss, timeStr, ',');
        std::getline(iss, desc, ',');
        std::getline(iss, lastModStr);

        if (transIban == iban) {
            double amount = std::stod(amountStr);
            std::time_t timestamp = 0;
            std::time_t lastModified = 0;

            std::istringstream ssTime(timeStr);
            std::istringstream ssMod(lastModStr);
            std::tm tm1 = {}, tm2 = {};
            if (ssTime >> std::get_time(&tm1, "%Y-%m-%d %H:%M:%S")) {
                timestamp = std::mktime(&tm1);
            }
            if (ssMod >> std::get_time(&tm2, "%Y-%m-%d %H:%M:%S")) {
                lastModified = std::mktime(&tm2);
            }

            if (transType == "Deposit") {
                account.transazioni.push_back(std::make_unique<Deposit>(amount, desc, timestamp, lastModified, iban));
            } else if (transType == "Withdrawal") {
                account.transazioni.push_back(std::make_unique<Withdrawal>(amount, desc, timestamp, lastModified, iban));
            }
        }
    }
    transFile.close();

    return account;
}

std::string Account::getIban() const { return iban; }
std::string Account::getNome() const { return intestatario.getNome(); }
std::string Account::getCognome() const { return intestatario.getCognome(); }
std::string Account::getCodicefiscale() const { return intestatario.getCodicefiscale(); }
std::string Account::getFileRiferimento() const { return fileRiferimento; }
double Account::getSaldo() const { return saldo; }

const std::vector<std::unique_ptr<Transaction>>& Account::getTransazioni() const {
    return transazioni;
}

const Transaction* Account::getTransactionByIndex(int index) const {
    if (index >= 0 && index < static_cast<int>(transazioni.size())) {
        return transazioni[index].get();
    }
    return nullptr;
}

Transaction* Account::operator[](size_t index) {
    if (index < transazioni.size()) {
        return transazioni[index].get();
    }
    return nullptr;
}

const Transaction* Account::operator[](size_t index) const {
    if (index < transazioni.size()) {
        return transazioni[index].get();
    }
    return nullptr;
}

std::vector<const Transaction*> Account::searchTransaction(const std::string& query) const {
    std::vector<const Transaction*> results;
    std::string q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);

    for (const auto& t : transazioni) {
        std::string time = t->formatTime(t->getTime());
        std::string desc = t->getDescription();
        std::transform(time.begin(), time.end(), time.begin(), ::tolower);
        std::transform(desc.begin(), desc.end(), desc.begin(), ::tolower);

        if (time.find(q) != std::string::npos || desc.find(q) != std::string::npos) {
            results.push_back(t.get());
        }
    }
    return results;
}

std::vector<const Transaction*> Account::searchTransaction(const std::string& query, const std::string& filePath) const {
    return searchTransaction(query); // filePath ignorato, compatibilità overload
}


bool Account::modifyTransaction(int index, const std::string& nuovaDescrizione, const std::string& filePath) {
    if (index < 0 || index >= static_cast<int>(transazioni.size())) return false;
    return transazioni[index]->modifyDescription(nuovaDescrizione, filePath);
}

bool Account::modifyTransaction(const std::string& query, int matchIndex, const std::string& nuovaDescrizione, const std::string& filePath) {
    auto results = searchTransaction(query);
    if (matchIndex < 0 || matchIndex >= static_cast<int>(results.size())) return false;

    const Transaction* target = results[matchIndex];
    for (auto& t : transazioni) {
        if (t.get() == target) {
            return t->modifyDescription(nuovaDescrizione, filePath);
        }
    }
    return false;
}


bool Account::deleteTransaction(int index, const std::string& filePath) {
    if (index < 0 || index >= static_cast<int>(transazioni.size())) return false;

    std::string type = transazioni[index]->getType();
    double amount = transazioni[index]->getAmount();

    if (type == "Deposit") saldo -= amount;
    else if (type == "Withdrawal") saldo += amount;

    transazioni.erase(transazioni.begin() + index);
    saveToAccountFile();

    // riscrive tutto il log
    std::ofstream outFile(filePath, std::ios::trunc);
    if (!outFile.is_open()) throw std::runtime_error("Errore scrittura file transazioni.");

    for (const auto& t : transazioni) {
        t->saveToLogTransaction(filePath, iban);
    }

    return true;
}

bool Account::deleteTransaction(const std::string& query, int matchIndex, const std::string& filePath) {
    auto results = searchTransaction(query);
    if (matchIndex < 0 || matchIndex >= static_cast<int>(results.size())) return false;

    const Transaction* target = results[matchIndex];
    for (size_t i = 0; i < transazioni.size(); ++i) {
        if (transazioni[i].get() == target) {
            return deleteTransaction(static_cast<int>(i), filePath);
        }
    }
    return false;
}

void Account::printTransactions() const {
    if (transazioni.empty()) {
        std::cout << "Nessuna transazione trovata.\n";
        return;
    }

    for (size_t i = 0; i < transazioni.size(); ++i) {
        const auto& t = transazioni[i];
        std::cout << i << ". " << t->getIban() << ", " << t->getType() << ", "
                  << t->getAmount() << ", " << t->formatTime(t->getTime()) << ", "
                  << t->getDescription() << ", " << t->formatTime(t->getLastModified()) << "\n";
    }
}
