#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

#include "Transaction.h"
#include "Account.h"
#include <string>

class Withdrawal : public Transaction {
public:
    explicit Withdrawal(double amount);
    void apply(Account& account) const override;
    void save(const std::string& filePath, double currentBalance) const override;
    std::string getType() const override { return "Withdrawal"; }
    double getAmount() const override { return amount; }
};

#endif 
