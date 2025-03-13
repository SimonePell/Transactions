#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Persona.h"
#include "Transaction.h"
#include <string>
#include <vector>

//TO_DO ricerca trasazioni, elimina transazioni, tutto in base ad una descrizione o data o parola data, check yransazioni esistenti e non (se il saldo in cambia)
//controllo transazioni
class Account {
private:
    std::string iban;
    Persona intestatario;
    std::string fileRiferimento;
    double saldo;
    std::vector<Transaction*>transazioni;

public:
    Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento);
    ~Account(); 

    void addTransaction(Transaction* transaction);
    void updateSaldo(double amount);          
    void saveToFile() const; 

    std::string getIban() const;
    std::string getNome() const;
    std::string getCognome() const;
    std::string getCodicefiscale() const;
    std::string getFileRiferimento() const;
    double getSaldo() const; 
    void getTransactions(const std::string& transactionsFilePath);
    
    static Account loadFromFile(const std::string& filePath);

    std::vector<Transaction*> findTransactionsByDescription(const std::string& desc) const;
    std::vector<Transaction*> findTransactionsByDate(const std::string& date) const;
    Transaction* findTransactionByIndex(int index) const;

    void deleteTransaction(int id);
    
    void printTransactions() const;
};

#endif
