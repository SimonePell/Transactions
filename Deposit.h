#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "Transaction.h"
#include "Account.h"
#include <string>

class Deposit : public Transaction {
private:
    double amount;
public:
    explicit Deposit(double amount);
    void apply(Account& account) const override;
    void save(const std::string& filePath, double currentBalance) const override;
    std::string getType() const override { return "Deposit"; }
    double getAmount() const override { return amount; }
};

#endif