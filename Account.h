#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Persona.h"
#include "Transaction.h"
#include <string>
#include <vector>
#include <memory>

const std::string TRANSACTIONS_PATH = "TRANSACTION/transazioni.txt"; 

class Account {
private:
    std::string iban;
    Persona intestatario;
    std::string fileRiferimento;
    double saldo;

public:
    Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento);
    ~Account(); 

    void addTransaction(std::unique_ptr<Transaction> transaction, const std::string& filePath=TRANSACTIONS_PATH);
    void updateSaldo(double amount);  
    
    
    void saveToAccountFile() const; 


    std::string getIban() const;
    std::string getNome() const;
    std::string getCognome() const;
    std::string getCodicefiscale() const;
    std::string getFileRiferimento() const;
    double getSaldo() const;
    bool hasTransactions(const std::string& filePath = TRANSACTIONS_PATH) const; 
    
    static Account loadFromFile(const std::string& filePath);

    void modifyTransactionByIndex(int index, const std::string& filePath=TRANSACTIONS_PATH);
    void modifyTransactionBySearch(const std::string& query, const std::string& filePath=TRANSACTIONS_PATH);

    void searchTransaction(const std::string& query, const std::string& filePath=TRANSACTIONS_PATH) const;

    void deleteTransactionByIndex(int index, const std::string &filePath = TRANSACTIONS_PATH) ;
    void deleteTransactionBySearch(const std::string& query, const std::string& filePath=TRANSACTIONS_PATH);
    
    void printTransactions(const std::string& filePath=TRANSACTIONS_PATH) const;
};

#endif
