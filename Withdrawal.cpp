#include "Withdrawal.h"
#include "Account.h"
#include <fstream>
#include <iostream>

using namespace std;

//costruttore nuova transazione
Withdrawal::Withdrawal(double amount, std::string description): Transaction(amount, std::move(description)) {}

//cotruttore per caricare una transazione da un file
Withdrawal::Withdrawal(double amount, std::string description, std::time_t timeStamp): Transaction(amount, std::move(description), timeStamp) {}

//aggiorna il saldo con save() e salva la transazione nel log con logTransaction()
void Withdrawal::apply(Account& account) const {
    account.updateSaldo(-amount);
    save(account.getFileRiferimento(), account.getSaldo());
    logTransaction("transazioni.txt", account.getIban());
}

//salva solo il saldo aggiornato nel file 
void Withdrawal::save(const std::string& filePath, double currentBalance) const {
    ofstream outFile(filePath, ios::trunc);  // sovrascrive il file con il nuovo saldo
    if (outFile.is_open()) {
        outFile << currentBalance << endl;
        outFile.close();
    } else {
        throw runtime_error("Errore: impossibile aprire il file " + filePath + " per aggiornare il saldo.");
    }
}

//scrive la transazione nel logTransactions
void Withdrawal::logTransaction(const std::string& filePath, const std::string& iban) const {
    ofstream outFile(filePath, ios::app);  // apre il file in modalità append
    if (outFile.is_open()) {
        // ottieni data e tempo della transazione dal metodo getTime()
        std::time_t transactionTime = getTime();
        char buffer[20];
        struct tm* timeinfo;
        timeinfo = localtime(&transactionTime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

        // scrivi la transazione nel file nella prima riga vuota (append)
        outFile << iban << "," << getType() << "," << getAmount() << "," << buffer << "," << getDescription() << endl;
        outFile.close();
    } else {
        throw runtime_error("Errore: impossibile aprire il file " + filePath + " per salvare la transazione.");
    }
}
