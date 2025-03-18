#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Persona.h"
#include "Transaction.h"
#include <string>
#include <vector>
#include <memory>

//TO_DO ricerca trasazioni, elimina transazioni, tutto in base ad una descrizione o data o parola data, check yransazioni esistenti e non (se il saldo in cambia)
//controllo transazioni
class Account {
private:
    std::string iban;
    Persona intestatario;
    std::string fileRiferimento;
    double saldo;

public:
    Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento);
    ~Account(); 

    void addTransaction(std::unique_ptr<Transaction> transaction);
    void updateSaldo(double amount);  
    
    
    void saveToAccountFile() const; 


    std::string getIban() const;
    std::string getNome() const;
    std::string getCognome() const;
    std::string getCodicefiscale() const;
    std::string getFileRiferimento() const;
    double getSaldo() const; 
    
    static Account loadFromFile(const std::string& filePath);


    void deleteTransaction();
    void deleteTransactionFromLog(const std::string& filePath, Transaction* t);
    
    void printTransactions() const;
};

#endif
