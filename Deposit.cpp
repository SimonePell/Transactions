#include "Deposit.h"
#include "Account.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;


//costruttore nuova transazione
Deposit::Deposit(double amount, std::string description, std::string iban)
    : Transaction(amount, std::move(description), std::move(iban)) {}

//cotruttore per caricare una transazione da un file
Deposit::Deposit(double amount, std::string description, std::time_t timeStamp, std::time_t lastMod, std::string iban)
    : Transaction(amount, std::move(description), timeStamp, lastMod, std::move(iban)) {}


void Deposit::apply(Account& account, const std::string& filePath) const {
    account.updateSaldo(amount);
    saveToLogTransaction(filePath, account.getIban());
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

bool Deposit::modifyDescription(const std::string& newDescription, const std::string& filePath) {
    description = newDescription;
    lastModified = std::time(nullptr);  //aggiorna il timestamp dell'ultima modifica
    try {
        updateLogTransaction(filePath, *this);
        return true; 
    } catch (const std::exception& e) {
        return false; 
    } 
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

        //controlla se la transazione deve essere aggiornata
        if (logIban == updatedTransaction.getIban() && 
            logType == updatedTransaction.getType() && 
            std::stod(amountStr) == updatedTransaction.getAmount()) {
            
            //genera la nuova data dell'ultima modifica
            char bufferLastMod[20];
            struct tm* timeinfo;
            std::time_t lastModTime = updatedTransaction.getLastModified();
            timeinfo = localtime(&lastModTime);
            strftime(bufferLastMod, sizeof(bufferLastMod), "%Y-%m-%d %H:%M:%S", timeinfo);

            //crea la nuova riga aggiornata
            stringstream newLine;
            newLine << logIban << "," 
                    << logType << "," 
                    << updatedTransaction.getAmount() << "," 
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

    //scrive le nuove righe aggiornate nel file
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