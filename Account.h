#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Persona.h"
#include "Transaction.h"
#include <string>

class Account {
private:
    std::string iban;
    Persona intestatario;
    std::string fileRiferimento;
    double saldo;

public:
    Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento);

    void addTransaction(Transaction* transaction);
    void updateSaldo(double amount);          
    void saveToFile() const;            
    void createFile() const;                                
    double getSaldo() const; 
    static Account loadFromFile(const std::string& filePath);
    std::string getIban() const;
    std::string getNome() const;
    std::string getCognome() const;
    std::string getCodicefiscale() const;
    std::string getFileRiferimento() const;
};

#endif
