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

        return account; 
    } else {
        throw std::runtime_error("Errore durante la lettura del file dell'account.");
    }
}

double Account::getSaldo() const { return saldo; }
std::string Account::getIban() const { return iban; }
std::string Account::getNome() const { return intestatario.getNome(); }
std::string Account::getCognome() const { return intestatario.getCognome(); }
std::string Account::getCodicefiscale() const { return intestatario.getCodicefiscale(); }
std::string Account::getFileRiferimento() const { return fileRiferimento; }



void Account::getTransactions(const std::string& transactionsFilePath) {
    std::ifstream transFile(transactionsFilePath);
    if (!transFile.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni: " + transactionsFilePath);
    }

    std::string line;

    while (std::getline(transFile, line)) {
        std::istringstream iss(line);
        std::string transIban, type, amountStr, dateTimeStr, desc, lastModStr;
        double amount;

        //lettura dei campi separati
        std::getline(iss, transIban, ',');
        std::getline(iss, type, ',');
        std::getline(iss, amountStr, ',');
        std::getline(iss, dateTimeStr, ',');
        std::getline(iss, desc, ',');
        std::getline(iss, lastModStr, ',');

        try {
            amount = std::stod(amountStr);
        } catch (const std::invalid_argument&) {
            std::cerr << "Errore: impossibile convertire l'importo in numero: " << amountStr << std::endl;
            continue; // Salta questa transazione
        }

        // Conversione della data e ora 
        std::tm timeinfo = {};
        std::istringstream dateStream(dateTimeStr);
        dateStream >> std::get_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        std::time_t timeStamp = mktime(&timeinfo);

        if (timeStamp == -1) {
            std::cerr << "Errore: impossibile convertire la data e ora: " << dateTimeStr << std::endl;
            continue;
        }

        // Conversione della data e ora 
        std::tm lastModInfo = {};
        std::istringstream lastModStream(lastModStr);
        lastModStream >> std::get_time(&lastModInfo, "%Y-%m-%d %H:%M:%S");
        std::time_t lastModified = mktime(&lastModInfo);

        if (lastModified == -1) {
            std::cerr << "Errore: impossibile convertire la data dell'ultima modifica: " << lastModStr << std::endl;
            continue;
        }

        //carica solo le transazioni con IBAN corrispondente
        if (transIban == iban) {
            if (type == "Deposit") {
                addTransaction(new Deposit(amount, desc, timeStamp, lastModified, transIban));
            } else if (type == "Withdrawal") {
                addTransaction(new Withdrawal(amount, desc, timeStamp, lastModified, transIban));
            } else {
                std::cerr << "Errore: tipo di transazione sconosciuto: " << type << std::endl;
            }
        }
    }
    transFile.close();
}


//trova la transazione tramite la descrizione, è case-insesitive e utilizza la ricerca parziale
std::vector<Transaction*> Account::findTransactionsByDescription(const std::string& desc) const {
    std::vector<Transaction*> results;

    auto caseInsensitiveFind = [](const std::string& text, const std::string& pattern) {
        return std::search(text.begin(), text.end(), pattern.begin(), pattern.end(),
                           [](char c1, char c2) { return std::tolower(c1) == std::tolower(c2); }) != text.end();
    };

    for (Transaction* t : transazioni) {
        if (caseInsensitiveFind(t->getDescription(), desc)) {
            results.push_back(t);  // Aggiunge la transazione trovata al vettore dei risultati
        }
    }

    return results;
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

//elimina una transazione
void Account::deleteTransaction(int id) {
    if (id < 0 || id >= transazioni.size()) {
        std::cout << "Errore: Indice fuori dall'intervallo valido.\n";
        return;
    }

    Transaction* t = transazioni[id];
    updateSaldo(-t->getAmount());
    transazioni.erase(transazioni.begin() + id);
    saveToFile();
}

//stampa le transazioni
void Account::printTransactions() const {
    int index = 1;
    for (const Transaction* t : transazioni) {
        std::cout << "Tipo: " << t->getType() << " | Importo: " << t->getAmount() 
                  << " | Descrizione: " << t->getDescription() << "\n";
        index++;
    }
}

//restituisce la transazione in base all'indice
Transaction* Account::findTransactionByIndex(int index) const {
    if (index < 0 || index >= transazioni.size()) {
        std::cout << "Errore: Indice fuori dall'intervallo valido.\n";
        return nullptr; // Ritorna nullptr se l'indice è invalido
    }

    return transazioni[index]; // Ritorna il puntatore alla transazione richiesta
}

