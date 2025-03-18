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

namespace fs = std::filesystem;
const std::string TRANSACTIONS_PATH = "TRANSACTION/transazioni.txt"; 

Account::Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento)
: iban(iban), intestatario(intestatario), fileRiferimento(fileRiferimento), saldo(0) 
{
std::cout << "DEBUG: Account created with IBAN " << iban << std::endl;
}


Account::~Account() {}


void Account::addTransaction(std::unique_ptr<Transaction> transaction) {
    transaction->apply(*this);
    std::ofstream file(TRANSACTIONS_PATH, std::ios::app);
    if (file.is_open()) {
        transaction->saveToLogTransaction(TRANSACTIONS_PATH, iban);
        file.close();
    } else {
        throw std::runtime_error("Errore: impossibile aprire il file per salvare la transazione.");
    }
    saveToAccountFile();
}


void Account::updateSaldo(double amount) {
    saldo += amount;
}


void Account::saveToAccountFile() const {
    //salva i dati dell'account nel file
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




Account Account::loadFromFile(const std::string& filePath) {
    //restituisce un oggetto Account costruito con i dati letti dal file 
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

        file.close();

        std::cout << "account caricato con successo: " << iban << " (Saldo: " << saldo << ")\n";
        return account; 
    } else {
        throw std::runtime_error("Errore: impossibile aprire il file dell'account: " + filePath);
    }
}


void Account::deleteTransactionByIndex(int index) {
    std::ifstream file(TRANSACTIONS_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni.");
    }

    std::vector<std::string> lines;
    std::string line;
    int currentIndex = 0;
    bool found = false;
    double transactionAmount = 0.0;
    std::string transactionType;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string transIban, transType, amountStr, logTimeStr, logDesc, lastModStr;

        std::getline(iss, transIban, ',');
        std::getline(iss, transType, ',');
        std::getline(iss, amountStr, ',');
        std::getline(iss, logTimeStr, ',');
        std::getline(iss, logDesc, ',');
        std::getline(iss, lastModStr);

        if (transIban == iban && currentIndex == index) {
            found = true;
            transactionAmount = std::stod(amountStr);
            transactionType = transType;
        } else {
            lines.push_back(line);
        }
        currentIndex++;
    }
    file.close();

    if (!found) {
        throw std::runtime_error("Errore: transazione non trovata.");
    }

    // Update the account balance
    if (transactionType == "Deposit") {
        saldo -= transactionAmount;  // Reverse deposit
    } else if (transactionType == "Withdrawal") {
        saldo += transactionAmount;  // Reverse withdrawal
    }

    // Save the updated balance to the account file
    saveToAccountFile();

    // Write back updated transactions
    std::ofstream outFile(TRANSACTIONS_PATH, std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file per la scrittura.");
    }
    for (const std::string& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    std::cout << "Transazione eliminata con successo e saldo aggiornato.\n";
}


void Account::printTransactions() const {
    if (!hasTransactions()) {
        std::cout << "Errore: Nessuna transazione trovata per questo account.\n";
        return;
    }

    std::ifstream file(TRANSACTIONS_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni.");
    }

    std::string line;
    int index = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string transIban;
        std::getline(iss, transIban, ',');
        if (transIban == iban) {
            std::cout << index++ << ". " << line << "\n";
        }
    }
    file.close();
}


double Account::getSaldo() const { return saldo; }
std::string Account::getIban() const { return iban; }
std::string Account::getNome() const { return intestatario.getNome(); }
std::string Account::getCognome() const { return intestatario.getCognome(); }
std::string Account::getCodicefiscale() const { return intestatario.getCodicefiscale(); }
std::string Account::getFileRiferimento() const { return fileRiferimento; }

bool Account::hasTransactions() const {
    std::ifstream file(TRANSACTIONS_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni.");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string transIban;
        std::getline(iss, transIban, ',');

        if (transIban == iban) {
            file.close();
            return true; // Found a transaction for this account
        }
    }

    file.close();
    return false; // No transactions found
}
