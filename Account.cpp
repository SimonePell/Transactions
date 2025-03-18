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


void Account::searchTransaction(const std::string& query) const {
    if (!hasTransactions()) {
        std::cout << "Errore: Nessuna transazione trovata per questo account.\n";
        return;
    }

    std::ifstream file(TRANSACTIONS_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni.");
    }

    std::string line;
    bool found = false;
    int index = 0;

    // Convert query to lowercase for case-insensitive comparison
    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

    std::cout << "Risultati della ricerca per \"" << query << "\":\n";

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string transIban, transType, amountStr, logTimeStr, logDesc, lastModStr;

        std::getline(iss, transIban, ',');
        std::getline(iss, transType, ',');
        std::getline(iss, amountStr, ',');
        std::getline(iss, logTimeStr, ',');
        std::getline(iss, logDesc, ',');
        std::getline(iss, lastModStr);

        // Check if transaction belongs to this account
        if (transIban == iban) {
            // Convert date and description to lowercase for comparison
            std::string logTimeLower = logTimeStr;
            std::string logDescLower = logDesc;
            std::transform(logTimeLower.begin(), logTimeLower.end(), logTimeLower.begin(), ::tolower);
            std::transform(logDescLower.begin(), logDescLower.end(), logDescLower.begin(), ::tolower);

            // Perform case-insensitive partial matching
            if (logTimeLower.find(queryLower) != std::string::npos || logDescLower.find(queryLower) != std::string::npos) {
                std::cout << index++ << ". " << line << "\n";
                found = true;
            }
        }
    }

    file.close();

    if (!found) {
        std::cout << "Nessuna transazione trovata con il criterio: " << query << "\n";
    }
}


void Account::modifyTransactionByIndex(int index) {
    if (!hasTransactions()) {
        std::cout << "Errore: Nessuna transazione disponibile per la modifica.\n";
        return;
    }

    std::ifstream file(TRANSACTIONS_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni.");
    }

    std::vector<std::string> lines;
    std::string line;
    int currentIndex = 0;
    bool found = false;

    std::string newDescription;
    std::time_t now = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::string lastModTime = buffer;

    std::cout << "Inserisci la nuova descrizione: ";
    std::cin.ignore();
    std::getline(std::cin, newDescription);

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
            line = transIban + "," + transType + "," + amountStr + "," + logTimeStr + "," + newDescription + "," + lastModTime;
        }
        lines.push_back(line);
        currentIndex++;
    }
    file.close();

    if (!found) {
        std::cout << "Errore: Indice non valido.\n";
        return;
    }

    std::ofstream outFile(TRANSACTIONS_PATH, std::ios::trunc);
    if (!outFile.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file per la scrittura.");
    }
    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    std::cout << "Descrizione modificata con successo!\n";
}
void Account::modifyTransactionBySearch(const std::string& query) {
    if (!hasTransactions()) {
        std::cout << "Errore: Nessuna transazione disponibile per la modifica.\n";
        return;
    }

    std::ifstream file(TRANSACTIONS_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni.");
    }

    std::vector<std::string> matchingTransactions;
    std::vector<int> matchingIndexes;
    std::string line;
    int originalIndex = 0;
    int displayIndex = 0;

    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

    std::cout << "Risultati della ricerca per \"" << query << "\":\n";

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string transIban, transType, amountStr, logTimeStr, logDesc, lastModStr;

        std::getline(iss, transIban, ',');
        std::getline(iss, transType, ',');
        std::getline(iss, amountStr, ',');
        std::getline(iss, logTimeStr, ',');
        std::getline(iss, logDesc, ',');
        std::getline(iss, lastModStr);

        if (transIban == iban) {
            std::string logTimeLower = logTimeStr;
            std::string logDescLower = logDesc;
            std::transform(logTimeLower.begin(), logTimeLower.end(), logTimeLower.begin(), ::tolower);
            std::transform(logDescLower.begin(), logDescLower.end(), logDescLower.begin(), ::tolower);

            if (logTimeLower.find(queryLower) != std::string::npos || logDescLower.find(queryLower) != std::string::npos) {
                std::cout << displayIndex << ". " << line << "\n";
                matchingTransactions.push_back(line);
                matchingIndexes.push_back(originalIndex);
                displayIndex++;
            }
        }
        originalIndex++;
    }
    file.close();

    if (matchingTransactions.empty()) {
        std::cout << "Nessuna transazione trovata con il criterio: " << query << "\n";
        return;
    }

    int selectedIndex;
    std::cout << "Inserisci l'indice della transazione da modificare: ";
    while (true) {
        std::cin >> selectedIndex;
        if (std::cin.fail() || selectedIndex < 0 || selectedIndex >= (int)matchingIndexes.size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Errore: Inserisci un numero valido tra 0 e " << matchingIndexes.size() - 1 << ": ";
        } else {
            break;
        }
    }

    // Call modifyTransactionByIndex using the real file index
    modifyTransactionByIndex(matchingIndexes[selectedIndex]);
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
void Account::deleteTransactionBySearch(const std::string& query) {
    if (!hasTransactions()) {
        std::cout << "Errore: Nessuna transazione disponibile per l'eliminazione.\n";
        return;
    }

    std::ifstream file(TRANSACTIONS_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Errore nell'apertura del file delle transazioni.");
    }

    std::vector<std::string> matchingTransactions;
    std::vector<int> matchingIndexes;
    std::string line;
    int originalIndex = 0;
    int displayIndex = 0;

    // Convert query to lowercase for case-insensitive comparison
    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

    std::cout << "Risultati della ricerca per \"" << query << "\":\n";

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string transIban, transType, amountStr, logTimeStr, logDesc, lastModStr;

        std::getline(iss, transIban, ',');
        std::getline(iss, transType, ',');
        std::getline(iss, amountStr, ',');
        std::getline(iss, logTimeStr, ',');
        std::getline(iss, logDesc, ',');
        std::getline(iss, lastModStr);

        // Check if transaction belongs to this account
        if (transIban == iban) {
            // Convert date and description to lowercase for comparison
            std::string logTimeLower = logTimeStr;
            std::string logDescLower = logDesc;
            std::transform(logTimeLower.begin(), logTimeLower.end(), logTimeLower.begin(), ::tolower);
            std::transform(logDescLower.begin(), logDescLower.end(), logDescLower.begin(), ::tolower);

            // Perform case-insensitive partial matching
            if (logTimeLower.find(queryLower) != std::string::npos || logDescLower.find(queryLower) != std::string::npos) {
                std::cout << displayIndex << ". " << line << "\n";
                matchingTransactions.push_back(line);
                matchingIndexes.push_back(originalIndex);
                displayIndex++;
            }
        }
        originalIndex++; // Increment the real index in the file
    }

    file.close();

    if (matchingTransactions.empty()) {
        std::cout << "Nessuna transazione trovata con il criterio: " << query << "\n";
        return;
    }

    // Ask the user for the index to delete
    int selectedIndex;
    std::cout << "Inserisci l'indice della transazione da eliminare: ";
    while (true) {
        std::cin >> selectedIndex;
        if (std::cin.fail() || selectedIndex < 0 || selectedIndex >= (int)matchingIndexes.size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Errore: Inserisci un numero valido tra 0 e " << matchingIndexes.size() - 1 << ": ";
        } else {
            break;
        }
    }

    // Delete the transaction using its actual index in the file
    deleteTransactionByIndex(matchingIndexes[selectedIndex]);
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
