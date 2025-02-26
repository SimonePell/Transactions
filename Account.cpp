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
}

void Account::updateSaldo(double amount) {
    saldo += amount;
}

void Account::saveToFile() const {
    std::ofstream file(fileRiferimento);
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

void Account::createFile() const {
    if (!fs::exists(fileRiferimento)) {
        std::ofstream file(fileRiferimento);
        if (file.is_open()) {
            file << iban << "\n"
                 << intestatario.getNome() << "\n"
                 << intestatario.getCognome() << "\n"
                 << intestatario.getCodicefiscale() << "\n"
                 << saldo << "\n";
            file.close();
        } else {
            throw std::runtime_error("Errore nella creazione del file");
        }
    }
}

double Account::getSaldo() const {return saldo;}

std::string Account::getIban() const { return iban;}

std::string Account::getNome() const { return intestatario.getNome();}

std::string Account::getCognome() const { return intestatario.getCognome();}

std::string Account::getCodicefiscale() const { return intestatario.getCodicefiscale();}

std::string Account::getFileRiferimento() const {return fileRiferimento;}

