#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "Transaction.h"
#include "Account.h"
#include <string>

class Deposit : public Transaction {
public:
    //nuova transazione
    explicit Deposit(double amount, std::string description, std::string iban);

    //transazione da caricare
    explicit Deposit(double amount, std::string description, std::time_t timeStamp, std::time_t lastMod, std::string iban);

    //funzioni utilizzate per una nuova transazione
    void apply(Account& account) const override;
    void save(const std::string& filePath, double currentBalance) const override;
    void logTransaction(const std::string& filePath, const std::string& iban )const override;

    //funzioni utilizzate per modificare una transazione
    void modifyDescription(const std::string& newDescription) override;
    void updateLogTransaction(const std::string& filePath, const Transaction& updatedTransaction) override;

    std::string getType() const override { return "Deposit"; }
    double getAmount() const override { return amount; }
};

#endif
