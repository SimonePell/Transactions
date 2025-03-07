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

// applica la transazione e salva lo stato aggiornato dell'account nel file
void Account::addTransaction(Transaction* transaction) {
    transaction->apply(*this); 
    saveToFile();
}

void Account::updateSaldo(double amount) {
    saldo += amount;
}

void Account::saveToFile() const {
    std::ofstream file(fileRiferimento, std::ios::trunc); //apertura file in modalità scrittura
    if (file.is_open()) {
        file << iban << "\n"
             << intestatario.getNome() << "\n"
             << intestatario.getCognome() << "\n"
             << intestatario.getCodicefiscale() << "\n"
             << saldo << "\n";
        file.close();//chiude il file dopo aver scritto i dati nel file
    } else {
        throw std::runtime_error("Errore nell'apertura del file per salvare l'account");
    }
}

//restituisce un oggetto Account costruito con i dati letti dal file 
Account Account::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath); //apertura file in modalità lettura
    std::string iban, nome, cognome, codicefiscale;
    double saldo;
    
    if (file.is_open()) {
        std::getline(file, iban);
        std::getline(file, nome);
        std::getline(file, cognome);
        std::getline(file, codicefiscale);
        file >> saldo;
        file.ignore();//pulisce il buffer

        Persona persona(nome, cognome, codicefiscale);
        Account account(iban, persona, filePath);
        account.updateSaldo(saldo);

        file.close();//chiusura file
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
