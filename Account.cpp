#include "Account.h"
#include "Deposit.h"
#include "Withdrawal.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

Account::Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento)
    : iban(iban), intestatario(intestatario), fileRiferimento(fileRiferimento), saldo(0) {}

void Account::addTransaction(Transaction* transaction) {
    transaction->apply(*this); 
    saveToFile();
}

void Account::updateSaldo(double amount) {
    saldo += amount;
}

void Account::saveToFile() const {
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
    
    if (file.is_open()) {
        std::getline(file, iban);
        std::getline(file, nome);
        std::getline(file, cognome);
        std::getline(file, codicefiscale);
        file >> saldo;
        file.ignore();

        Persona persona(nome, cognome, codicefiscale);
        Account account(iban, persona, filePath);
        account.updateSaldo(saldo);

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            double amount;
            std::getline(ss, type, ',');
            ss >> amount;
        }

        file.close();
        return account;
    } else {
        throw std::runtime_error("Errore durante la lettura del file");
    }
}

double Account::getSaldo() const { return saldo; }
std::string Account::getIban() const { return iban; }
std::string Account::getNome() const { return intestatario.getNome(); }
std::string Account::getCognome() const { return intestatario.getCognome(); }
std::string Account::getCodicefiscale() const { return intestatario.getCodicefiscale(); }
std::string Account::getFileRiferimento() const { return fileRiferimento; }
