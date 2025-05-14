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
    std::vector<std::unique_ptr<Transaction>> transazioni;

public:
    Account(const std::string& iban, const Persona& intestatario, const std::string& fileRiferimento);
    ~Account();

    void addTransaction(std::unique_ptr<Transaction> transaction, const std::string& filePath = TRANSACTIONS_PATH);
    void updateSaldo(double amount);
    void saveToAccountFile() const;

    static Account loadFromFile(const std::string& filePath);

    // Getter
    std::string getIban() const;
    std::string getNome() const;
    std::string getCognome() const;
    std::string getCodicefiscale() const;
    std::string getFileRiferimento() const;
    double getSaldo() const;
    bool hasTransactions(const std::string& filePath = TRANSACTIONS_PATH) const;

    // Accesso alle transazioni
    const std::vector<std::unique_ptr<Transaction>>& getTransazioni() const;
    const Transaction* getTransactionByIndex(int index) const;

    Transaction* operator[](size_t index);
    const Transaction* operator[](size_t index) const;

    // Ricerca
    void searchTransaction(const std::string& query, const std::string& filePath = TRANSACTIONS_PATH) const;
    std::vector<const Transaction*> searchTransaction(const std::string& query) const;

    // Modifica (overload)
    void modifyTransactionByIndex(int index, const std::string& filePath = TRANSACTIONS_PATH);
    bool modifyTransactionByIndex(int index, const std::string& nuovaDescrizione, const std::string& filePath = TRANSACTIONS_PATH);

    void modifyTransactionBySearch(const std::string& query, const std::string& filePath = TRANSACTIONS_PATH);
    bool modifyTransactionBySearch(const std::string& query, int matchIndex, const std::string& nuovaDescrizione, const std::string& filePath = TRANSACTIONS_PATH);

    // Eliminazione (overload)
    void deleteTransactionByIndex(int index, const std::string& filePath = TRANSACTIONS_PATH);
    bool deleteTransactionByIndex(int index, const std::string& filePath);

    void deleteTransactionBySearch(const std::string& query, const std::string& filePath = TRANSACTIONS_PATH);
    bool deleteTransactionBySearch(const std::string& query, int matchIndex, const std::string& filePath = TRANSACTIONS_PATH);

    // Visualizzazione
    void printTransactions(const std::string& filePath = TRANSACTIONS_PATH) const;
};

#endif
