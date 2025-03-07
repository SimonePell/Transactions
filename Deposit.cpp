#include "Deposit.h"
#include "Account.h"
#include <fstream>
#include <iostream>

using namespace std;

Deposit::Deposit(double amount) : amount(amount) {this->amount=amount;}

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