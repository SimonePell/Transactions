#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

#include "Transaction.h"
#include "Account.h"
#include <string>

class Withdrawal : public Transaction {
public:
    //nuova transazione
    explicit Withdrawal(double amount, std::string description = "Nessuna Descrizione"): Transaction(amount, std::move(description)) {}
    //transazione da caricare
    explicit Withdrawal(double amount, std::string description, std::time_t timeStamp): Transaction(amount, std::move(description), timeStamp) {}
    void apply(Account& account) const override;
    void save(const std::string& filePath, double currentBalance) const override;
    std::string getType() const override { return "Withdrawal"; }
    double getAmount() const override { return amount; }
    void logTransaction(const std::string& filePath, const std::string& iban )const{}
};

#endif 
