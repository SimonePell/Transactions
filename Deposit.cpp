#include "Deposit.h"
#include "Account.h"
#include <fstream>
#include <iostream>

using namespace std;

Deposit::Deposit(double amount, std::string description): Transaction(amount, std::move(description)) {}

//aggiorna il saldo e salva la transazione
void Deposit::apply(Account& account) const {
    account.updateSaldo(amount);
    save(account.getFileRiferimento(), account.getSaldo());
}

//funzione che scrive la transazione di deposito nel file apposito creando una nuova riga 
void Deposit::save(const std::string& filePath, double currentBalance) const {
    ofstream outFile(filePath, ios::app); //apertura in modalità append
    if (outFile.is_open()) {
        outFile << getType() << "," << getAmount() << "," << currentBalance << endl;
        outFile.close();
    } else {
        throw runtime_error("Errore nell'apertura del file per salvare il deposito");
    }
}

void Deposit::logTransaction(const std::string& filePath, const std::string& iban) const {
    ofstream outFile(filePath, ios::app);  // Apre il file in modalità append
    if (outFile.is_open()) {
        // Ottieni il timestamp della transazione dal metodo getTime()
        std::time_t transactionTime = getTime();
        char buffer[20];
        struct tm* timeinfo;
        timeinfo = localtime(&transactionTime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

        // Scrivi la transazione nel file
        outFile << iban << "," << getType() << "," << getAmount() << "," << buffer << "," << getDescription() << endl;
        outFile.close();
    } else {
        throw runtime_error("Errore: impossibile aprire il file " + filePath + " per salvare la transazione.");
    }
}
