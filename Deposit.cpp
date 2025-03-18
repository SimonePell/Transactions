#include "Deposit.h"
#include "Account.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
const std::string TRANSACTIONS_PATH = "TRANSACTION/transazioni.txt"; 


//costruttore nuova transazione
Deposit::Deposit(double amount, std::string description, std::string iban)
    : Transaction(amount, std::move(description), std::move(iban)) {}

//cotruttore per caricare una transazione da un file
Deposit::Deposit(double amount, std::string description, std::time_t timeStamp, std::time_t lastMod, std::string iban)
    : Transaction(amount, std::move(description), timeStamp, lastMod, std::move(iban)) {}

//aggiorna il saldo con saveToAccountFile() e salva la transazione nel log con logTransaction()
void Deposit::apply(Account& account) const {
    account.updateSaldo(amount);
    saveToAccountFile(account.getFileRiferimento(), account.getSaldo());
    saveToLogTransaction(TRANSACTIONS_PATH, account.getIban());
}

//salva solo il saldo aggiornato nel file 
void Deposit::saveToAccountFile(const std::string& filePath, double currentBalance) const {
    ofstream outFile(filePath, ios::trunc);  // sovrascrive il file con il nuovo saldo
    if (outFile.is_open()) {
        outFile << currentBalance << endl;
        outFile.close();
    } else {
        throw runtime_error("Errore: impossibile aprire il file " + filePath + " per aggiornare il saldo.");
    }
}

//scrive nella prima riga disponibile la nuova transazione
void Deposit::saveToLogTransaction(const std::string& filePath, const std::string& iban) const {
    ofstream outFile(filePath, ios::app);  //apre il file in modalità append
    if (outFile.is_open()) {
        //formattazione della data della transazione
        char buffer[20], bufferLastMod[20];
        struct tm* timeinfo;
        
        std::time_t transactionTime = getTime();
        timeinfo = localtime(&transactionTime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);


        //scrivi la transazione con l'ultima modifica
        outFile << iban << "," << getType() << "," << getAmount() << "," << buffer << "," << getDescription() << "," << buffer << endl;
        outFile.close();
    } else {
        throw runtime_error("Errore: impossibile aprire il file " + filePath + " per salvare la transazione.");
    }
}

void Deposit::modifyDescription(const std::string& newDescription) {
    description = newDescription;
    lastModified = std::time(nullptr);  //aggiorna il timestamp dell'ultima modifica
    updateLogTransaction(TRANSACTIONS_PATH, *this);  
}

void Deposit::updateLogTransaction(const std::string& filePath, const Transaction& updatedTransaction) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        throw runtime_error("Errore: impossibile aprire il file " + filePath);
    }

    vector<string> lines;
    string line;
    bool updated = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string logIban, logType, amountStr, logTimeStr, logDesc, lastModStr;

        getline(ss, logIban, ',');
        getline(ss, logType, ',');
        getline(ss, amountStr, ',');
        getline(ss, logTimeStr, ',');
        getline(ss, logDesc, ',');
        getline(ss, lastModStr);

        // Controlla se la transazione deve essere aggiornata
        if (logIban == updatedTransaction.getIban() && 
            logTimeStr == to_string(updatedTransaction.getTime()) && 
            logType == updatedTransaction.getType() && 
            logDesc == updatedTransaction.getDescription() &&
            amountStr == to_string(updatedTransaction.getAmount())) {
            
            // Genera la nuova data dell'ultima modifica
            char bufferLastMod[20];
            struct tm* timeinfo;
            std::time_t lastModTime = updatedTransaction.getLastModified();
            timeinfo = localtime(&lastModTime);
            strftime(bufferLastMod, sizeof(bufferLastMod), "%Y-%m-%d %H:%M:%S", timeinfo);

            // Crea la nuova riga aggiornata
            stringstream newLine;
            newLine << logIban << "," 
                    << logType << "," 
                    << amountStr << "," 
                    << logTimeStr << "," 
                    << updatedTransaction.getDescription() << "," 
                    << bufferLastMod;

            lines.push_back(newLine.str());
            updated = true;
        } else {
            lines.push_back(line);
        }
    }
    inFile.close();

    // Scrivi le nuove righe aggiornate nel file
    ofstream outFile(filePath, ios::trunc);
    if (!outFile.is_open()) {
        throw runtime_error("Errore: impossibile aprire il file " + filePath);
    }

    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.close();

    if (!updated) {
        throw runtime_error("Errore: transazione non trovata nel file.");
    }
}
