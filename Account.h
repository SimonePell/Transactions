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

    Account(const Account&) = delete;
    Account& operator=(const Account&) = delete;
    Account(Account&&) noexcept = default;
    Account& operator=(Account&&) noexcept = default;


    // Gestione transazioni
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

    const std::vector<std::unique_ptr<Transaction>>& getTransazioni() const;
    const Transaction* getTransactionByIndex(int index) const;

    Transaction* operator[](size_t index);
    const Transaction* operator[](size_t index) const;

    // Overload SEARCH
    std::vector<const Transaction*> searchTransaction(const std::string& query) const;
    std::vector<const Transaction*> searchTransaction(const std::string& query, const std::string& filePath) const;

    // Overload MODIFY
    bool modifyTransaction(int index, const std::string& nuovaDescrizione, const std::string& filePath = TRANSACTIONS_PATH);
    bool modifyTransaction(const std::string& query, int matchIndex, const std::string& nuovaDescrizione, const std::string& filePath = TRANSACTIONS_PATH);

    // Overload DELETE
    bool deleteTransaction(int index, const std::string& filePath = TRANSACTIONS_PATH);
    bool deleteTransaction(const std::string& query, int matchIndex, const std::string& filePath = TRANSACTIONS_PATH);

    void printTransactions() const;
};

#endif
