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

Account::~Account() {
    for (Transaction* t : transazioni) {
        delete t;
    }
    transazioni.clear();
}

// applica la transazione e salva lo stato aggiornato dell'account nel file
void Account::addTransaction(Transaction* transaction) {
    transaction->apply(*this);
    transazioni.push_back(transaction); // Manteniamo il puntatore nella lista
    saveToFile();
}

//TO_DO errore
void Account::updateSaldo(double amount) {
    saldo += amount;
}

//TO_DO errore
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

Transaction* Account::findTransactionByDescription(const std::string& desc) const {
    for (Transaction* t : transazioni) {
        if (t->getDescription() == desc) {
            return t;
        }
    }
    return nullptr;
}

std::vector<Transaction*> Account::findTransactionsByDate(const std::string& date) const {
    std::vector<Transaction*> results;
    for (Transaction* t : transazioni) {
        std::time_t time = t->getTime();
        char buffer[11];
        struct tm* timeinfo = localtime(&time);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);

        if (date == buffer) {
            results.push_back(t);
        }
    }
    return results;
}

//modifica solo l'importo di una transazione esistente
void Account::modifyTransactionAmount(const std::string& desc, double newAmount) {
    Transaction* t = findTransactionByDescription(desc);
    if (t) {
        double oldAmount = t->getAmount();  // Memorizza l'importo precedente
        t->setAmount(newAmount);  // Modifica l'importo
        updateSaldo(newAmount - oldAmount); // Aggiorna il saldo con la differenza
        saveToFile();
    } else {
        throw std::runtime_error("Transazione non trovata.");
    }
}

// Modifica solo la descrizione di una transazione
void Account::modifyTransactionDescription(const std::string& desc, const std::string& newDescription) {
    Transaction* t = findTransactionByDescription(desc);
    if (t) {
        t->setDescription(newDescription); // Modifica la descrizione
        saveToFile();
    } else {
        throw std::runtime_error("Transazione non trovata.");
    }
}

//elimina una transazione
void Account::deleteTransaction(const std::string& desc) {
    for (auto it = transazioni.begin(); it != transazioni.end(); ++it) {
        if ((*it)->getDescription() == desc) {
            updateSaldo(-(*it)->getAmount()); // Rimuove il saldo della transazione eliminata
            delete *it;
            transazioni.erase(it);
            saveToFile();
            return;
        }
    }
    throw std::runtime_error("Transazione non trovata.");
}

//stampa le transazioni
void Account::printTransactions() const {
    for (const Transaction* t : transazioni) {
        std::cout << "Tipo: " << t->getType() << " | Importo: " << t->getAmount() 
                  << " | Descrizione: " << t->getDescription() << "\n";
    }
}
