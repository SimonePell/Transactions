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






void Account::deleteTransaction() {
    printTransactions();
    
    std::cout << "Inserisci l'indice della transazione da eliminare: ";
    int index;
    std::cin >> index;
    
    std::ifstream file(TRANSACTIONS_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni.");
    }
    
    std::vector<std::string> lines;
    std::string line;
    int currentIndex = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string transIban;
        std::getline(iss, transIban, ',');
        if (!(transIban == iban && currentIndex == index)) {
            lines.push_back(line);
        }
        currentIndex++;
    }
    file.close();
    
    std::ofstream outFile(TRANSACTIONS_PATH, std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file per la scrittura.");
    }
    for (const std::string& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();
    
    std::cout << "Transazione eliminata con successo.\n";
}


void Account::deleteTransactionFromLog(const std::string& filePath, Transaction* t) {
    //elimina la transazione dal file delle transazioni
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        throw std::runtime_error("Errore: impossibile aprire il file " + filePath);
    }

    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (std::getline(inFile, line)) {
        std::istringstream ss(line);
        std::string logIban, logType, amountStr, logTimeStr, logDesc, lastModStr;

        std::getline(ss, logIban, ',');
        std::getline(ss, logType, ',');
        std::getline(ss, amountStr, ',');
        std::getline(ss, logTimeStr, ',');
        std::getline(ss, logDesc, ',');
        std::getline(ss, lastModStr);

        if (logIban == t->getIban() && logType == t->getType() && logDesc == t->getDescription()) {
            found = true;
        } else {
            lines.push_back(line);
        }
    }
    inFile.close();

    if (!found) {
        throw std::runtime_error("Errore: transazione non trovata nel file.");
    }

    std::ofstream outFile(filePath, std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Errore: impossibile aprire il file " + filePath);
    }

    for (const std::string& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();
}


void Account::printTransactions() const {
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
